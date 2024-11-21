// AddDlg.cpp
#include "pch.h"
#include "AddDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CAddDlg, CDialogEx)

bool IsNumeric(CString str);

CAddDlg::CAddDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_ADD_DIALOG, pParent)
{
}

CAddDlg::~CAddDlg()
{
}

BOOL CAddDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    m_DepartureTimePickerCtrl.SetFormat(_T("HH:mm"));  // 隐藏秒数
    //m_DurationTimePickerCtrl.SetFormat(_T("HH:mm"));

    return TRUE;
}

void CAddDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_BUS_NUMBER_EDIT, m_busNumber);
    DDX_Text(pDX, IDC_START_STATION_EDIT, m_startStation);
    DDX_Text(pDX, IDC_END_STATION_EDIT, m_endStation);
    DDX_Text(pDX, IDC_PRICE_EDIT, m_price);
    DDX_Text(pDX, IDC_CAPACITY_EDIT, m_capacity);
    DDX_Text(pDX, IDC_DURATION_EDIT, m_travelTime);

    DDX_Control(pDX, IDC_DEPARTURE_TIMEPICKER, m_DepartureTimePickerCtrl);
    //DDX_Control(pDX, IDC_DURATION_TIMEPICKER, m_DurationTimePickerCtrl);
}

BEGIN_MESSAGE_MAP(CAddDlg, CDialogEx)
END_MESSAGE_MAP()

// 验证输入字段的有效性
bool CAddDlg::ValidateInputs()
{
    UpdateData(true);

    // 检查每个输入字段是否为空
    if (m_busNumber.IsEmpty() || m_startStation.IsEmpty() || m_endStation.IsEmpty() ||
        m_travelTime.IsEmpty() || m_price.IsEmpty() || m_capacity.IsEmpty())
    {
        AfxMessageBox(_T("请完整填写所有字段"));
        return false;
    }

    // 检查车次号是否已存在
    for (const auto& timetable : g_TimetableData)
    {
        if (timetable.busNumber == m_busNumber.GetBuffer())
        {
            AfxMessageBox(_T("车次号已存在，请输入一个新的车次号！"));
            return false;
        }
    }

    if (!IsNumeric(m_travelTime) || !IsNumeric(m_price) || !IsNumeric(m_capacity))
    {
        AfxMessageBox(_T("行车时间、价格和容量必须为有效数字"));
        return false;
    }

    // 检查价格和容量是否是正数
    float price = _tstof(m_price);
    int capacity = _tstoi(m_capacity);
    if (price < 0 || capacity <= 0)
    {
        AfxMessageBox(_T("价格必须为正数，且容量必须为正整数"));
        return false;
    }

    return true;
}

// 重写OnOK函数，在点击OK按钮时进行验证
void CAddDlg::OnOK()
{
    if (ValidateInputs()) // 如果验证通过
    {
        // 默认已售票数为0
        m_soldTickets = _T("0");

        // 获取发车时间
        CTime departureTime;
        m_DepartureTimePickerCtrl.GetTime(departureTime);

        BusTimetable newBus;
        newBus.busNumber = CStringA(m_busNumber);
        newBus.departureTime = departureTime;
        newBus.startStation = CStringA(m_startStation);
        newBus.endStation = CStringA(m_endStation);
        newBus.travelTime = CStringA(m_travelTime);
        newBus.price = _ttoi(m_price);
        newBus.capacity = _ttoi(m_capacity);
        newBus.soldTickets = 0; // 默认售票数为0

        if (isLogged)
            backup.push_back(newBus);
        else
            g_TimetableData.push_back(newBus); // 添加到全局数组

        // 调用基类OnOK以关闭对话框
        CDialogEx::OnOK();
    }
}

bool IsNumeric(CString str)
{
    // 检查字符串是否为空
    if (str.IsEmpty())
        return false;

    // 遍历字符串，检查是否全为数字或者小数点
    bool hasDecimalPoint = false;
    for (int i = 0; i < str.GetLength(); ++i)
    {
        if (!_istdigit(str[i]))
        {
            if (str[i] == '.' && !hasDecimalPoint)  // 小数点只能有一个
            {
                hasDecimalPoint = true;
            }
            else
            {
                return false;  // 如果有非数字字符或多于一个小数点，则返回 false
            }
        }
    }

    return true;
}