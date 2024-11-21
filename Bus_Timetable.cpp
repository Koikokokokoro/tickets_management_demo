#include "pch.h"
#include "framework.h"
#include "Bus_Timetable.h"
#include "Bus_TimetableDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CBusTimetableApp, CWinApp)
    ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CBusTimetableApp::CBusTimetableApp()
{
    m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

CBusTimetableApp theApp;

BOOL CBusTimetableApp::InitInstance()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    CWinApp::InitInstance();
    AfxEnableControlContainer();

    CShellManager* pShellManager = new CShellManager;
    CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

    SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

    CBusTimetableDlg dlg;
    m_pMainWnd = &dlg;
    INT_PTR nResponse = dlg.DoModal();

    if (pShellManager != nullptr)
    {
        delete pShellManager;
    }

    return FALSE; // 退出应用程序
}

