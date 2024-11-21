#include "pch.h"
#include "LoginDlg.h"
#include "Bus_Timetable.h"
#include "AdminDlg.h"
#include <string>
#include <iostream>
#include <fstream>
#include <aes.h>

using namespace std;

bool ReadUserInfoFromFile(string filename, CStringArray& usernames, CStringArray& passwords);

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CLoginDlg::IDD, pParent)
{
    m_strUsername = _T("");
    m_strPassword = _T("");
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_USERNAME_EDIT, m_strUsername);
    DDX_Text(pDX, IDC_PASSWORD_EDIT, m_strPassword);
}

BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
    ON_BN_CLICKED(IDC_LOGIN_BUTTON, &CLoginDlg::OnLogin)
END_MESSAGE_MAP()

void CLoginDlg::OnLogin()
{
    UpdateData(TRUE); // 获取输入框中的数据

    CStringArray usernames, passwords;
    string filename = "admininfo.aes";

    vector<pair<string, string>> adminInfo;

    aes aesDecryptor;

    ifstream f(filename);
    if (f.good())
    {
        aesDecryptor.LoadDecryptedAdminInfo(adminInfo, filename.data());
        bool isAuthenticated = false;
        for (const auto& admin : adminInfo)
        {
            if (m_strUsername.GetBuffer() == admin.first && m_strPassword.GetBuffer() == admin.second)
            {
                isAuthenticated = true;
                break;
            }
        }
        if (isAuthenticated)
        {
            AfxMessageBox(_T("登录成功！"));
            EndDialog(IDOK);  // 关闭登录对话框
        }
        else
        {
            AfxMessageBox(_T("用户名或密码错误！"));
        }
    }
    else
    {
        AfxMessageBox(_T("无法加载用户信息！"));
    }

    //if (ReadUserInfoFromFile(filename, usernames, passwords))
    //{
    //    bool isAuthenticated = false;
    //    for (int i = 0; i < usernames.GetSize(); ++i)
    //    {
    //        if (m_strUsername == usernames[i] && m_strPassword == passwords[i])
    //        {
    //            isAuthenticated = true;
    //            break;
    //        }
    //    }

    //    if (isAuthenticated)
    //    {
    //        AfxMessageBox(_T("登录成功！"));
    //        EndDialog(IDOK);  // 关闭登录对话框
    //    }
    //    else
    //    {
    //        AfxMessageBox(_T("用户名或密码错误！"));
    //    }
    //}
    //else
    //{
    //    AfxMessageBox(_T("无法加载用户信息！"));
    //}
}

bool ReadUserInfoFromFile(string filename, CStringArray& usernames, CStringArray& passwords)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        return false;
    }

    string line;
    while (getline(file, line))
    {
        size_t pos = line.find(':');
        if (pos != string::npos)
        {
            usernames.Add(CString(line.substr(0, pos).c_str()));
            passwords.Add(CString(line.substr(pos + 1).c_str()));
        }
    }
    file.close();
    return true;
}
