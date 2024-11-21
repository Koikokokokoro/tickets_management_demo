#pragma once

#include "resource.h"
#include "SharedFunc.h"

class CEditDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CEditDlg)

public:
    CEditDlg(CWnd* pParent = nullptr);   // ��׼���캯��
    virtual ~CEditDlg();

    // �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_EDIT_BUS_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

    DECLARE_MESSAGE_MAP()

private:
    CString m_BusNumber;        // ���κ�
    CTime m_DepartureTime;    // ����ʱ��
    CString m_StartStation;     // ���վ
    CString m_EndStation;       // �յ�վ
    CString m_TravelTime;       // �г�ʱ��
    int m_Price;                // Ʊ��
    int m_Capacity;             // �����
    int m_SoldTickets;          // ����Ʊ��

    CDateTimeCtrl m_DepartureTimePickerCtrl;

public:
    void SetBusInfo(const BusTimetable& timetable);  // ���ó�����Ϣ
    void GetUpdatedBusInfo(BusTimetable& timetable); // ��ȡ�޸ĺ�ĳ�����Ϣ
};
