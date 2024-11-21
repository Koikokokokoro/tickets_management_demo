#pragma once

#include "resource.h"
#include "AdminDlg.h"

class CCheckDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CCheckDlg)

public:
    CCheckDlg(CWnd* pParent = nullptr);   // 标准构造函数
    virtual ~CCheckDlg();

    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_CHECK_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
};
