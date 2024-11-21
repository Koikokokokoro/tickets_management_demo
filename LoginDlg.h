// LoginDlg.h : header file for the CLoginDlg class
//

#if !defined(AFX_LOGINDLG_H__INCLUDED_)
#define AFX_LOGINDLG_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"		// Ö÷·ûºÅ

// CLoginDlg dialog
class CLoginDlg : public CDialog
{
	// Construction
public:
	CLoginDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLoginDlg)
	enum { IDD = IDD_LOGIN_DIALOG };
	CString m_strUsername;
	CString m_strPassword;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoginDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CLoginDlg)
	afx_msg void OnLogin();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif // !defined(AFX_LOGINDLG_H__INCLUDED_)
