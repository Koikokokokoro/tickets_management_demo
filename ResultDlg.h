#pragma once

#include <vector>
#include "resource.h"
#include "SharedFunc.h"

class CResultDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CResultDlg)

public:
    CResultDlg(CWnd* pParent = nullptr);
    virtual ~CResultDlg();

    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_RESULT_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

    DECLARE_MESSAGE_MAP()

private:
    std::vector<BusTimetable> m_results; // 存储搜索结果
    CListCtrl m_resultListCtrl; // 用于显示结果的列表控件

public:
    void SetResults(const std::vector<BusTimetable>& results); // 设置搜索结果
    virtual BOOL OnInitDialog(); // 初始化对话框
    afx_msg void OnBnClickedOk();
};
