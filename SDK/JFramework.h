/****************************************************************************
 * Copyright (c) 2025 zjlove1989

 * https://github.com/zjlove1989/JFramework
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ****************************************************************************/

#ifndef JFRAMEWORK
#define JFRAMEWORK

#include <exception>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace JFramework {
// ================ �쳣���� ================
class FrameworkException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class ArchitectureNotSetException : public FrameworkException {
public:
    ArchitectureNotSetException(const std::string& typeName)
        : FrameworkException("Architecture not available: " + typeName)
    {
    }
};

class ComponentNotRegisteredException : public FrameworkException {
public:
    explicit ComponentNotRegisteredException(const std::string& typeName)
        : FrameworkException("Component not registered: " + typeName)
    {
    }
};

class ComponentAlreadyRegisteredException : public FrameworkException {
public:
    explicit ComponentAlreadyRegisteredException(const std::string& typeName)
        : FrameworkException("Component already registered: " + typeName)
    {
    }
};

// ================ ǰ������ ================
class ISystem;
class IModel;
class ICommand;
template <typename _Ty>
class IQuery;
class IUtility;
template <typename _Ty>
class BindableProperty;
class IOCContainer;

/// @brief �¼��ӿ�
class IEvent {
public:
    virtual ~IEvent() = default;
};

/// @brief ����Event����
class ICanHandleEvent {
public:
    virtual ~ICanHandleEvent() = default;
    virtual void HandleEvent(std::shared_ptr<IEvent> event) = 0;
};

/// @brief �¼�����ʵ��
class EventBus {
public:
    void RegisterEvent(std::type_index eventType, ICanHandleEvent* handler)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mSubscribers[eventType.name()].push_back(handler);
    }

    void SendEvent(std::shared_ptr<IEvent> event)
    {
        std::vector<ICanHandleEvent*> subscribers;
        {
            std::lock_guard<std::mutex> lock(mMutex);
            auto it = mSubscribers.find(typeid(*event).name());
            if (it != mSubscribers.end()) {
                subscribers = it->second;
            }
        }

        for (auto& handler : subscribers) {
            try {
                handler->HandleEvent(event);
            } catch (const std::exception&) {
            }
        }
    }

    void UnRegisterEvent(std::type_index eventType, ICanHandleEvent* handler)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        auto it = mSubscribers.find(eventType.name());
        if (it != mSubscribers.end()) {
            auto& handlers = it->second;
            auto handlerIt = std::find(handlers.begin(), handlers.end(), handler);
            if (handlerIt != handlers.end()) {
                handlers.erase(handlerIt);
                if (handlers.empty()) {
                    mSubscribers.erase(it);
                }
            }
        }
    }

    void Clear()
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mSubscribers.clear();
    }

private:
    std::mutex mMutex;
    std::unordered_map<std::string, std::vector<ICanHandleEvent*>> mSubscribers;
};

// ================ ���ļܹ��ӿ� ================

/// @brief �ܹ����Ľӿ�
class IArchitecture : public std::enable_shared_from_this<IArchitecture> {
public:
    virtual ~IArchitecture() = default;
    // ��������������������ʵ�� shared_from_this()
    virtual std::shared_ptr<IArchitecture> GetSharedFromThis() = 0;

    // �������
    virtual void SendCommand(std::unique_ptr<ICommand> command) = 0;

    virtual void Deinit() = 0;

protected:
    // ע�����
    virtual void RegisterSystem(std::type_index typeId,
        std::shared_ptr<ISystem> system)
        = 0;
    virtual void RegisterModel(std::type_index typeId,
        std::shared_ptr<IModel> model)
        = 0;
    virtual void RegisterUtility(std::type_index typeId,
        std::shared_ptr<IUtility> utility)
        = 0;

    // ��ȡ���
    virtual std::shared_ptr<ISystem> GetSystem(std::type_index typeId) = 0;
    virtual std::shared_ptr<IModel> GetModel(std::type_index typeId) = 0;
    virtual std::shared_ptr<IUtility> GetUtility(std::type_index typeId) = 0;

    // �¼�����
    virtual void SendEvent(std::shared_ptr<IEvent> event) = 0;
    virtual void RegisterEvent(std::type_index eventType,
        ICanHandleEvent* handler)
        = 0;
    virtual void UnRegisterEvent(std::type_index eventType,
        ICanHandleEvent* handler)
        = 0;

public:
    template <typename _Ty>
    void RegisterSystem(std::shared_ptr<_Ty> system)
    {
        static_assert(std::is_base_of_v<ISystem, _Ty>,
            "_Ty must inherit from ISystem");
        RegisterSystem(typeid(_Ty), std::static_pointer_cast<ISystem>(system));
    }

    template <typename _Ty>
    void RegisterModel(std::shared_ptr<_Ty> model)
    {
        static_assert(std::is_base_of_v<IModel, _Ty>,
            "_Ty must inherit from IModel");
        RegisterModel(typeid(_Ty), std::static_pointer_cast<IModel>(model));
    }

    template <typename _Ty>
    void RegisterUtility(std::shared_ptr<_Ty> utility)
    {
        if (!utility) {
            throw std::invalid_argument("IUtility cannot be null");
        }
        static_assert(std::is_base_of_v<IUtility, _Ty>,
            "_Ty must inherit from IUtility");
        RegisterUtility(typeid(_Ty), std::static_pointer_cast<IUtility>(utility));
    }

    template <typename _Ty>
    std::shared_ptr<_Ty> GetSystem()
    {
        auto system = GetSystem(typeid(_Ty));
        if (!system) {
            throw ComponentNotRegisteredException(typeid(_Ty).name());
        }
        return std::dynamic_pointer_cast<_Ty>(system);
    }

    template <typename _Ty>
    std::shared_ptr<_Ty> GetModel()
    {
        auto model = GetModel(typeid(_Ty));
        if (!model) {
            throw ComponentNotRegisteredException(typeid(_Ty).name());
        }
        return std::dynamic_pointer_cast<_Ty>(model);
    }

    template <typename _Ty>
    std::shared_ptr<_Ty> GetUtility()
    {
        auto utility = GetUtility(typeid(_Ty));
        if (!utility) {
            throw ComponentNotRegisteredException(typeid(_Ty).name());
        }
        return std::dynamic_pointer_cast<_Ty>(utility);
    }

    template <typename _Ty>
    void RegisterEvent(ICanHandleEvent* handler)
    {
        if (!handler) {
            throw std::invalid_argument("ICanHandleEvent cannot be null");
        }
        static_assert(std::is_base_of_v<IEvent, _Ty>,
            "_Ty must inherit from IEvent");
        mEventBus->RegisterEvent(typeid(_Ty), handler);
    }

    template <typename _Ty>
    void UnRegisterEvent(ICanHandleEvent* handler)
    {
        if (!handler) {
            throw std::invalid_argument("ICanHandleEvent cannot be null");
        }
        static_assert(std::is_base_of_v<IEvent, _Ty>,
            "_Ty must inherit from IEvent");
        mEventBus->UnRegisterEvent(typeid(_Ty), handler);
    }

    template <typename _Ty, typename... Args>
    void SendEvent(Args&&... args)
    {
        static_assert(std::is_base_of_v<IEvent, _Ty>,
            "_Ty must inherit from IEvent");
        this->SendEvent(std::make_shared<_Ty>(std::forward<Args>(args)...));
    }

    template <typename _Ty, typename... Args>
    void SendCommand(Args&&... args)
    {
        static_assert(std::is_base_of_v<ICommand, _Ty>,
            "_Ty must inherit from ICommand");
        this->SendCommand(std::make_unique<_Ty>(std::forward<Args>(args)...));
    }

    // ----------------------------------Query--------------------------------------//
    template <typename _Ty>
    auto SendQuery(std::unique_ptr<_Ty> query) -> decltype(query->Do())
    {
        static_assert(std::is_base_of_v<IQuery<decltype(query->Do())>, _Ty>,
            "_Ty must inherit from IQuery");

        if (!query) {
            throw std::invalid_argument("Query cannot be null");
        }
        query->SetArchitecture(GetSharedFromThis());
        return query->Do();
    }

    template <typename _Ty, typename... Args>
    auto SendQuery(Args&&... args) -> decltype(std::declval<_Ty>().Do())
    {
        static_assert(
            std::is_base_of_v<IQuery<decltype(std::declval<_Ty>().Do())>, _Ty>,
            "_Ty must inherit from IQuery");

        auto query = std::make_unique<_Ty>(std::forward<Args>(args)...);
        return this->SendQuery(std::move(query));
    }

    bool IsInitialized() const { return mInitialized; }

protected:
    bool mInitialized = false;
    std::unique_ptr<IOCContainer> mContainer;
    std::unique_ptr<EventBus> mEventBus;
};

// ================ �����ӿ� ================

// ��ע���ӿ�
class IUnRegister {
public:
    virtual ~IUnRegister() = default;
    virtual void UnRegister() = 0;
};

class UnRegisterTrigger {
public:
    virtual ~UnRegisterTrigger() { this->UnRegister(); }

    void AddUnRegister(std::shared_ptr<IUnRegister> unRegister)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mUnRegisters.push_back(std::move(unRegister));
    }

    void UnRegister()
    {
        std::lock_guard<std::mutex> lock(mMutex);
        for (auto& unRegister : mUnRegisters) {
            unRegister->UnRegister();
        }
        mUnRegisters.clear();
    }

protected:
    std::mutex mMutex;
    std::vector<std::shared_ptr<IUnRegister>> mUnRegisters;
};

template <typename _Ty>
class BindablePropertyUnRegister
    : public IUnRegister,
      public std::enable_shared_from_this<BindablePropertyUnRegister<_Ty>> {
public:
    BindablePropertyUnRegister(int id,
        BindableProperty<_Ty>* property,
        std::function<void(_Ty)> callback)
        : mProperty(property)
        , mCallback(std::move(callback))
        , mId(id)
    {
    }
    void UnRegisterWhenObjectDestroyed(UnRegisterTrigger* unRegisterTrigger)
    {
        unRegisterTrigger->AddUnRegister(this->shared_from_this());
    }
    int GetId() const { return mId; }

    void SetProperty(BindableProperty<_Ty>* property)
    {
        mProperty = property;
    }

    void UnRegister() override
    {
        if (mProperty) {
            mProperty->UnRegister(mId);
            mProperty = nullptr; // ��ֹ�ظ�����
        }
    }

    void Invoke(_Ty value)
    {
        if (mCallback) {
            mCallback(std::move(value));
        }
    }

protected:
    int mId;

private:
    BindableProperty<_Ty>* mProperty;
    std::function<void(_Ty)> mCallback;
};

// �ɹ۲�������
template <typename _Ty>
class BindableProperty {
public:
    BindableProperty() = default;
    // ��ֹ��������Ϳ�����ֵ
    BindableProperty(const BindableProperty&) = delete;
    BindableProperty& operator=(const BindableProperty&) = delete;
    BindableProperty(BindableProperty&& other) noexcept
    {
        std::lock_guard<std::mutex> lock(other.mMutex);
        mValue = std::move(other.mValue);
        mObservers = std::move(other.mObservers);
        mNextId = other.mNextId;
        // ��������observer��mPropertyָ��
        for (auto& observer : mObservers) {
            if (observer)
                observer->SetProperty(this);
        }
        other.mNextId = 0;
    }

    BindableProperty& operator=(BindableProperty&& other) noexcept
    {
        if (this != &other) {
            std::lock_guard<std::mutex> lock1(mMutex);
            std::lock_guard<std::mutex> lock2(other.mMutex);
            mValue = std::move(other.mValue);
            mObservers = std::move(other.mObservers);
            mNextId = other.mNextId;
            for (auto& observer : mObservers) {
                if (observer)
                    observer->SetProperty(this);
            }
            other.mNextId = 0;
        }
        return *this;
    }
    explicit BindableProperty(const _Ty& value)
        : mValue(std::move(value))
    {
    }

    ~BindableProperty()
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mObservers.clear();
    }

    // ��ȡ��ǰֵ
    const _Ty& GetValue() const { return mValue; }

    // ������ֵ
    void SetValue(const _Ty& newValue)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        if (mValue == newValue)
            return;
        mValue = newValue;
        for (auto& observer : mObservers) {
            try {
                observer->Invoke(mValue);
            } catch (...) {
            }
        }
    }

    // ������֪ͨ������
    void SetValueWithoutEvent(const _Ty& newValue)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mValue = newValue;
    }

    // ע��۲��ߣ�����ʼֵ֪ͨ��
    std::shared_ptr<BindablePropertyUnRegister<_Ty>> RegisterWithInitValue(
        std::function<void(const _Ty&)> onValueChanged)
    {
        onValueChanged(mValue);
        return Register(std::move(onValueChanged));
    }

    // ע��۲���
    std::shared_ptr<BindablePropertyUnRegister<_Ty>> Register(
        std::function<void(const _Ty&)> onValueChanged)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        auto unRegister = std::make_shared<BindablePropertyUnRegister<_Ty>>(
            mNextId++, this, std::move(onValueChanged));
        mObservers.push_back(unRegister);
        return unRegister;
    }

    // ע���۲���
    void UnRegister(int id)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        for (size_t i = 0; i < mObservers.size(); i++) {
            auto& observer = mObservers[i];
            if (observer && observer->GetId() == id) {
                mObservers.erase(mObservers.begin() + i);
                break; // �ҵ���ע�����˳�ѭ��
            }
        }
    }

    // ���������أ�����ʹ��
    operator _Ty() const { return mValue; }
    BindableProperty<_Ty>& operator=(const _Ty& newValue)
    {
        SetValue(std::move(newValue));
        return *this;
    }

private:
    std::mutex mMutex;
    int mNextId = 0;
    _Ty mValue;
    std::vector<std::shared_ptr<BindablePropertyUnRegister<_Ty>>> mObservers;
};

/// @brief ��ʼ���ӿ�
class ICanInit {
public:
    bool IsInitialized() const { return mInitialized; }
    void SetInitialized(bool initialized) { mInitialized = initialized; }
    virtual ~ICanInit() = default;
    virtual void Init() = 0;
    virtual void Deinit() = 0;

protected:
    bool mInitialized = false;
};

/// @brief �ܹ������ӿ�
class IBelongToArchitecture {
public:
    virtual ~IBelongToArchitecture() = default;
    virtual std::weak_ptr<IArchitecture> GetArchitecture() const = 0;
};

/// @brief �ܹ����ýӿ�
class ICanSetArchitecture {
public:
    virtual ~ICanSetArchitecture() = default;
    virtual void SetArchitecture(std::shared_ptr<IArchitecture> architecture) = 0;
};

// ================ ���ܽӿ� ================

/// @brief ��ȡModel����
class ICanGetModel : public IBelongToArchitecture {
public:
    template <typename _Ty>
    std::shared_ptr<_Ty> GetModel()
    {
        auto arch = GetArchitecture().lock();
        if (!arch) {
            throw ArchitectureNotSetException(typeid(_Ty).name());
        }

        auto model = arch->GetModel<_Ty>();
        return model;
    }
};

/// @brief ��ȡSystem����
class ICanGetSystem : public IBelongToArchitecture {
public:
    template <typename _Ty>
    std::shared_ptr<_Ty> GetSystem()
    {
        auto arch = GetArchitecture().lock();
        if (!arch) {
            throw ArchitectureNotSetException(typeid(_Ty).name());
        }

        auto system = arch->GetSystem<_Ty>();
        return system;
    }
};

/// @brief ����Command����
class ICanSendCommand : public IBelongToArchitecture {
public:
    template <typename _Ty, typename... Args>
    void SendCommand(Args&&... args)
    {
        auto arch = GetArchitecture().lock();
        if (!arch) {
            throw ArchitectureNotSetException(typeid(_Ty).name());
        }
        arch->SendCommand<_Ty>(std::forward<Args>(args)...);
    }

    void SendCommand(std::unique_ptr<ICommand> command)
    {
        auto arch = GetArchitecture().lock();
        if (!arch) {
            throw ArchitectureNotSetException(typeid(command).name());
        }
        arch->SendCommand(std::move(command));
    }
};

/// @brief ����Command����
class ICanSendQuery : public IBelongToArchitecture {
public:
    template <typename _Ty, typename... Args>
    auto SendQuery(Args&&... args) -> decltype(std::declval<_Ty>().Do())
    {
        auto arch = GetArchitecture().lock();
        if (!arch) {
            throw ArchitectureNotSetException(typeid(_Ty).name());
        }

        auto result = arch->SendQuery<_Ty>(std::forward<Args>(args)...);
        return result;
    }

    template <typename _Ty>
    auto SendQuery(std::unique_ptr<_Ty> query)
        -> decltype(std::declval<_Ty>().Do())
    {
        auto arch = GetArchitecture().lock();
        if (!arch) {
            throw ArchitectureNotSetException(typeid(_Ty).name());
        }

        auto result = arch->SendQuery(std::move(query));
        return result;
    }
};

class ICanGetUtility : public IBelongToArchitecture {
public:
    template <typename _Ty>
    std::shared_ptr<_Ty> GetUtility()
    {
        auto arch = GetArchitecture().lock();
        if (!arch) {
            throw ArchitectureNotSetException(typeid(_Ty).name());
        }

        auto utility = arch->GetUtility<_Ty>();
        return utility;
    }
};

// ================ �¼�ע�������ӿ� ================

/// @brief ����Event����
class ICanSendEvent : public IBelongToArchitecture {
public:
    template <typename _Ty, typename... Args>
    void SendEvent(Args&&... args)
    {
        auto arch = GetArchitecture().lock();
        if (!arch) {
            throw ArchitectureNotSetException(typeid(_Ty).name());
        }
        arch->SendEvent<_Ty>(std::forward<Args>(args)...);
    }
};

/// @brief ע��/ע���¼���������
class ICanRegisterEvent : public IBelongToArchitecture {
public:
    virtual ~ICanRegisterEvent() = default;

    template <typename _Ty>
    void RegisterEvent(ICanHandleEvent* handler)
    {
        static_assert(std::is_base_of_v<IEvent, _Ty>,
            "_Ty must inherit from IEvent");

        auto arch = GetArchitecture().lock();
        if (!arch) {
            throw ArchitectureNotSetException(typeid(_Ty).name());
        }

        arch->RegisterEvent<_Ty>(handler);
    }

    template <typename _Ty>
    void UnRegisterEvent(ICanHandleEvent* handler)
    {
        static_assert(std::is_base_of_v<IEvent, _Ty>,
            "_Ty must inherit from IEvent");

        auto arch = GetArchitecture().lock();
        if (!arch) {
            throw ArchitectureNotSetException(typeid(_Ty).name());
        }

        arch->UnRegisterEvent<_Ty>(handler);
    }
};

// ================ ��������ӿ� ================

/// @brief ����ӿ�
class ICommand : public ICanSetArchitecture,
                 public ICanGetSystem,
                 public ICanGetModel,
                 public ICanSendCommand,
                 public ICanSendEvent,
                 public ICanSendQuery,
                 public ICanGetUtility {
public:
    virtual ~ICommand() override = default;
    virtual void Execute() = 0;
};

/// @brief Model�ӿ�
class IModel : public ICanSetArchitecture,
               public ICanInit,
               public ICanSendEvent,
               public ICanSendQuery,
               public ICanGetUtility,
               public ICanGetModel {
};

/// @brief System�ӿ�
class ISystem : public ICanSetArchitecture,
                public ICanInit,
                public ICanGetModel,
                public ICanHandleEvent,
                public ICanGetSystem,
                public ICanRegisterEvent,
                public ICanSendEvent,
                public ICanSendQuery,
                public ICanGetUtility {
};

// @brief Controller�ӿ�
class IController : public ICanGetSystem,
                    public ICanGetModel,
                    public ICanSendCommand,
                    public ICanSendEvent,
                    public ICanHandleEvent,
                    public ICanRegisterEvent,
                    public ICanSendQuery,
                    public ICanGetUtility {
};

/// @brief Query�ӿ�
template <typename _Ty>
class IQuery : public ICanSetArchitecture,
               public ICanGetModel,
               public ICanGetSystem,
               public ICanSendQuery {
public:
    virtual ~IQuery() override = default;
    virtual _Ty Do() = 0;
};

class IUtility {
public:
    virtual ~IUtility() = default; // �������������
};

// ================ ʵ���� ================

/// @brief IOC����ʵ��
class IOCContainer {
public:
    template <typename _Ty, typename TBase>
    void Register(std::type_index typeId, std::shared_ptr<TBase> component)
    {
        static_assert(std::is_base_of_v<TBase, _Ty>, "_Ty must inherit from TBase");
        auto& container = GetContainer(ContainerTypeTag<TBase> {});
        std::lock_guard<std::mutex> lock(GetMutex(MutexTypeTag<TBase> {}));

        if (container.find(typeId.name()) != container.end())
            throw ComponentAlreadyRegisteredException(typeId.name());

        container[typeId.name()] = std::static_pointer_cast<TBase>(component);
    }

    template <typename TBase>
    std::shared_ptr<TBase> Get(std::type_index typeId)
    {
        auto& container = GetContainer(ContainerTypeTag<TBase> {});
        std::lock_guard<std::mutex> lock(GetMutex(MutexTypeTag<TBase> {}));
        auto it = container.find(typeId.name());
        return it != container.end() ? it->second : nullptr;
    }

    template <typename TBase>
    std::vector<std::shared_ptr<TBase>> GetAll()
    {
        auto& container = GetContainer(ContainerTypeTag<TBase> {});
        std::lock_guard<std::mutex> lock(GetMutex(MutexTypeTag<TBase> {}));
        std::vector<std::shared_ptr<TBase>> result;
        for (auto& pair : container) {
            result.push_back(pair.second);
        }
        return result;
    }

    void Clear()
    {
        std::lock_guard<std::mutex> lock1(mModelMutex);
        std::lock_guard<std::mutex> lock2(mSystemMutex);
        std::lock_guard<std::mutex> lock3(mUtilityMutex);
        mModels.clear();
        mSystems.clear();
        mUtilitys.clear();
    }

private:
    template <typename>
    struct ContainerTypeTag { };
    auto& GetContainer(ContainerTypeTag<IModel>) { return mModels; }
    auto& GetContainer(ContainerTypeTag<ISystem>) { return mSystems; }
    auto& GetContainer(ContainerTypeTag<IUtility>) { return mUtilitys; }

    template <typename>
    struct MutexTypeTag { };
    auto& GetMutex(MutexTypeTag<IModel>) { return mModelMutex; }
    auto& GetMutex(MutexTypeTag<ISystem>) { return mSystemMutex; }
    auto& GetMutex(MutexTypeTag<IUtility>) { return mUtilityMutex; }

    std::unordered_map<std::string, std::shared_ptr<IModel>> mModels;
    std::unordered_map<std::string, std::shared_ptr<ISystem>> mSystems;
    std::unordered_map<std::string, std::shared_ptr<IUtility>> mUtilitys;

    std::mutex mModelMutex;
    std::mutex mSystemMutex;
    std::mutex mUtilityMutex;
};

/// @brief �ܹ�����ʵ��
class Architecture : public IArchitecture {
public:
    // ���� IArchitecture ��ģ�巽��
    using IArchitecture::GetModel;
    using IArchitecture::GetSystem;
    using IArchitecture::GetUtility;
    using IArchitecture::RegisterEvent;
    using IArchitecture::RegisterModel;
    using IArchitecture::RegisterSystem;
    using IArchitecture::RegisterUtility;
    using IArchitecture::SendCommand;
    using IArchitecture::SendEvent;
    using IArchitecture::UnRegisterEvent;

private:
    // ----------------------------------System--------------------------------------//

    void RegisterSystem(std::type_index typeId,
        std::shared_ptr<ISystem> system) override
    {
        if (!system) {
            throw std::invalid_argument("System cannot be null");
        }
        system->SetArchitecture(shared_from_this());
        mContainer->Register<ISystem>(typeId, system);
        if (mInitialized) {
            InitializeComponent(system);
        }
    }

    std::shared_ptr<ISystem> GetSystem(std::type_index typeId) override
    {
        return mContainer->Get<ISystem>(typeId);
    }

    // ----------------------------------Model--------------------------------------//

    void RegisterModel(std::type_index typeId,
        std::shared_ptr<IModel> model) override
    {
        if (!model) {
            throw std::invalid_argument("Model cannot be null");
        }
        model->SetArchitecture(shared_from_this());
        mContainer->Register<IModel>(typeId, model);
        if (mInitialized) {
            InitializeComponent(model);
        }
    }

    std::shared_ptr<IModel> GetModel(std::type_index typeId) override
    {
        return mContainer->Get<IModel>(typeId);
    }

    // ----------------------------------Utility--------------------------------------//

    void RegisterUtility(std::type_index typeId,
        std::shared_ptr<IUtility> utility) override
    {
        mContainer->Register<IUtility>(typeId, utility);
    }

    std::shared_ptr<IUtility> GetUtility(std::type_index typeId) override
    {
        return mContainer->Get<IUtility>(typeId);
    }

    // ----------------------------------Event--------------------------------------//

    void RegisterEvent(std::type_index eventType,
        ICanHandleEvent* handler) override
    {
        if (!handler) {
            throw std::invalid_argument("ICanHandleEvent cannot be null");
        }
        mEventBus->RegisterEvent(eventType, handler);
    }

    void UnRegisterEvent(std::type_index eventType,
        ICanHandleEvent* handler) override
    {
        if (!handler) {
            throw std::invalid_argument("ICanHandleEvent cannot be null");
        }
        mEventBus->UnRegisterEvent(eventType, handler);
    }

public:
    // ʵ�� GetSharedPtr()
    std::shared_ptr<IArchitecture> GetSharedFromThis() final
    {
        return shared_from_this();
    }

    // ----------------------------------Command--------------------------------------//

    void SendCommand(std::unique_ptr<ICommand> command) override
    {
        if (!command) {
            throw std::invalid_argument("ICommand cannot be null");
        }
        command->SetArchitecture(shared_from_this());
        try {
            command->Execute();
        } catch (const std::exception&) {
        }
    }

    void SendEvent(std::shared_ptr<IEvent> event) override
    {
        if (!event) {
            throw std::invalid_argument("IEvent cannot be null");
        }
        mEventBus->SendEvent(event);
    }

    // ----------------------------------Init--------------------------------------//

    void Deinit() final
    {
        if (!mInitialized)
            return;

        mInitialized = false;

        this->OnDeinit();

        for (auto& model : mContainer->GetAll<IModel>()) {
            UnInitializeComponent(model);
        }

        for (auto& system : mContainer->GetAll<ISystem>()) {
            UnInitializeComponent(system);
        }
    }

    virtual void InitArchitecture()
    {
        if (mInitialized)
            return;

        mInitialized = true;

        this->Init();

        for (auto& model : mContainer->GetAll<IModel>()) {
            InitializeComponent(model);
        }

        for (auto& system : mContainer->GetAll<ISystem>()) {
            InitializeComponent(system);
        }
    }

    IOCContainer* GetContainer() { return mContainer.get(); }

protected:
    // ˽�й��캯��
    Architecture()
    {
        mContainer = std::make_unique<IOCContainer>();
        mEventBus = std::make_unique<EventBus>();
        mInitialized = false;
    }

    virtual ~Architecture() = default;

    virtual void Init() = 0;

    virtual void OnDeinit() { }

private:
    template <typename _Ty>
    void InitializeComponent(std::shared_ptr<_Ty> component)
    {
        static_assert(std::is_base_of_v<ICanInit, _Ty>,
            "Component must implement ICanInit");

        if (!component->IsInitialized()) {
            component->Init();
            component->SetInitialized(true);
        }
    }

    template <typename _Ty>
    void UnInitializeComponent(std::shared_ptr<_Ty> component)
    {
        static_assert(std::is_base_of_v<ICanInit, _Ty>,
            "Component must implement ICanInit");

        if (component->IsInitialized()) {
            component->Deinit();
            component->SetInitialized(false);
        }
    }
};

// ================ ������� ================

/// @brief ����Command
class AbstractCommand : public ICommand {
private:
    std::weak_ptr<IArchitecture> mArchitecture;

public:
    std::weak_ptr<IArchitecture> GetArchitecture() const final
    {
        return mArchitecture;
    }

    void Execute() final { this->OnExecute(); }

public:
    void SetArchitecture(std::shared_ptr<IArchitecture> architecture) final
    {
        mArchitecture = architecture;
    }

protected:
    virtual void OnExecute() = 0;
};

class AbstractModel : public IModel {
private:
    std::weak_ptr<IArchitecture> mArchitecture;

public:
    std::weak_ptr<IArchitecture> GetArchitecture() const final
    {
        return mArchitecture;
    }

    virtual void Init() final { this->OnInit(); }

    void Deinit() final { this->OnDeinit(); }

private:
    void SetArchitecture(std::shared_ptr<IArchitecture> architecture) final
    {
        mArchitecture = architecture;
    }

protected:
    virtual void OnInit() = 0;
    virtual void OnDeinit() = 0;
};

class AbstractSystem : public ISystem {
private:
    std::weak_ptr<IArchitecture> mArchitecture;

public:
    std::weak_ptr<IArchitecture> GetArchitecture() const final
    {
        return mArchitecture;
    }

    virtual void Init() final { this->OnInit(); }

    void Deinit() final { OnDeinit(); }

    void HandleEvent(std::shared_ptr<IEvent> event) final { OnEvent(event); }

private:
    void SetArchitecture(std::shared_ptr<IArchitecture> architecture) final
    {
        mArchitecture = architecture;
    }

protected:
    virtual void OnInit() = 0;
    virtual void OnDeinit() = 0;
    virtual void OnEvent(std::shared_ptr<IEvent> event) = 0;
};

class AbstractController : public IController {
private:
    void HandleEvent(std::shared_ptr<IEvent> event) final { OnEvent(event); }

protected:
    virtual void OnEvent(std::shared_ptr<IEvent> event) = 0;
};

template <typename _Ty>
class AbstractQuery : public IQuery<_Ty> {
private:
    std::weak_ptr<IArchitecture> mArchitecture;

public:
    std::weak_ptr<IArchitecture> GetArchitecture() const final
    {
        return mArchitecture;
    }

    _Ty Do() final { return OnDo(); }

public:
    void SetArchitecture(std::shared_ptr<IArchitecture> architecture) final
    {
        mArchitecture = architecture;
    }

protected:
    virtual _Ty OnDo() = 0;
};
}; // namespace JFramework

#endif // !_JFRAMEWORK_
