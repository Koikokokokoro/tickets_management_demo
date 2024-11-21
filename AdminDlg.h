#pragma once
#include <string>
#include <vector>
#include "resource.h"
#include "CheckDlg.h"
#include "SharedFunc.h"

using namespace std;

class CAdminDlg : public CDialog
{
public:
    CAdminDlg(CWnd* pParent = NULL);

    enum { IDD = IDD_ADMIN_DIALOG };

    afx_msg void OnBnClickedAddButton();
    afx_msg void OnBnClickedSearchButton();
    afx_msg void OnBnClickedExitButton();
    afx_msg void OnBnClickedDeleteButton();
    afx_msg void OnBnClickedResetButton();
    afx_msg void OnBnClickedEditButton();
    afx_msg void OnLvnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);  // �е���¼�

    void UpdateTimetableList();

    CListCtrl* p_listCtrl = &m_TimetableListCtrl;

protected:
    virtual BOOL OnInitDialog();  // ��д OnInitDialog
    virtual void DoDataExchange(CDataExchange* pDX);

    // ��¼��ǰ�����м�����˳��
    int m_nSortedColumn;
    BOOL m_bSortAscending;
    int m_searchType;
    CString m_searchEdit;

    bool isSearched;

    // �б�������
    //static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

    DECLARE_MESSAGE_MAP()

private:
    CListCtrl m_TimetableListCtrl;  // �б�ؼ���Ա����

    void SortTimetableData(int columnIndex);  // ������
};
