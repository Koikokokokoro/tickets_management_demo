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

    m_DepartureTimePickerCtrl.SetFormat(_T("HH:mm"));  // ��������
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

// ��֤�����ֶε���Ч��
bool CAddDlg::ValidateInputs()
{
    UpdateData(true);

    // ���ÿ�������ֶ��Ƿ�Ϊ��
    if (m_busNumber.IsEmpty() || m_startStation.IsEmpty() || m_endStation.IsEmpty() ||
        m_travelTime.IsEmpty() || m_price.IsEmpty() || m_capacity.IsEmpty())
    {
        AfxMessageBox(_T("��������д�����ֶ�"));
        return false;
    }

    // ��鳵�κ��Ƿ��Ѵ���
    for (const auto& timetable : g_TimetableData)
    {
        if (timetable.busNumber == m_busNumber.GetBuffer())
        {
            AfxMessageBox(_T("���κ��Ѵ��ڣ�������һ���µĳ��κţ�"));
            return false;
        }
    }

    if (!IsNumeric(m_travelTime) || !IsNumeric(m_price) || !IsNumeric(m_capacity))
    {
        AfxMessageBox(_T("�г�ʱ�䡢�۸����������Ϊ��Ч����"));
        return false;
    }

    // ���۸�������Ƿ�������
    float price = _tstof(m_price);
    int capacity = _tstoi(m_capacity);
    if (price < 0 || capacity <= 0)
    {
        AfxMessageBox(_T("�۸����Ϊ����������������Ϊ������"));
        return false;
    }

    return true;
}

// ��дOnOK�������ڵ��OK��ťʱ������֤
void CAddDlg::OnOK()
{
    if (ValidateInputs()) // �����֤ͨ��
    {
        // Ĭ������Ʊ��Ϊ0
        m_soldTickets = _T("0");

        // ��ȡ����ʱ��
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
        newBus.soldTickets = 0; // Ĭ����Ʊ��Ϊ0

        if (isLogged)
            backup.push_back(newBus);
        else
            g_TimetableData.push_back(newBus); // ��ӵ�ȫ������

        // ���û���OnOK�ԹرնԻ���
        CDialogEx::OnOK();
    }
}

bool IsNumeric(CString str)
{
    // ����ַ����Ƿ�Ϊ��
    if (str.IsEmpty())
        return false;

    // �����ַ���������Ƿ�ȫΪ���ֻ���С����
    bool hasDecimalPoint = false;
    for (int i = 0; i < str.GetLength(); ++i)
    {
        if (!_istdigit(str[i]))
        {
            if (str[i] == '.' && !hasDecimalPoint)  // С����ֻ����һ��
            {
                hasDecimalPoint = true;
            }
            else
            {
                return false;  // ����з������ַ������һ��С���㣬�򷵻� false
            }
        }
    }

    return true;
}