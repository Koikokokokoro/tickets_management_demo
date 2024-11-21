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

    // �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_USER_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    void HandleTicketTransaction(bool isBuying);

    CListCtrl m_TimetableListCtrl;  // ������ʾʱ�̱���б�ؼ�

    //int m_nSortedColumn;
    BOOL m_bSortAscending;
    bool isSearched;

    DECLARE_MESSAGE_MAP()

public:
    virtual BOOL OnInitDialog();  // ��ʼ��ʱ��ȡlist.txt����ʾ����
    bool CheckLogFileExists();

    afx_msg void OnBnClickedUserExit(); // �����û��˳���ť
    afx_msg void OnBnClickedSearchButton();
    afx_msg void OnBnClickedBuyButton();
    afx_msg void OnBnClickedRefundButton();
    afx_msg void OnBnClickedResetButton();
    afx_msg void OnBnClickedLoadTicketButton();
    afx_msg void OnLvnColumnClickUser(NMHDR* pNMHDR, LRESULT* pResult);  // �е���¼�

    CListCtrl* p_listCtrl = &m_TimetableListCtrl;

    CString m_searchEdit;
    int m_searchType;

private:
    void SortTimetableDataUser(int columnIndex);  // ������
};
