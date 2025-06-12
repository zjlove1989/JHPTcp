// JHPTcpServerDlg.h: 头文件
//

#pragma once
#include "../SDK/JFramework.h"
#include <afxdialogex.h>
#include "../SDK/helper.h"
using namespace JFramework;
// CJHPTcpServerDlg 对话框
class CJHPTcpServerDlg : public CDialogEx, public AbstractController {
    // 构造
public:
    CJHPTcpServerDlg(CWnd* pParent = nullptr); // 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_JHPTCPSERVER_DIALOG };
#endif


 std::weak_ptr<IArchitecture> GetArchitecture() const override;

protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV 支持


 void OnEvent(std::shared_ptr<IEvent> event) override;

    // 实现
protected:
    HICON m_hIcon;

    // 生成的消息映射函数
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonStart();
    afx_msg void OnClose();
    CButton m_Start;
    CListBox m_ListBoxLog;
protected:
    EnAppState m_enState;
    void SetAppState(EnAppState state);
public:
    CButton m_Stop;
    afx_msg void OnBnClickedButtonStop();
};
