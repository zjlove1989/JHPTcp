#include "HPTcpClient.h"
#include "../../Text.h"

// HPTcpClient й╣ож
HPTcpClient::HPTcpClient()
	: m_client(this)
	, m_remotePort(0)
	, m_async(true)
	, m_connId(0)
{
}

HPTcpClient::~HPTcpClient()
{
	Stop();
}

bool HPTcpClient::Initialize(const std::string& remoteIp, uint16_t remotePort, bool async)
{
	m_remoteIp = remoteIp;
	m_remotePort = remotePort;
	m_async = async;

	return true;
}


EnHandleResult HPTcpClient::OnConnect(ITcpClient* pSender, CONNID dwConnID)
{
	if (m_onConnect) {
		m_onConnect(dwConnID);
	}
	return HR_OK;
}

EnHandleResult HPTcpClient::OnReceive(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
{
	if (m_onReceive) {
		m_onReceive(dwConnID, pData, iLength);
	}
	return HR_OK;
}

EnHandleResult HPTcpClient::OnClose(ITcpClient* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
{
	if (m_onClose) {
		m_onClose(dwConnID);
	}
	return HR_OK;
}

bool HPTcpClient::Start()
{
	if (!m_client) {
		return false;
	}


	if (!m_client->Start(Text::text_to_wstr(m_remoteIp).c_str(), m_remotePort, m_async)) {
		return false;
	}

	m_connId = m_client->GetConnectionID();
	return true;
}

bool HPTcpClient::Stop()
{
	if (m_client && m_client->Stop()) {
		return true;
	}
	return false;
}

bool HPTcpClient::Send(const BYTE* data, int length)
{
	if (!m_client || !IsConnected()) {
		return false;
	}

	return m_client->Send(data, length);
}

bool HPTcpClient::SendString(const std::string& str)
{
	return Send(reinterpret_cast<const BYTE*>(str.c_str()), static_cast<int>(str.length()));
}

bool HPTcpClient::IsConnected() const
{
	return m_client && m_client->IsConnected();
}