// AddDlg.h
#pragma once

#include "resource.h"
#include <ctime>
#include "SharedFunc.h"

class CAddDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CAddDlg)

public:
    CAddDlg(CWnd* pParent = nullptr);   // ��׼���캯��
    virtual ~CAddDlg();

    // �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ADD_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    virtual BOOL OnInitDialog();

    DECLARE_MESSAGE_MAP()

private:
    //CDateTimeCtrl m_DurationTimePickerCtrl;

public:
    CString m_busNumber;
    CString m_departureTime;
    CString m_startStation;
    CString m_endStation;
    CString m_travelTime;
    CString m_price;
    CString m_capacity;
    CString m_soldTickets;

    CDateTimeCtrl m_DepartureTimePickerCtrl;

public:

    virtual void OnOK(); // ��дOnOK����������OK��ťʱ��֤����
    bool ValidateInputs(); // ������֤����
};