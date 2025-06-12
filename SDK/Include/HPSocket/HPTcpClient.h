#ifndef HP_TCP_CLIENT_H
#define HP_TCP_CLIENT_H

#include "SocketInterface.h"
#include "HPSocket.h"
#include <string>
#include <functional>
#include <memory>
#include <afx.h>



class HPTcpClient : public CTcpClientListener
{
public:
	// ����ص���������
	using OnConnectCallback = std::function<void(CONNID dwConnId)>;
	using OnReceiveCallback = std::function<void(CONNID dwConnId, const BYTE* pData, int iLength)>;
	using OnCloseCallback = std::function<void(CONNID dwConnId)>;

	// ���캯��
	HPTcpClient();

	// ��������
	virtual ~HPTcpClient();

	// ��ֹ�����͸�ֵ
	HPTcpClient(const HPTcpClient&) = delete;
	HPTcpClient& operator=(const HPTcpClient&) = delete;

	/**
	 * @brief ��ʼ���ͻ���
	 * @param remoteIp ������IP��ַ
	 * @param remotePort �������˿�
	 * @param async �Ƿ��첽���ӣ�Ĭ��Ϊtrue
	 * @return �Ƿ��ʼ���ɹ�
	 */
	bool Initialize(const std::string& remoteIp, uint16_t remotePort, bool async = true);

	/**
	 * @brief �����ͻ���
	 * @return �Ƿ������ɹ�
	 */
	bool Start();

	/**
	 * @brief ֹͣ�ͻ���
	 * @return �Ƿ�ֹͣ�ɹ�
	 */
	bool Stop();

	/**
	 * @brief ��������
	 * @param data Ҫ���͵�����ָ��
	 * @param length ���ݳ���
	 * @return �Ƿ��ͳɹ�
	 */
	bool Send(const BYTE* data, int length);

	/**
	 * @brief �����ַ�������
	 * @param str Ҫ���͵��ַ���
	 * @return �Ƿ��ͳɹ�
	 */
	bool SendString(const std::string& str);

	/**
	 * @brief �Ƿ�������
	 * @return ����״̬
	 */
	bool IsConnected() const;

	// ���ûص�����
	void SetOnConnectCallback(OnConnectCallback cb) { m_onConnect = cb; }
	void SetOnReceiveCallback(OnReceiveCallback cb) { m_onReceive = cb; }
	void SetOnCloseCallback(OnCloseCallback cb) { m_onClose = cb; }

private:
	virtual EnHandleResult OnReceive(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength);
	virtual EnHandleResult OnClose(ITcpClient* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode);
	virtual EnHandleResult OnConnect(ITcpClient* pSender, CONNID dwConnID);

private:
	CTcpPackClientPtr m_client;          // HPSocket TCP�ͻ��˶���
    std::string m_remoteIp; // Զ�̷�����IP
	uint16_t m_remotePort;                         // Զ�̷������˿�
	bool m_async;                                  // �Ƿ��첽����
	CONNID m_connId;                               // ����ID

	// �ص�����
	OnConnectCallback m_onConnect;
	OnReceiveCallback m_onReceive;
	OnCloseCallback m_onClose;
};

#endif // HP_TCP_CLIENT_H