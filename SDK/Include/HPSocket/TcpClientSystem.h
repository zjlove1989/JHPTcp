#ifndef HP_TCP_CLIENT_H
#define HP_TCP_CLIENT_H

#include "../../JFramework.h"
#include "SocketInterface.h"
#include "HPSocket.h"

using namespace JFramework;
class TcpClientSystem : public AbstractSystem, public CTcpClientListener
{
public:
	// 构造函数
	TcpClientSystem();

	// 析构函数
	virtual ~TcpClientSystem();

	/**
	 * @brief 启动客户端
	 * @return 是否启动成功
	 */
	bool Start(const wchar_t* bindAddress, uint16_t port);

	/**
	 * @brief 停止客户端
	 * @return 是否停止成功
	 */
	bool Stop();

	/**
	 * @brief 发送数据
	 * @param data 要发送的数据指针
	 * @param length 数据长度
	 * @return 是否发送成功
	 */
	bool Send(const BYTE* data, int length);

	/**
	 * @brief 发送字符串数据
	 * @param str 要发送的字符串
	 * @return 是否发送成功
	 */
	bool SendString(const std::string& str);

	EnHandleResult OnPrepareConnect(ITcpClient* pSender, CONNID dwConnID, SOCKET socket) override;


	EnHandleResult OnConnect(ITcpClient* pSender, CONNID dwConnID) override;


	EnHandleResult OnHandShake(ITcpClient* pSender, CONNID dwConnID) override;

	EnHandleResult OnReceive(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override;

	EnHandleResult OnSend(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override;


	EnHandleResult OnClose(ITcpClient* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode) override;

private:
	CTcpPackClientPtr m_client;          // HPSocket TCP客户端对象
protected:
	void OnInit() override;

	void OnDeinit() override;

	void OnEvent(std::shared_ptr<IEvent> event) override;
};

#endif // HP_TCP_CLIENT_H