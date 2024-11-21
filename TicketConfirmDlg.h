#pragma once

#include "resource.h"
#include "SharedFunc.h"

class CTicketConfirmDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CTicketConfirmDlg)

public:
    CTicketConfirmDlg(CWnd* pParent = nullptr);
    virtual ~CTicketConfirmDlg();

    // �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_TICKET_CONFIRM_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

    DECLARE_MESSAGE_MAP()

private:
    int m_TicketCount;  // �û������Ʊ��

public:
    int GetTicketCount() const { return m_TicketCount; }  // ��ȡƱ��
};