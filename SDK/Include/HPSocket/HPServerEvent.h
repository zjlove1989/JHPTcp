#include "../../JFramework.h"
#include "HPTypeDef.h"
#include <winsock2.h>

using namespace JFramework;
class HPServerPrepareListenEvent : public IEvent {
public:
	HPServerPrepareListenEvent(ITcpServer* pSender, SOCKET socket)
		: m_pSender(pSender)
		, m_socket(socket)
	{
	}
	ITcpServer* m_pSender;
	SOCKET m_socket = 0;
};
class HPServerAcceptEvent : public IEvent {
public:
	HPServerAcceptEvent(ITcpServer* pSender, CONNID dwConnID, UINT_PTR soClient)
		: m_pSender(pSender)
		, m_dwConnID(dwConnID)
		, m_soClient(soClient)
	{
	}
	ITcpServer* m_pSender;
	CONNID m_dwConnID;
	UINT_PTR m_soClient;
};

class HPServerHandShakeEvent : public IEvent {
public:
	HPServerHandShakeEvent(ITcpServer* pSender, CONNID dwConnID)
		: m_pSender(pSender)
		, m_dwConnID(dwConnID)
	{
	}
	ITcpServer* m_pSender;
	CONNID m_dwConnID;
};

class HPServerReceiveEvent : public IEvent {
public:
	HPServerReceiveEvent(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
		: m_pSender(pSender)
		, m_dwConnID(dwConnID)
		, m_pData(pData)
		, m_iLength(iLength)
	{
	}
	ITcpServer* m_pSender;
	CONNID m_dwConnID;
	const BYTE* m_pData;
	int m_iLength;
};

class HPServerCloseEvent : public IEvent {
public:
	HPServerCloseEvent(ITcpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
		: m_pSender(pSender)
		, m_dwConnID(dwConnID)
		, m_enOperation(enOperation)
		, m_iErrorCode(iErrorCode)
	{
	}
	ITcpServer* m_pSender;
	CONNID m_dwConnID;
	EnSocketOperation m_enOperation;
	int m_iErrorCode;
};

class HPServerSendEvent : public IEvent {
public:
	HPServerSendEvent(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
		: m_pSender(pSender)
		, m_dwConnID(dwConnID)
		, m_pData(pData)
		, m_iLength(iLength)
	{
	}
	ITcpServer* m_pSender;
	CONNID m_dwConnID;
	const BYTE* m_pData;
	int m_iLength;
};

class HPServerShutdownEvent : public IEvent {
public:
	HPServerShutdownEvent(ITcpServer* pSender)
		: m_pSender(pSender)
	{
	}
	ITcpServer* m_pSender;
};
