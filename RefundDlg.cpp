#include "pch.h"
#include "RefundDlg.h"
#include "afxdialogex.h"
#include <ctime>  // ����ʱ�����

IMPLEMENT_DYNAMIC(CRefundDlg, CDialogEx)

CRefundDlg::CRefundDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_REFUND_DIALOG, pParent)
    , m_RefundBusNumber(_T(""))
    , m_RefundTicketCount(0)
{
}

CRefundDlg::~CRefundDlg()
{
}

void CRefundDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_REFUND_BUS_EDIT, m_RefundBusNumber);  // �󶨳��κű༭��
    DDX_Text(pDX, IDC_REFUND_TICKET_EDIT, m_RefundTicketCount);  // ����Ʊ���༭��
}

BEGIN_MESSAGE_MAP(CRefundDlg, CDialogEx)
    ON_BN_CLICKED(IDOK, &CRefundDlg::OnBnClickedOk)  // ��ȷ�ϰ�ť�¼�
END_MESSAGE_MAP()

// ���ȷ�ϰ�ť������Ʊ�߼�
void CRefundDlg::OnBnClickedOk()
{
    UpdateData(TRUE);

    // ��鳵���Ƿ����
    bool found = false;
    BusTimetable* selectedTimetable = nullptr;

    for (auto& timetable : g_TimetableData)
    {
        if (timetable.busNumber == m_RefundBusNumber.GetBuffer())
        {
            selectedTimetable = &timetable;
            found = true;
            break;
        }
    }

    if (!found)
    {
        AfxMessageBox(_T("���β����ڣ�"));
        return;
    }

    // �����Ʊ���Ƿ������ѹ�Ʊ��
    if (m_RefundTicketCount > selectedTimetable->soldTickets)
    {
        AfxMessageBox(_T("��Ʊ�������ѹ�Ʊ����"));
        return;
    }

    // �����Ʊʱ���Ƿ�෢��ʱ�����10����
    int checkresult = SharedFunc::CheckTime(selectedTimetable->departureTime);

    if (checkresult == 2)
    {
        AfxMessageBox(_T("���뷢��ʱ�䲻��10���ӣ��޷���Ʊ��"));
        return;
    }
    else if (checkresult == 0)
    {
        AfxMessageBox(_T("�����ѳ������޷���Ʊ��"));
        return;
    }

    // ��������Ʊ������ʾ��Ʊ�ɹ�
    selectedTimetable->soldTickets -= m_RefundTicketCount;
    CString successMessage;
    successMessage.Format(_T("��Ʊ�ɹ���\n���Σ�%s\n��Ʊ����%d"), m_RefundBusNumber, m_RefundTicketCount);
    AfxMessageBox(successMessage);

    CDialogEx::OnOK();  // �رնԻ���
}
