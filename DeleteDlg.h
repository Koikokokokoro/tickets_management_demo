#pragma once

#include "resource.h"
#include "AdminDlg.h"
#include <string>

class CDeleteDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CDeleteDlg)

public:
    CDeleteDlg(CWnd* pParent = nullptr);   // ��׼���캯��
    virtual ~CDeleteDlg();

    afx_msg void OnBnClickedCheckButton();

    // �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DELETE_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    bool CheckExistence();

    CString m_deletebus;

    DECLARE_MESSAGE_MAP()
};
