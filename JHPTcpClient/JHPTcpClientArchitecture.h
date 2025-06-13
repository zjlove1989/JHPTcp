#pragma once
#include "../SDK/JFramework.h"
#include <mutex>
#include <memory>

using namespace JFramework;
class JHPTcpClientArchitecture : public Architecture {
public:
    // ÉùÃ÷¾²Ì¬µÄ shared_ptr ÊµÀı
    static std::shared_ptr<JHPTcpClientArchitecture> Instance()
    {
        std::lock_guard<std::mutex> lock(mutex); // ¼ÓËø
        if (!instance) {
            instance = std::make_shared<JHPTcpClientArchitecture>();
            instance->InitArchitecture();
        }
        return instance;
    }

protected:
    void Init() override;

private:
    static std::shared_ptr<JHPTcpClientArchitecture> instance; // ¾²Ì¬ÊµÀı
    static std::mutex mutex; // ¾²Ì¬»¥³âËø
};
