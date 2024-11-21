#pragma once

#include "resource.h"
#include "SharedFunc.h"

class CTicketConfirmDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CTicketConfirmDlg)

public:
    CTicketConfirmDlg(CWnd* pParent = nullptr);
    virtual ~CTicketConfirmDlg();

    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_TICKET_CONFIRM_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

    DECLARE_MESSAGE_MAP()

private:
    int m_TicketCount;  // 用户输入的票数

public:
    int GetTicketCount() const { return m_TicketCount; }  // 获取票数
};