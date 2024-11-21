#include "pch.h"
#include "framework.h"
#include "Bus_Timetable.h"
#include "Bus_TimetableDlg.h"
#include "afxdialogex.h"
#include "LoginDlg.h"
#include "AdminDlg.h"
#include "UserDlg.h"
#include "aes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBusTimetableDlg::CBusTimetableDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_BUS_TIMETABLE_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_AIC);
}

void CBusTimetableDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBusTimetableDlg, CDialogEx)
    ON_BN_CLICKED(IDC_ADMIN_BUTTON, &CBusTimetableDlg::OnBnClickedAdminButton)
    ON_BN_CLICKED(IDC_USER_BUTTON, &CBusTimetableDlg::OnBnClickedUserButton)
    ON_BN_CLICKED(IDC_EXIT, &CBusTimetableDlg::OnBnClickedExit)
END_MESSAGE_MAP()

BOOL CBusTimetableDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    //初始化管理员加密 仅初次运行使用
    //std::vector<std::pair<std::string, std::string>> adminInfo = {
    //{"admin", "123"},
    //{"backup", "password"}
    //};

    //aes aesEncryptor;
    //aesEncryptor.SaveEncryptedAdminInfo(adminInfo, "admininfo.aes");

    return TRUE;
}

void CBusTimetableDlg::OnBnClickedUserButton()
{
    EndDialog(IDOK);
    CUserDlg userDlg;
    userDlg.DoModal();
}

void CBusTimetableDlg::OnBnClickedAdminButton()
{
    // 打开登录对话框
    CLoginDlg loginDlg;
    INT_PTR nResponse = loginDlg.DoModal();

    if (nResponse == IDOK)
    {
        // 登录成功，关闭当前窗口
        EndDialog(IDOK);

        // 创建并显示管理员窗口
        CAdminDlg adminDlg;
        adminDlg.DoModal();
    }
}

void CBusTimetableDlg::OnBnClickedExit()
{
    EndDialog(IDCANCEL);
}
