// JHPTcpServerDlg.cpp: 实现文件
//

#include "JHPTcpServerDlg.h"
#include "JHPTcpServer.h"
#include "JHPTcpServerArchitecture.h"
#include "afxdialogex.h"
#include "framework.h"
#include "pch.h"
#include "../SDK/Text.h"
#include "../SDK/Include/HPSocket/HPServerEvent.h"
#include "../SDK/helper.h"
#include "../SDK/Include/HPSocket/TcpServerSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const LPCTSTR ADDRESS = _T("0.0.0.0");
const USHORT PORT = 5555;

// CJHPTcpServerDlg 对话框

CJHPTcpServerDlg::CJHPTcpServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_JHPTCPSERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

std::weak_ptr<JFramework::IArchitecture> CJHPTcpServerDlg::GetArchitecture() const
{
	return JHPTcpServerArchitecture::Instance();
}

void CJHPTcpServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_START, m_Start);
	DDX_Control(pDX, IDC_LIST_LOG, m_Info);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_Stop);
}

BEGIN_MESSAGE_MAP(CJHPTcpServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CJHPTcpServerDlg::OnBnClickedButtonStart)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CJHPTcpServerDlg::OnBnClickedButtonStop)
END_MESSAGE_MAP()

// CJHPTcpServerDlg 消息处理程序

BOOL CJHPTcpServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr) {
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty()) {
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE); // 设置大图标
	SetIcon(m_hIcon, FALSE); // 设置小图标

	// TODO: 在此添加额外的初始化代码

	this->RegisterEvent<HPServerPrepareListenEvent>(this);
	this->RegisterEvent<HPServerAcceptEvent>(this);
	this->RegisterEvent<HPServerHandShakeEvent>(this);
	this->RegisterEvent<HPServerReceiveEvent>(this);
	this->RegisterEvent<HPServerCloseEvent>(this);
	this->RegisterEvent<HPServerSendEvent>(this);
	this->RegisterEvent<HPServerShutdownEvent>(this);

	::SetMainWnd(this);
	::SetInfoList(&m_Info);
	SetAppState(EnAppState::HP_STOPPED);

	return TRUE; // 除非将焦点设置到控件，否则返回 TRUE
}

void CJHPTcpServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CJHPTcpServerDlg::OnPaint()
{
	if (IsIconic()) {
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
	else {
		CDialogEx::OnPaint();
	}
}

// 当用户拖动最小化窗口时系统调用此函数取得光标
// 显示。
HCURSOR CJHPTcpServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CJHPTcpServerDlg::OnBnClickedButtonStart()
{
	SetAppState(EnAppState::HP_STARTING);


	::LogServerStart(ADDRESS, PORT);

	auto tcpServerSystem = this->GetSystem<TcpServerSystem>();
	if (!tcpServerSystem->Start(ADDRESS, PORT))
	{
		::LogServerStartFail(tcpServerSystem->m_server->GetLastError(),
			tcpServerSystem->m_server->GetLastErrorDesc());
		SetAppState(EnAppState::HP_STOPPED);
	}
	else
	{
		SetAppState(EnAppState::HP_STARTED);
	}
}

void CJHPTcpServerDlg::OnBnClickedButtonStop()
{
	SetAppState(EnAppState::HP_STOPPING);
	auto tcpServerSystem = this->GetSystem<TcpServerSystem>();

	if (tcpServerSystem->Stop())
	{
		::LogServerStop();
		SetAppState(EnAppState::HP_STOPPED);
	}
	else
	{
		ASSERT(FALSE);
	}
}

void CJHPTcpServerDlg::OnClose()
{
	this->UnRegisterEvent<HPServerPrepareListenEvent>(this);
	this->UnRegisterEvent<HPServerAcceptEvent>(this);
	this->UnRegisterEvent<HPServerHandShakeEvent>(this);
	this->UnRegisterEvent<HPServerReceiveEvent>(this);
	this->UnRegisterEvent<HPServerCloseEvent>(this);
	this->UnRegisterEvent<HPServerSendEvent>(this);
	this->UnRegisterEvent<HPServerShutdownEvent>(this);

	this->GetSystem<TcpServerSystem>()->Stop();

	__super::OnClose();
}

void CJHPTcpServerDlg::SetAppState(EnAppState state)
{
	m_enState = state;
	if (this->GetSafeHwnd() == nullptr)
		return;
	m_Start.EnableWindow(m_enState == EnAppState::HP_STOPPED);
	m_Stop.EnableWindow(m_enState == EnAppState::HP_STARTED);
}

void CJHPTcpServerDlg::OnEvent(std::shared_ptr<IEvent> event)
{
	if (auto e = std::dynamic_pointer_cast<HPServerPrepareListenEvent>(event))
	{
		TCHAR szAddress[100];
		int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
		USHORT usPort;

		e->m_pSender->GetListenAddress(szAddress, iAddressLen, usPort);
		::PostOnPrepareListen(szAddress, usPort);
	}
	else if (auto e = std::dynamic_pointer_cast<HPServerAcceptEvent>(event))
	{
		BOOL bPass = TRUE;
		TCHAR szAddress[100];
		int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
		USHORT usPort;
		e->m_pSender->GetRemoteAddress(e->m_dwConnID, szAddress, iAddressLen, usPort);
		::PostOnAccept(e->m_dwConnID, szAddress, usPort, bPass);
	}
	else if (auto e = std::dynamic_pointer_cast<HPServerHandShakeEvent>(event))
	{
		::PostOnHandShake(e->m_dwConnID, L"");
	}
	else if (auto e = std::dynamic_pointer_cast<HPServerReceiveEvent>(event))
	{
		::PostOnReceive(e->m_dwConnID, e->m_pData, e->m_iLength);
		this->GetSystem<TcpServerSystem>()->Send(e->m_dwConnID, e->m_pData, e->m_iLength);
	}
	else if (auto e = std::dynamic_pointer_cast<HPServerCloseEvent>(event))
	{
		e->m_iErrorCode == SE_OK ? 
			::PostOnClose(e->m_dwConnID) : 
			::PostOnError(e->m_dwConnID, e->m_enOperation, e->m_iErrorCode);
	}
	else if (auto e = std::dynamic_pointer_cast<HPServerSendEvent>(event))
	{
		::PostOnSend(e->m_dwConnID, e->m_pData, e->m_iLength);
	}
	else if (auto e = std::dynamic_pointer_cast<HPServerShutdownEvent>(event))
	{
		::PostOnShutdown();
	}
}