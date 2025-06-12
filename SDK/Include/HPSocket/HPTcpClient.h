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
	// 定义回调函数类型
	using OnConnectCallback = std::function<void(CONNID dwConnId)>;
	using OnReceiveCallback = std::function<void(CONNID dwConnId, const BYTE* pData, int iLength)>;
	using OnCloseCallback = std::function<void(CONNID dwConnId)>;

	// 构造函数
	HPTcpClient();

	// 析构函数
	virtual ~HPTcpClient();

	// 禁止拷贝和赋值
	HPTcpClient(const HPTcpClient&) = delete;
	HPTcpClient& operator=(const HPTcpClient&) = delete;

	/**
	 * @brief 初始化客户端
	 * @param remoteIp 服务器IP地址
	 * @param remotePort 服务器端口
	 * @param async 是否异步连接，默认为true
	 * @return 是否初始化成功
	 */
	bool Initialize(const std::string& remoteIp, uint16_t remotePort, bool async = true);

	/**
	 * @brief 启动客户端
	 * @return 是否启动成功
	 */
	bool Start();

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

	/**
	 * @brief 是否已连接
	 * @return 连接状态
	 */
	bool IsConnected() const;

	// 设置回调函数
	void SetOnConnectCallback(OnConnectCallback cb) { m_onConnect = cb; }
	void SetOnReceiveCallback(OnReceiveCallback cb) { m_onReceive = cb; }
	void SetOnCloseCallback(OnCloseCallback cb) { m_onClose = cb; }

private:
	virtual EnHandleResult OnReceive(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength);
	virtual EnHandleResult OnClose(ITcpClient* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode);
	virtual EnHandleResult OnConnect(ITcpClient* pSender, CONNID dwConnID);

private:
	CTcpPackClientPtr m_client;          // HPSocket TCP客户端对象
    std::string m_remoteIp; // 远程服务器IP
	uint16_t m_remotePort;                         // 远程服务器端口
	bool m_async;                                  // 是否异步连接
	CONNID m_connId;                               // 连接ID

	// 回调函数
	OnConnectCallback m_onConnect;
	OnReceiveCallback m_onReceive;
	OnCloseCallback m_onClose;
};

#endif // HP_TCP_CLIENT_H