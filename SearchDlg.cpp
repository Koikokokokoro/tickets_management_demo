#include "pch.h"
#include "SearchDlg.h"
#include "afxdialogex.h"
#include "SharedFunc.h"

IMPLEMENT_DYNAMIC(CSearchDlg, CDialogEx)

CSearchDlg::CSearchDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_SEARCH_DIALOG, pParent)
{
}

CSearchDlg::~CSearchDlg()
{
}

void CSearchDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_SEARCH_EDIT, m_searchInput);      // �󶨱༭��
    DDX_CBIndex(pDX, IDC_COMBO, m_searchType);
    // �󶨶Ի���ؼ��ͱ���
}

BEGIN_MESSAGE_MAP(CSearchDlg, CDialogEx)
    ON_BN_CLICKED(IDC_SEARCH_BUTTON, &CSearchDlg::OnBnClickedSearchButton)
END_MESSAGE_MAP()

void CSearchDlg::OnBnClickedSearchButton()
{
    UpdateData(TRUE);  // ��ȡ�༭��������������

    vector<BusTimetable> searchResults;

    // ����������ѡ���������
    if (m_searchType == 0) // ����������
    {
        for (const auto& timetable : g_TimetableData)
        {
            if (timetable.busNumber == m_searchInput.GetBuffer())
            {
                searchResults.push_back(timetable);
            }
        }
    }
    else if (m_searchType == 1) // ���յ�վ����
    {
        for (const auto& timetable : g_TimetableData)
        {
            if (timetable.endStation == m_searchInput.GetBuffer())
            {
                searchResults.push_back(timetable);
            }
        }
    }

    // ����ҵ����������ĳ��Σ���ʾ���
    if (!searchResults.empty())
    {
        DisplaySearchResults(searchResults);
    }
    else
    {
        AfxMessageBox(_T("δ�ҵ����������ĳ�����Ϣ��"));
    }
}

// ��ʾ��������ĺ���
void CSearchDlg::DisplaySearchResults(const vector<BusTimetable>& results)
{
    // ���ƽ�����ֲ������Խ�������
    vector<BusTimetable> sortedResults = results;

    // ������ʱ����������
    sort(sortedResults.begin(), sortedResults.end(), [](const BusTimetable& a, const BusTimetable& b) {
        return a.departureTime < b.departureTime;  // ֱ�Ӱ��ַ����Ƚϣ���ʽ"HH:MM"���ַ���������ʱ������һ��
        });

    // ��������Ի��򲢴����������
    CResultDlg resultDlg;
    resultDlg.SetResults(results);
    resultDlg.DoModal();
}