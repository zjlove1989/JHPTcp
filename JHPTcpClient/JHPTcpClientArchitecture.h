#pragma once
#include "../SDK/JFramework.h"
#include <mutex>
#include <memory>

using namespace JFramework;
class JHPTcpClientArchitecture : public Architecture {
public:
    // ������̬�� shared_ptr ʵ��
    static std::shared_ptr<JHPTcpClientArchitecture> Instance()
    {
        std::lock_guard<std::mutex> lock(mutex); // ����
        if (!instance) {
            instance = std::make_shared<JHPTcpClientArchitecture>();
            instance->InitArchitecture();
        }
        return instance;
    }

protected:
    void Init() override;

private:
    static std::shared_ptr<JHPTcpClientArchitecture> instance; // ��̬ʵ��
    static std::mutex mutex; // ��̬������
};
