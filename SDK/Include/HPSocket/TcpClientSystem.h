#ifndef HP_TCP_CLIENT_H
#define HP_TCP_CLIENT_H

#include "../../JFramework.h"
#include "SocketInterface.h"
#include "HPSocket.h"

using namespace JFramework;
class TcpClientSystem : public AbstractSystem, public CTcpClientListener
{
public:
	// ���캯��
	TcpClientSystem();

	// ��������
	virtual ~TcpClientSystem();

	/**
	 * @brief �����ͻ���
	 * @return �Ƿ������ɹ�
	 */
	bool Start(const wchar_t* bindAddress, uint16_t port);

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

	EnHandleResult OnPrepareConnect(ITcpClient* pSender, CONNID dwConnID, SOCKET socket) override;


	EnHandleResult OnConnect(ITcpClient* pSender, CONNID dwConnID) override;


	EnHandleResult OnHandShake(ITcpClient* pSender, CONNID dwConnID) override;

	EnHandleResult OnReceive(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override;

	EnHandleResult OnSend(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override;


	EnHandleResult OnClose(ITcpClient* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode) override;

private:
	CTcpPackClientPtr m_client;          // HPSocket TCP�ͻ��˶���
protected:
	void OnInit() override;

	void OnDeinit() override;

	void OnEvent(std::shared_ptr<IEvent> event) override;
};

#endif // HP_TCP_CLIENT_H