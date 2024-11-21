#include "pch.h"
#include "TicketConfirmDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CTicketConfirmDlg, CDialogEx)

CTicketConfirmDlg::CTicketConfirmDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_CONFIRM_DIALOG, pParent), m_TicketCount(0)
{
}

CTicketConfirmDlg::~CTicketConfirmDlg()
{
}

void CTicketConfirmDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_CONFIRM_EDIT, m_TicketCount);  // °ó¶¨Æ±Êý±à¼­¿ò
}

BEGIN_MESSAGE_MAP(CTicketConfirmDlg, CDialogEx)
END_MESSAGE_MAP()
