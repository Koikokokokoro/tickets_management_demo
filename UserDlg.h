#pragma once

#include "resource.h"
#include <vector>
#include "SearchDlg.h"
#include "SharedFunc.h"
#include "TicketConfirmDlg.h"

using namespace std;

class CUserDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CUserDlg)

public:
    CUserDlg(CWnd* pParent = nullptr);
    virtual ~CUserDlg();

    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_USER_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    void HandleTicketTransaction(bool isBuying);

    CListCtrl m_TimetableListCtrl;  // 用于显示时刻表的列表控件

    //int m_nSortedColumn;
    BOOL m_bSortAscending;
    bool isSearched;

    DECLARE_MESSAGE_MAP()

public:
    virtual BOOL OnInitDialog();  // 初始化时读取list.txt并显示数据
    bool CheckLogFileExists();

    afx_msg void OnBnClickedUserExit(); // 处理用户退出按钮
    afx_msg void OnBnClickedSearchButton();
    afx_msg void OnBnClickedBuyButton();
    afx_msg void OnBnClickedRefundButton();
    afx_msg void OnBnClickedResetButton();
    afx_msg void OnBnClickedLoadTicketButton();
    afx_msg void OnLvnColumnClickUser(NMHDR* pNMHDR, LRESULT* pResult);  // 列点击事件

    CListCtrl* p_listCtrl = &m_TimetableListCtrl;

    CString m_searchEdit;
    int m_searchType;

private:
    void SortTimetableDataUser(int columnIndex);  // 排序函数
};
