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
    DDX_Text(pDX, IDC_BUY_BUS_EDIT, m_BusNumber);  // 绑定车次号编辑框
    DDX_Text(pDX, IDC_BUY_TICKET_EDIT, m_TicketCount);  // 绑定购票数编辑框
}

BEGIN_MESSAGE_MAP(CBuyDlg, CDialogEx)
    ON_BN_CLICKED(IDOK, &CBuyDlg::OnBnClickedOk)  // 绑定确认按钮
END_MESSAGE_MAP()

// 点击确认按钮后处理购票逻辑
void CBuyDlg::OnBnClickedOk()
{
    UpdateData(TRUE);  // 获取输入框中的数据

    // 检查车次是否存在
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
        AfxMessageBox(_T("车次不存在！"));
        return;
    }

    // 检查购票时间与发车时间的差距是否大于10分钟
    int checkresult = SharedFunc::CheckTime(selectedTimetable->departureTime);

    if (checkresult == 2)
    {
        AfxMessageBox(_T("距离发车时间不足10分钟，无法购票！"));
        return;
    }
    else if (checkresult == 0)
    {
        AfxMessageBox(_T("该车次已发车，无法购票！"));
        return;
    }

    // 检查空余票数是否足够
    int availableTickets = selectedTimetable->capacity - selectedTimetable->soldTickets;
    if (availableTickets < m_TicketCount)
    {
        AfxMessageBox(_T("余票不足！"));
        return;
    }

    // 更新已售票数并提示购票成功
    selectedTimetable->soldTickets += m_TicketCount;
    CString successMessage;
    successMessage.Format(_T("购票成功！\n车次：%s\n购票数：%d"), m_BusNumber, m_TicketCount);
    AfxMessageBox(successMessage);

    CDialogEx::OnOK();  // 关闭对话框
}