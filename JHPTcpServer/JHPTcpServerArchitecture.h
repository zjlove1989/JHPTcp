#pragma once
#include "../SDK/JFramework.h"
#include <mutex>
#include <memory>

using namespace JFramework;
class JHPTcpServerArchitecture : public Architecture {
public:
    // ������̬�� shared_ptr ʵ��
    static std::shared_ptr<JHPTcpServerArchitecture> Instance()
    {
        std::lock_guard<std::mutex> lock(mutex); // ����
        if (!instance) {
            instance = std::make_shared<JHPTcpServerArchitecture>();
            instance->InitArchitecture();
        }
        return instance;
    }

protected:
    void Init() override;

private:
    static std::shared_ptr<JHPTcpServerArchitecture> instance; // ��̬ʵ��
    static std::mutex mutex; // ��̬������
};
