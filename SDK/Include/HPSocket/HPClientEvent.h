#include "../../JFramework.h"
#include "HPTypeDef.h"
#include "SocketInterface.h"
#include <winsock2.h>

using namespace JFramework;
class HPClientPrepareConnectEvent : public IEvent {
public:
	HPClientPrepareConnectEvent(ITcpClient* pSender, CONNID dwConnID, SOCKET socket)
		: m_pSender(pSender)
		, m_dwConnID(dwConnID)
		, m_socket(socket)
	{
	}
	ITcpClient* m_pSender;
	CONNID m_dwConnID;
	SOCKET m_socket = 0;
};
class HPClientConnectEvent : public IEvent {
public:
	HPClientConnectEvent(ITcpClient* pSender, CONNID dwConnID)
		: m_pSender(pSender)
		, m_dwConnID(dwConnID)
	{
	}
	ITcpClient* m_pSender;
	CONNID m_dwConnID;
};

class HPClientHandShakeEvent : public IEvent {
public:
	HPClientHandShakeEvent(ITcpClient* pSender, CONNID dwConnID)
		: m_pSender(pSender)
		, m_dwConnID(dwConnID)
	{
	}
	ITcpClient* m_pSender;
	CONNID m_dwConnID;
};

class HPClientReceiveEvent : public IEvent {
public:
	HPClientReceiveEvent(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
		: m_pSender(pSender)
		, m_dwConnID(dwConnID)
		, m_pData(pData)
		, m_iLength(iLength)
	{
	}
	ITcpClient* m_pSender;
	CONNID m_dwConnID;
	const BYTE* m_pData;
	int m_iLength;
};

class HPClientSendEvent : public IEvent {
public:
	HPClientSendEvent(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
		: m_pSender(pSender)
		, m_dwConnID(dwConnID)
		, m_pData(pData)
		, m_iLength(iLength)
	{
	}
	ITcpClient* m_pSender;
	CONNID m_dwConnID;
	const BYTE* m_pData;
	int m_iLength;
};


class HPClientCloseEvent : public IEvent {
public:
	HPClientCloseEvent(ITcpClient* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
		: m_pSender(pSender)
		, m_dwConnID(dwConnID)
		, m_enOperation(enOperation)
		, m_iErrorCode(iErrorCode)
	{
	}
	ITcpClient* m_pSender;
	CONNID m_dwConnID;
	EnSocketOperation m_enOperation;
	int m_iErrorCode;
};

