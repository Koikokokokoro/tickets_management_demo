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
    DDX_Control(pDX, IDC_RESULT_LIST, m_resultListCtrl); // 绑定列表控件
}

BEGIN_MESSAGE_MAP(CResultDlg, CDialogEx)
    ON_BN_CLICKED(IDOK, &CResultDlg::OnBnClickedOk)
END_MESSAGE_MAP()

// 初始化对话框时加载搜索结果到列表控件
BOOL CResultDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 初始化列表控件的列
    m_resultListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    m_resultListCtrl.InsertColumn(0, _T("车次"), LVCFMT_LEFT, 100);
    m_resultListCtrl.InsertColumn(1, _T("发车时间"), LVCFMT_LEFT, 100);
    m_resultListCtrl.InsertColumn(2, _T("起点站"), LVCFMT_LEFT, 100);
    m_resultListCtrl.InsertColumn(3, _T("终点站"), LVCFMT_LEFT, 100);
    m_resultListCtrl.InsertColumn(4, _T("行车时间"), LVCFMT_LEFT, 100);
    m_resultListCtrl.InsertColumn(5, _T("票价"), LVCFMT_LEFT, 100);
    m_resultListCtrl.InsertColumn(6, _T("额定载量"), LVCFMT_LEFT, 100);
    m_resultListCtrl.InsertColumn(7, _T("已售票数"), LVCFMT_LEFT, 100);

    CString cstrdepartureTime;

    // 插入搜索结果到列表控件
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

// 设置搜索结果
void CResultDlg::SetResults(const std::vector<BusTimetable>& results)
{
    m_results = results;
}


void CResultDlg::OnBnClickedOk()
{
    // TODO: 在此添加控件通知处理程序代码
    CDialogEx::OnOK();
}
