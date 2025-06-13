#include "TcpServerSystem.h"
#include "HPServerEvent.h"

TcpServerSystem::TcpServerSystem()
    : mServer(this)
{
}

TcpServerSystem::~TcpServerSystem() { }

void TcpServerSystem::OnInit() { }

void TcpServerSystem::OnDeinit() { }

void TcpServerSystem::OnEvent(std::shared_ptr<IEvent> event) { }

bool TcpServerSystem::Start(const wchar_t* bindAddress, uint16_t port)
{
    return mServer->Start(bindAddress, port);
}

bool TcpServerSystem::Stop()
{
    if (mServer->HasStarted()) {
        return mServer->Stop();
    }

    return true;
}

bool TcpServerSystem::Send(HP_CONNID connId, const BYTE* data, int length)
{
    if (!mServer->HasStarted()) {
        return false;
    }

    return mServer->Send(connId, data, length);
}

bool TcpServerSystem::Disconnect(HP_CONNID connId)
{
    if (!mServer->HasStarted()) {
        return false;
    }

    return mServer->Disconnect(connId);
}

uint32_t TcpServerSystem::GetConnectionCount() const
{
    return mServer->GetConnectionCount();
}

EnHandleResult TcpServerSystem::OnPrepareListen(ITcpServer* pSender,
    SOCKET soListen)
{
    this->SendEvent<HPServerPrepareListenEvent>(pSender,soListen);
    return HR_OK;
}

EnHandleResult TcpServerSystem::OnAccept(ITcpServer* pSender, CONNID dwConnID,
    UINT_PTR soClient)
{
    this->SendEvent<HPServerAcceptEvent>(pSender,dwConnID, soClient);
    return HR_OK;
}

EnHandleResult TcpServerSystem::OnHandShake(ITcpServer* pSender,
    CONNID dwConnID)
{
    this->SendEvent<HPServerHandShakeEvent>(pSender, dwConnID);
    return HR_OK;
}

EnHandleResult TcpServerSystem::OnReceive(ITcpServer* pSender, CONNID dwConnID,
    const BYTE* pData, int iLength)
{
    this->SendEvent<HPServerReceiveEvent>(pSender, dwConnID, pData, iLength);
    return HR_OK;
}

EnHandleResult TcpServerSystem::OnClose(ITcpServer* pSender, CONNID dwConnID,
    EnSocketOperation enOperation,
    int iErrorCode)
{
    this->SendEvent<HPServerCloseEvent>(pSender, dwConnID, enOperation, iErrorCode);
    return HR_OK;
}

EnHandleResult TcpServerSystem::OnSend(ITcpServer* pSender, CONNID dwConnID,
    const BYTE* pData, int iLength)
{
    this->SendEvent<HPServerSendEvent>(pSender, dwConnID, pData, iLength);
    return HR_OK;
}

EnHandleResult TcpServerSystem::OnShutdown(ITcpServer* pSender)
{
    this->SendEvent<HPServerShutdownEvent>(pSender);
    return HR_OK;
}