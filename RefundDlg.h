#pragma once

#include "resource.h"
#include "Bus_TimetableDlg.h"
#include "AdminDlg.h"
#include "BuyDlg.h"

class CRefundDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CRefundDlg)

public:
    CRefundDlg(CWnd* pParent = nullptr);   // ��׼���캯��
    virtual ~CRefundDlg();

    // �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_REFUND_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()

public:
    CString m_RefundBusNumber;  // ��Ʊ���κ�
    int m_RefundTicketCount;    // ��Ʊ����
    afx_msg void OnBnClickedOk();  // ���ȷ�ϰ�ť������Ʊ�߼�
};
