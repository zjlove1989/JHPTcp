#pragma once
#include "../../JFramework.h"
#include "SocketInterface.h"
#include "HPSocket.h"


using namespace JFramework;
class TcpServerSystem : public AbstractSystem, public CTcpServerListener {
public:
    TcpServerSystem();
    virtual ~TcpServerSystem();
protected:
    void OnInit() override;

    void OnDeinit() override;

    void OnEvent(std::shared_ptr<IEvent> event) override;

public:
    // ����������
    bool Start(const wchar_t* bindAddress, uint16_t port);

    // ֹͣ������
    bool Stop();

    // ��������
    bool Send(HP_CONNID connId, const BYTE* data, int length);

    // �Ͽ�ָ������
    bool Disconnect(HP_CONNID connId);

    // ��ȡ��ǰ������
    uint32_t GetConnectionCount() const;

    EnHandleResult OnPrepareListen(ITcpServer* pSender, SOCKET soListen) override;

    EnHandleResult OnAccept(ITcpServer* pSender, CONNID dwConnID, UINT_PTR soClient) override;

    EnHandleResult OnHandShake(ITcpServer* pSender, CONNID dwConnID) override;

    EnHandleResult OnSend(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override;

    EnHandleResult OnShutdown(ITcpServer* pSender) override;

    EnHandleResult OnReceive(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override;

    EnHandleResult OnClose(ITcpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode) override;

public:
    // HPSocket������ָ��
    CTcpPackServerPtr mServer;
};
