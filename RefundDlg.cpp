#include "pch.h"
#include "RefundDlg.h"
#include "afxdialogex.h"
#include <ctime>  // 用于时间计算

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
    DDX_Text(pDX, IDC_REFUND_BUS_EDIT, m_RefundBusNumber);  // 绑定车次号编辑框
    DDX_Text(pDX, IDC_REFUND_TICKET_EDIT, m_RefundTicketCount);  // 绑定退票数编辑框
}

BEGIN_MESSAGE_MAP(CRefundDlg, CDialogEx)
    ON_BN_CLICKED(IDOK, &CRefundDlg::OnBnClickedOk)  // 绑定确认按钮事件
END_MESSAGE_MAP()

// 点击确认按钮后处理退票逻辑
void CRefundDlg::OnBnClickedOk()
{
    UpdateData(TRUE);

    // 检查车次是否存在
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
        AfxMessageBox(_T("车次不存在！"));
        return;
    }

    // 检查退票数是否少于已购票数
    if (m_RefundTicketCount > selectedTimetable->soldTickets)
    {
        AfxMessageBox(_T("退票数超过已购票数！"));
        return;
    }

    // 检查退票时间是否距发车时间大于10分钟
    int checkresult = SharedFunc::CheckTime(selectedTimetable->departureTime);

    if (checkresult == 2)
    {
        AfxMessageBox(_T("距离发车时间不足10分钟，无法退票！"));
        return;
    }
    else if (checkresult == 0)
    {
        AfxMessageBox(_T("车次已出发，无法退票！"));
        return;
    }

    // 更新已售票数并提示退票成功
    selectedTimetable->soldTickets -= m_RefundTicketCount;
    CString successMessage;
    successMessage.Format(_T("退票成功！\n车次：%s\n退票数：%d"), m_RefundBusNumber, m_RefundTicketCount);
    AfxMessageBox(successMessage);

    CDialogEx::OnOK();  // 关闭对话框
}
