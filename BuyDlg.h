#pragma once

#include "SharedFunc.h"

class CBuyDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CBuyDlg)

public:
    CBuyDlg(CWnd* pParent = nullptr);   // ��׼���캯��
    virtual ~CBuyDlg();

    // �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_BUY_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()

public:
    CString m_BusNumber;  // �洢�û�����ĳ��κ�
    int m_TicketCount;    // �洢�û�����Ĺ�Ʊ����
    afx_msg void OnBnClickedOk();  // ���ȷ�ϰ�ť����Ʊ�߼�

    //int CheckTime(CTime ntime);
};
