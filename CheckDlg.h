#pragma once

#include "resource.h"
#include "AdminDlg.h"

class CCheckDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CCheckDlg)

public:
    CCheckDlg(CWnd* pParent = nullptr);   // ��׼���캯��
    virtual ~CCheckDlg();

    // �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_CHECK_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()
};
