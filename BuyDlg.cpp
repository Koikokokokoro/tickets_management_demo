#include "pch.h"
#include "BuyDlg.h"

IMPLEMENT_DYNAMIC(CBuyDlg, CDialogEx)

CBuyDlg::CBuyDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_BUY_DIALOG, pParent)
    , m_BusNumber(_T(""))
    , m_TicketCount(0)
{
}

CBuyDlg::~CBuyDlg()
{
}

void CBuyDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_BUY_BUS_EDIT, m_BusNumber);  // �󶨳��κű༭��
    DDX_Text(pDX, IDC_BUY_TICKET_EDIT, m_TicketCount);  // �󶨹�Ʊ���༭��
}

BEGIN_MESSAGE_MAP(CBuyDlg, CDialogEx)
    ON_BN_CLICKED(IDOK, &CBuyDlg::OnBnClickedOk)  // ��ȷ�ϰ�ť
END_MESSAGE_MAP()

// ���ȷ�ϰ�ť����Ʊ�߼�
void CBuyDlg::OnBnClickedOk()
{
    UpdateData(TRUE);  // ��ȡ������е�����

    // ��鳵���Ƿ����
    bool found = false;
    BusTimetable* selectedTimetable = nullptr;

    for (auto& timetable : g_TimetableData)
    {
        if (timetable.busNumber == m_BusNumber.GetBuffer())
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

    // ��鹺Ʊʱ���뷢��ʱ��Ĳ���Ƿ����10����
    int checkresult = SharedFunc::CheckTime(selectedTimetable->departureTime);

    if (checkresult == 2)
    {
        AfxMessageBox(_T("���뷢��ʱ�䲻��10���ӣ��޷���Ʊ��"));
        return;
    }
    else if (checkresult == 0)
    {
        AfxMessageBox(_T("�ó����ѷ������޷���Ʊ��"));
        return;
    }

    // ������Ʊ���Ƿ��㹻
    int availableTickets = selectedTimetable->capacity - selectedTimetable->soldTickets;
    if (availableTickets < m_TicketCount)
    {
        AfxMessageBox(_T("��Ʊ���㣡"));
        return;
    }

    // ��������Ʊ������ʾ��Ʊ�ɹ�
    selectedTimetable->soldTickets += m_TicketCount;
    CString successMessage;
    successMessage.Format(_T("��Ʊ�ɹ���\n���Σ�%s\n��Ʊ����%d"), m_BusNumber, m_TicketCount);
    AfxMessageBox(successMessage);

    CDialogEx::OnOK();  // �رնԻ���
}