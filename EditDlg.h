#pragma once

#include "resource.h"
#include "SharedFunc.h"

class CEditDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CEditDlg)

public:
    CEditDlg(CWnd* pParent = nullptr);   // 标准构造函数
    virtual ~CEditDlg();

    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_EDIT_BUS_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

    DECLARE_MESSAGE_MAP()

private:
    CString m_BusNumber;        // 车次号
    CTime m_DepartureTime;    // 发车时间
    CString m_StartStation;     // 起点站
    CString m_EndStation;       // 终点站
    CString m_TravelTime;       // 行车时间
    int m_Price;                // 票价
    int m_Capacity;             // 额定载量
    int m_SoldTickets;          // 已售票数

    CDateTimeCtrl m_DepartureTimePickerCtrl;

public:
    void SetBusInfo(const BusTimetable& timetable);  // 设置车次信息
    void GetUpdatedBusInfo(BusTimetable& timetable); // 获取修改后的车次信息
};
