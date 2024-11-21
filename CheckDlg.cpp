#include "pch.h"
#include "DeleteDlg.h"
#include "afxdialogex.h"
#include "CheckDlg.h"

IMPLEMENT_DYNAMIC(CCheckDlg, CDialogEx)

CCheckDlg::CCheckDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_CHECK_DIALOG, pParent){}

CCheckDlg::~CCheckDlg()
{
}

void CCheckDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCheckDlg, CDialogEx)
END_MESSAGE_MAP()