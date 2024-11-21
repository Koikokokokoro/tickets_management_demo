#include "pch.h"
#include "ResultDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CResultDlg, CDialogEx)

CResultDlg::CResultDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_RESULT_DIALOG, pParent)
{
}

CResultDlg::~CResultDlg()
{
}

void CResultDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_RESULT_LIST, m_resultListCtrl); // ���б�ؼ�
}

BEGIN_MESSAGE_MAP(CResultDlg, CDialogEx)
    ON_BN_CLICKED(IDOK, &CResultDlg::OnBnClickedOk)
END_MESSAGE_MAP()

// ��ʼ���Ի���ʱ��������������б�ؼ�
BOOL CResultDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // ��ʼ���б�ؼ�����
    m_resultListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    m_resultListCtrl.InsertColumn(0, _T("����"), LVCFMT_LEFT, 100);
    m_resultListCtrl.InsertColumn(1, _T("����ʱ��"), LVCFMT_LEFT, 100);
    m_resultListCtrl.InsertColumn(2, _T("���վ"), LVCFMT_LEFT, 100);
    m_resultListCtrl.InsertColumn(3, _T("�յ�վ"), LVCFMT_LEFT, 100);
    m_resultListCtrl.InsertColumn(4, _T("�г�ʱ��"), LVCFMT_LEFT, 100);
    m_resultListCtrl.InsertColumn(5, _T("Ʊ��"), LVCFMT_LEFT, 100);
    m_resultListCtrl.InsertColumn(6, _T("�����"), LVCFMT_LEFT, 100);
    m_resultListCtrl.InsertColumn(7, _T("����Ʊ��"), LVCFMT_LEFT, 100);

    CString cstrdepartureTime;

    // ��������������б�ؼ�
    for (size_t i = 0; i < m_results.size(); ++i)
    {
        const auto& timetable = m_results[i];

        cstrdepartureTime.Format("%d:%d", timetable.departureTime.GetHour(), timetable.departureTime.GetMinute());

        m_resultListCtrl.InsertItem(i, timetable.busNumber.c_str());
        m_resultListCtrl.SetItemText(i, 1, cstrdepartureTime);
        m_resultListCtrl.SetItemText(i, 2, timetable.startStation.c_str());
        m_resultListCtrl.SetItemText(i, 3, timetable.endStation.c_str());
        m_resultListCtrl.SetItemText(i, 4, timetable.travelTime.c_str());
        m_resultListCtrl.SetItemText(i, 5, to_string(timetable.price).c_str());
        m_resultListCtrl.SetItemText(i, 6, std::to_string(timetable.capacity).c_str());
        m_resultListCtrl.SetItemText(i, 7, std::to_string(timetable.soldTickets).c_str());
    }

    return TRUE;
}

// �����������
void CResultDlg::SetResults(const std::vector<BusTimetable>& results)
{
    m_results = results;
}


void CResultDlg::OnBnClickedOk()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CDialogEx::OnOK();
}
