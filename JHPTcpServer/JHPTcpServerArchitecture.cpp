#include "JHPTcpServerArchitecture.h"
#include "TcpServerSystem.h"

std::shared_ptr<JHPTcpServerArchitecture> JHPTcpServerArchitecture::instance = nullptr;
std::mutex JHPTcpServerArchitecture::mutex;

void JHPTcpServerArchitecture::Init()
{
    this->RegisterSystem(std::make_shared<TcpServerSystem>());
}