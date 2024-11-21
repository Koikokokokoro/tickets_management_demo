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
    DDX_Text(pDX, IDC_SEARCH_EDIT, m_searchInput);      // 绑定编辑框
    DDX_CBIndex(pDX, IDC_COMBO, m_searchType);
    // 绑定对话框控件和变量
}

BEGIN_MESSAGE_MAP(CSearchDlg, CDialogEx)
    ON_BN_CLICKED(IDC_SEARCH_BUTTON, &CSearchDlg::OnBnClickedSearchButton)
END_MESSAGE_MAP()

void CSearchDlg::OnBnClickedSearchButton()
{
    UpdateData(TRUE);  // 获取编辑框和下拉框的数据

    vector<BusTimetable> searchResults;

    // 根据下拉框选择进行搜索
    if (m_searchType == 0) // 按车次搜索
    {
        for (const auto& timetable : g_TimetableData)
        {
            if (timetable.busNumber == m_searchInput.GetBuffer())
            {
                searchResults.push_back(timetable);
            }
        }
    }
    else if (m_searchType == 1) // 按终点站搜索
    {
        for (const auto& timetable : g_TimetableData)
        {
            if (timetable.endStation == m_searchInput.GetBuffer())
            {
                searchResults.push_back(timetable);
            }
        }
    }

    // 如果找到符合条件的车次，显示结果
    if (!searchResults.empty())
    {
        DisplaySearchResults(searchResults);
    }
    else
    {
        AfxMessageBox(_T("未找到符合条件的车次信息！"));
    }
}

// 显示搜索结果的函数
void CSearchDlg::DisplaySearchResults(const vector<BusTimetable>& results)
{
    // 复制结果到局部变量以进行排序
    vector<BusTimetable> sortedResults = results;

    // 按发车时间升序排序
    sort(sortedResults.begin(), sortedResults.end(), [](const BusTimetable& a, const BusTimetable& b) {
        return a.departureTime < b.departureTime;  // 直接按字符串比较，格式"HH:MM"的字符串排序与时间排序一致
        });

    // 创建结果对话框并传入搜索结果
    CResultDlg resultDlg;
    resultDlg.SetResults(results);
    resultDlg.DoModal();
}