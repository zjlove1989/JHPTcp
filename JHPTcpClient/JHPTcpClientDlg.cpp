// JHPTcpClientDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "JHPTcpClient.h"
#include "JHPTcpClientDlg.h"
#include "afxdialogex.h"
#include "JHPTcpClientArchitecture.h"
#include "../SDK/Include/HPSocket/HPClientEvent.h"
#include "../SDK/helper.h"
#include "../SDK/Include/HPSocket/TcpClientSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CJHPTcpClientDlg 对话框

const LPCTSTR ADDRESS = _T("127.0.0.1");
const USHORT PORT = 5555;

CJHPTcpClientDlg::CJHPTcpClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_JHPTCPCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

std::weak_ptr<JFramework::IArchitecture> CJHPTcpClientDlg::GetArchitecture() const
{
	return JHPTcpClientArchitecture::Instance();
}

void CJHPTcpClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INFO, m_Info);
	DDX_Control(pDX, IDC_BUTTON_START, m_Start);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_Stop);
	DDX_Control(pDX, IDC_EDIT_CONTENT, m_Content);
}

BEGIN_MESSAGE_MAP(CJHPTcpClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_START, &CJHPTcpClientDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CJHPTcpClientDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CJHPTcpClientDlg::OnBnClickedButtonSend)
END_MESSAGE_MAP()

// CJHPTcpClientDlg 消息处理程序

BOOL CJHPTcpClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	this->RegisterEvent<HPClientPrepareConnectEvent>(this);
	this->RegisterEvent<HPClientConnectEvent>(this);
	this->RegisterEvent<HPClientHandShakeEvent>(this);
	this->RegisterEvent<HPClientReceiveEvent>(this);
	this->RegisterEvent<HPClientSendEvent>(this);
	this->RegisterEvent<HPClientCloseEvent>(this);


	::SetMainWnd(this);
	::SetInfoList(&m_Info);
	SetAppState(EnAppState::HP_STOPPED);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CJHPTcpClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CJHPTcpClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CJHPTcpClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CJHPTcpClientDlg::OnClose()
{
	this->UnRegisterEvent<HPClientPrepareConnectEvent>(this);
	this->UnRegisterEvent<HPClientConnectEvent>(this);
	this->UnRegisterEvent<HPClientHandShakeEvent>(this);
	this->UnRegisterEvent<HPClientReceiveEvent>(this);
	this->UnRegisterEvent<HPClientSendEvent>(this);
	this->UnRegisterEvent<HPClientCloseEvent>(this);

	__super::OnClose();
}

void CJHPTcpClientDlg::OnEvent(std::shared_ptr<IEvent> event)
{
	if (auto e = std::dynamic_pointer_cast<HPClientPrepareConnectEvent>(event))
	{

	}
	else if (auto e = std::dynamic_pointer_cast<HPClientConnectEvent>(event))
	{
		TCHAR szAddress[100];
		int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
		USHORT usPort;

		e->m_pSender->GetLocalAddress(szAddress, iAddressLen, usPort);

		::PostOnConnect(e->m_dwConnID, szAddress, usPort);
		SetAppState(EnAppState::HP_STARTED);
	}
	else if (auto e = std::dynamic_pointer_cast<HPClientHandShakeEvent>(event))
	{
		::PostOnHandShake(e->m_dwConnID, L"");
	}
	else if (auto e = std::dynamic_pointer_cast<HPClientReceiveEvent>(event))
	{
		::PostOnReceive(e->m_dwConnID, e->m_pData, e->m_iLength);
	}
	else if (auto e = std::dynamic_pointer_cast<HPClientSendEvent>(event))
	{
		::PostOnSend(e->m_dwConnID, e->m_pData, e->m_iLength);
	}
	else if (auto e = std::dynamic_pointer_cast<HPClientCloseEvent>(event))
	{
		e->m_iErrorCode == SE_OK ? 
			::PostOnClose(e->m_dwConnID) :
			::PostOnError(e->m_dwConnID, e->m_enOperation, e->m_iErrorCode);

		SetAppState(EnAppState::HP_STOPPED);
	}
}
void CJHPTcpClientDlg::OnBnClickedButtonStart()
{
	SetAppState(EnAppState::HP_STARTING);


	::LogClientStarting(ADDRESS, PORT);

	auto tcpClientSystem = this->GetSystem<TcpClientSystem>();
	if (!tcpClientSystem->Start(ADDRESS, PORT))
	{
		::LogClientStartFail(tcpClientSystem->m_client->GetLastError(),
			tcpClientSystem->m_client->GetLastErrorDesc());
		SetAppState(EnAppState::HP_STOPPED);
	}
	else
	{
		SetAppState(EnAppState::HP_STARTED);
	}
}

void CJHPTcpClientDlg::OnBnClickedButtonStop()
{
	auto tcpClientSystem = this->GetSystem<TcpClientSystem>();
	if (tcpClientSystem->Stop())
	{
		::LogClientStopping(tcpClientSystem->m_client->GetConnectionID());
		SetAppState(EnAppState::HP_STOPPED);
	}
	else
	{
		ASSERT(FALSE);
	}
}

void CJHPTcpClientDlg::SetAppState(EnAppState state)
{
	m_enState = state;
	if (this->GetSafeHwnd() == nullptr)
		return;
	m_Start.EnableWindow(m_enState == EnAppState::HP_STOPPED);
	m_Stop.EnableWindow(m_enState == EnAppState::HP_STARTED);
}

void CJHPTcpClientDlg::OnBnClickedButtonSend()
{
	CString strContent;
	m_Content.GetWindowText(strContent);

	auto tcpClientSystem = this->GetSystem<TcpClientSystem>();
	if (tcpClientSystem->Send((LPBYTE)strContent.GetBuffer(), strContent.GetLength()))
		::LogSend(tcpClientSystem->m_client->GetConnectionID(), strContent);
	else
		::LogSendFail(tcpClientSystem->m_client->GetConnectionID(), ::GetLastError(), ::HP_GetSocketErrorDesc(SE_DATA_SEND));
}
