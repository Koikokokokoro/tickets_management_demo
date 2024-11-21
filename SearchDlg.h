#pragma once

#include "resource.h"
#include <vector>
#include "AdminDlg.h"
#include <algorithm>
#include "SharedFunc.h"

using namespace std;

class CSearchDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CSearchDlg)

public:
    CSearchDlg(CWnd* pParent = nullptr);   // 标准构造函数
    virtual ~CSearchDlg();

    CString m_searchInput; // 用户输入的搜索内容
    int m_searchType;      // 用户选择的搜索类型
    afx_msg void OnBnClickedSearchButton(); // 搜索按钮的处理函数

    void DisplaySearchResults(const vector<BusTimetable>& results);

    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_SEARCH_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
};
