#pragma once

#include "SharedFunc.h"

class CBuyDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CBuyDlg)

public:
    CBuyDlg(CWnd* pParent = nullptr);   // 标准构造函数
    virtual ~CBuyDlg();

    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_BUY_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()

public:
    CString m_BusNumber;  // 存储用户输入的车次号
    int m_TicketCount;    // 存储用户输入的购票数量
    afx_msg void OnBnClickedOk();  // 点击确认按钮后处理购票逻辑

    //int CheckTime(CTime ntime);
};
