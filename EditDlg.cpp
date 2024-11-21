#include "pch.h"
#include "EditDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CEditDlg, CDialogEx)

CEditDlg::CEditDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_EDIT_DIALOG, pParent), m_Price(0), m_Capacity(0), m_SoldTickets(0)
{
}

CEditDlg::~CEditDlg()
{
}

void CEditDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_BUS_NUMBER_EDIT, m_BusNumber);
    DDX_Control(pDX, IDC_DEPARTURE_TIMEPICKER, m_DepartureTimePickerCtrl);
    DDX_Text(pDX, IDC_START_STATION_EDIT, m_StartStation);
    DDX_Text(pDX, IDC_END_STATION_EDIT, m_EndStation);
    DDX_Text(pDX, IDC_DURATION_EDIT, m_TravelTime);
    DDX_Text(pDX, IDC_PRICE_EDIT, m_Price);
    DDX_Text(pDX, IDC_CAPACITY_EDIT, m_Capacity);
    DDX_Text(pDX, IDC_SOLDED_EDIT, m_SoldTickets);
}

BEGIN_MESSAGE_MAP(CEditDlg, CDialogEx)
END_MESSAGE_MAP()

// 设置车次信息
void CEditDlg::SetBusInfo(const BusTimetable& timetable)
{
    m_BusNumber = timetable.busNumber;
    m_DepartureTime = timetable.departureTime;
    m_StartStation = timetable.startStation;
    m_EndStation = timetable.endStation;
    m_TravelTime = timetable.travelTime;
    m_Price = timetable.price;
    m_Capacity = timetable.capacity;
    m_SoldTickets = timetable.soldTickets;
}

// 获取修改后的车次信息
void CEditDlg::GetUpdatedBusInfo(BusTimetable& timetable)
{
    timetable.busNumber = CStringA(m_BusNumber).GetString();
    timetable.departureTime = m_DepartureTime;
    timetable.startStation = CStringA(m_StartStation).GetString();
    timetable.endStation = CStringA(m_EndStation).GetString();
    timetable.travelTime = CStringA(m_TravelTime).GetString();
    timetable.price = m_Price;
    timetable.capacity = m_Capacity;
    timetable.soldTickets = m_SoldTickets;
    timetable.soldTickets = m_SoldTickets;
}
