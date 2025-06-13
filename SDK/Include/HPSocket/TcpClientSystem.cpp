#include "TcpClientSystem.h"
#include "HPClientEvent.h"
TcpClientSystem::TcpClientSystem()
	: m_client(this)
{
}

TcpClientSystem::~TcpClientSystem()
{
}

bool TcpClientSystem::Start(const wchar_t* bindAddress, uint16_t port)
{
	return m_client->Start(bindAddress, port);
}

bool TcpClientSystem::Stop()
{
	if (m_client && m_client->HasStarted()) {
		return m_client->Stop();
	}

	return true;
}

bool TcpClientSystem::Send(const BYTE* data, int length)
{
	if (m_client && m_client->HasStarted()) {
		return m_client->Send(data, length);
	}
	return false;
}

bool TcpClientSystem::SendString(const std::string& str)
{
	if (m_client && m_client->HasStarted()) {
		return Send(reinterpret_cast<const BYTE*>(str.c_str()), static_cast<int>(str.length()));
	}
	return false;
}

EnHandleResult TcpClientSystem::OnReceive(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
{
	this->SendEvent<HPClientReceiveEvent>(pSender, dwConnID, pData, iLength);
	return HR_OK;
}

EnHandleResult TcpClientSystem::OnSend(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
{
	this->SendEvent<HPClientSendEvent>(pSender, dwConnID, pData, iLength);
	return HR_OK;
}

EnHandleResult TcpClientSystem::OnClose(ITcpClient* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
{
	this->SendEvent<HPClientCloseEvent>(pSender, dwConnID, enOperation, iErrorCode);
	return HR_OK;
}

EnHandleResult TcpClientSystem::OnPrepareConnect(ITcpClient* pSender, CONNID dwConnID, SOCKET socket)
{
	this->SendEvent<HPClientPrepareConnectEvent>(pSender, dwConnID, socket);
	return HR_OK;
}

EnHandleResult TcpClientSystem::OnConnect(ITcpClient* pSender, CONNID dwConnID)
{
	this->SendEvent<HPClientConnectEvent>(pSender, dwConnID);
	return HR_OK;
}

EnHandleResult TcpClientSystem::OnHandShake(ITcpClient* pSender, CONNID dwConnID)
{
	this->SendEvent<HPClientHandShakeEvent>(pSender, dwConnID);
	return HR_OK;
}

void TcpClientSystem::OnInit()
{
}

void TcpClientSystem::OnDeinit()
{
}

void TcpClientSystem::OnEvent(std::shared_ptr<IEvent> event)
{
}