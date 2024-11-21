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

    // �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_RESULT_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

    DECLARE_MESSAGE_MAP()

private:
    std::vector<BusTimetable> m_results; // �洢�������
    CListCtrl m_resultListCtrl; // ������ʾ������б�ؼ�

public:
    void SetResults(const std::vector<BusTimetable>& results); // �����������
    virtual BOOL OnInitDialog(); // ��ʼ���Ի���
    afx_msg void OnBnClickedOk();
};
