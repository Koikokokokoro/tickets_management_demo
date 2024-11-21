#include "pch.h"
#include "DeleteDlg.h"
#include "afxdialogex.h"
#include "CheckDlg.h"
#include "SharedFunc.h"

IMPLEMENT_DYNAMIC(CDeleteDlg, CDialogEx)

CDeleteDlg::CDeleteDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DELETE_DIALOG, pParent)
{
}

CDeleteDlg::~CDeleteDlg()
{
}

void CDeleteDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_DELETE_BUS_EDIT, m_deletebus);
    // �󶨶Ի���ؼ��ͱ���
}

BEGIN_MESSAGE_MAP(CDeleteDlg, CDialogEx)
    ON_BN_CLICKED(IDC_CHECK_BUTTON, &CDeleteDlg::OnBnClickedCheckButton)
END_MESSAGE_MAP()

bool CDeleteDlg::CheckExistence()
{
    UpdateData(TRUE);

    for (const auto& timetable : g_TimetableData)
    {
        if (timetable.busNumber == m_deletebus.GetBuffer())
        {
            return true;
        }
    }
    return false;
}

void CDeleteDlg::OnBnClickedCheckButton()
{
    if (CheckExistence())
    {
        CCheckDlg checkDlg;
        if (checkDlg.DoModal() == IDOK)
        {
            for (auto it = g_TimetableData.begin(); it != g_TimetableData.end(); ++it)
            {
                if (it->busNumber == std::string(CT2A(m_deletebus)))  // �Ƚϳ��κ�
                {
                    g_TimetableData.erase(it);  // ɾ��ƥ��ĳ���
                    AfxMessageBox(_T("�����ѳɹ�ɾ����"));
                    break;
                }
            }
        }
    }
    else
    {
        AfxMessageBox(_T("�����ڸó��Σ�"));
    }

    EndDialog(IDOK);
}