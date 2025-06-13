#include "JHPTcpClientArchitecture.h"
#include "../SDK/Include/HPSocket/TcpClientSystem.h"


std::shared_ptr<JHPTcpClientArchitecture> JHPTcpClientArchitecture::instance = nullptr;
std::mutex JHPTcpClientArchitecture::mutex;

void JHPTcpClientArchitecture::Init()
{
    this->RegisterSystem(std::make_shared<TcpClientSystem>());
}