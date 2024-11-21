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
    CSearchDlg(CWnd* pParent = nullptr);   // ��׼���캯��
    virtual ~CSearchDlg();

    CString m_searchInput; // �û��������������
    int m_searchType;      // �û�ѡ�����������
    afx_msg void OnBnClickedSearchButton(); // ������ť�Ĵ�����

    void DisplaySearchResults(const vector<BusTimetable>& results);

    // �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_SEARCH_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()
};
