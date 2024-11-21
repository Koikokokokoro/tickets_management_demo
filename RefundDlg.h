#pragma once

#include "resource.h"
#include "Bus_TimetableDlg.h"
#include "AdminDlg.h"
#include "BuyDlg.h"

class CRefundDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CRefundDlg)

public:
    CRefundDlg(CWnd* pParent = nullptr);   // 标准构造函数
    virtual ~CRefundDlg();

    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_REFUND_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()

public:
    CString m_RefundBusNumber;  // 退票车次号
    int m_RefundTicketCount;    // 退票数量
    afx_msg void OnBnClickedOk();  // 点击确认按钮后处理退票逻辑
};
