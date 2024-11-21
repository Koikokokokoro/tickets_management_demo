#include "pch.h"
#include "afxdialogex.h"
#include "AdminDlg.h"
#include "AddDlg.h"
#include "EditDlg.h"
#include "SearchDlg.h"
#include "DeleteDlg.h"
#include "SharedFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

CAdminDlg::CAdminDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ADMIN_DIALOG, pParent)
{
}

void CAdminDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Timetable_List, m_TimetableListCtrl);  // 绑定列表控件
	DDX_Text(pDX, IDC_SEARCH_EDIT, m_searchEdit);
	DDX_CBIndex(pDX, IDC_SEARCH_COMBO, m_searchType);
}

BEGIN_MESSAGE_MAP(CAdminDlg, CDialog)
	ON_BN_CLICKED(IDC_ADD_BUTTON, &CAdminDlg::OnBnClickedAddButton)
	ON_BN_CLICKED(IDC_SEARCH_BUTTON, &CAdminDlg::OnBnClickedSearchButton)
	ON_BN_CLICKED(IDC_DELETE_BUTTON, &CAdminDlg::OnBnClickedDeleteButton)
	ON_BN_CLICKED(IDC_ADMIN_EXIT, &CAdminDlg::OnBnClickedExitButton)
	ON_BN_CLICKED(IDC_RESET_BUTTON, &CAdminDlg::OnBnClickedResetButton)
	ON_BN_CLICKED(IDC_EDIT_BUTTON, &CAdminDlg::OnBnClickedEditButton)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_Timetable_List, &CAdminDlg::OnLvnColumnClick)
END_MESSAGE_MAP()


BOOL CAdminDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置列表控件为报表视图
	m_TimetableListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 初始化列表控件列
	m_TimetableListCtrl.InsertColumn(0, _T("车次"), LVCFMT_LEFT, 80);
	m_TimetableListCtrl.InsertColumn(1, _T("发车时间"), LVCFMT_LEFT, 100);
	m_TimetableListCtrl.InsertColumn(2, _T("起点站"), LVCFMT_LEFT, 100);
	m_TimetableListCtrl.InsertColumn(3, _T("终点站"), LVCFMT_LEFT, 100);
	m_TimetableListCtrl.InsertColumn(4, _T("行车时间"), LVCFMT_LEFT, 100);
	m_TimetableListCtrl.InsertColumn(5, _T("票价"), LVCFMT_LEFT, 80);
	m_TimetableListCtrl.InsertColumn(6, _T("额定载量"), LVCFMT_LEFT, 100);
	m_TimetableListCtrl.InsertColumn(7, _T("已售票数"), LVCFMT_LEFT, 100);

	this->ModifyStyleEx(0, WS_EX_APPWINDOW);

	m_bSortAscending = TRUE;
	bool isSearched = false;

	// 检查当天的日志文件是否存在
	if (SharedFunc::CheckLogFileExists())
	{
		isLogged = true;
		// 如果当天日志文件存在，从日志文件读取数据
		char filename[100];
		time_t t = time(nullptr);
		tm* now = localtime(&t);
		strftime(filename, sizeof(filename), "%Y-%m-%d.log", now);
		SharedFunc::ReadDataFromFile(filename, g_TimetableData);
	}
	else
	{
		isLogged = false;
		// 否则从list.txt读取数据
		SharedFunc::ReadDataFromFile("list.txt", g_TimetableData);
	}

	v_TimetableData = g_TimetableData;

	SharedFunc::ReadDataFromFile("list.txt", backup);

	SharedFunc::DisplayTimetableData(p_listCtrl, v_TimetableData);

	return TRUE;
}

// 实现按钮点击事件处理函数
void CAdminDlg::OnBnClickedAddButton()
{
	// 创建并显示添加车次的对话框
	CAddDlg addDlg;
	if (addDlg.DoModal() == IDOK)
	{
		if (isLogged)
		{
			SharedFunc::SaveDataIntoFile("list.txt", backup);
			AfxMessageBox(_T("新增车次成功，明日起生效！"));
		}
		else 
		{
			m_TimetableListCtrl.DeleteAllItems();
			if (isSearched)
				v_TimetableData = SharedFunc::SearchBus(m_searchType, m_searchEdit);
			else
				v_TimetableData = g_TimetableData;
			SharedFunc::DisplayTimetableData(p_listCtrl, v_TimetableData);
		}
	}
}

void CAdminDlg::OnBnClickedEditButton()
{
	POSITION pos = m_TimetableListCtrl.GetFirstSelectedItemPosition();
	if (pos == nullptr)
	{
		AfxMessageBox(_T("请选择一个车次进行编辑！"));
		return;
	}

	int selectedIndex = m_TimetableListCtrl.GetNextSelectedItem(pos);
	CString selectedBusNumber = m_TimetableListCtrl.GetItemText(selectedIndex, 0); // 获取选中的车次号

	// 在全局数组中找到对应的车次
	auto it = find_if(g_TimetableData.begin(), g_TimetableData.end(),
		[&selectedBusNumber](const BusTimetable& timetable) {
			return timetable.busNumber == selectedBusNumber.GetBuffer();
		});

	if (it == g_TimetableData.end())
	{
		AfxMessageBox(_T("未找到该车次！"));
		return;
	}

	// 打开编辑车次对话框，并传递选中的车次信息
	BusTimetable& selectedTimetable = *it;

	// 打开编辑车次对话框
	CEditDlg editDlg;
	editDlg.SetBusInfo(selectedTimetable);  // 填充车次信息

	if (editDlg.DoModal() == IDOK)
	{
		// 获取修改后的车次信息
		editDlg.GetUpdatedBusInfo(selectedTimetable);

		// 更新全局数组中的车次信息，并刷新显示
		auto itG = std::find_if(v_TimetableData.begin(), v_TimetableData.end(),
			[&selectedBusNumber](const BusTimetable& timetable) {
				return timetable.busNumber == CStringA(selectedBusNumber).GetString();
			});

		if (itG != v_TimetableData.end())
		{
			*itG = selectedTimetable;  // 同步更新全局数组
		}

		// 更新显示
		SharedFunc::DisplayTimetableData(p_listCtrl, v_TimetableData);
		AfxMessageBox(_T("车次信息已成功更新！"));
	}
}

void CAdminDlg::OnBnClickedResetButton()
{
	isSearched = false;
	v_TimetableData = g_TimetableData;
	SharedFunc::DisplayTimetableData(p_listCtrl, g_TimetableData);
}

//查询车次
void CAdminDlg::OnBnClickedSearchButton()
{
	UpdateData(TRUE);

	v_TimetableData = SharedFunc::SearchBus(m_searchType, m_searchEdit);

	if (!v_TimetableData.empty())
	{
		isSearched = true;
		SharedFunc::DisplayTimetableData(p_listCtrl, v_TimetableData);
	}
	else
	{
		AfxMessageBox(_T("未找到符合条件的车次信息！"));
	}
}

void CAdminDlg::OnBnClickedDeleteButton()
{
	POSITION pos = m_TimetableListCtrl.GetFirstSelectedItemPosition();
	if (pos == nullptr)
	{
		AfxMessageBox(_T("请选择要删除的车次！"));
		return;
	}

	int selectedIndex = m_TimetableListCtrl.GetNextSelectedItem(pos);
	CString selectedTrainNumber = m_TimetableListCtrl.GetItemText(selectedIndex, 0); // 获取选中的车次号

	// 在全局数组中找到并删除对应的车次
	auto it = std::find_if(backup.begin(), backup.end(),
		[&selectedTrainNumber](const BusTimetable& timetable) {
			return timetable.busNumber == selectedTrainNumber.GetBuffer();
		});

	if (it != backup.end())
	{
		CCheckDlg checkDlg;
		if (checkDlg.DoModal() == IDOK)
		{
			backup.erase(it);
			if (isLogged)
			{
				SharedFunc::SaveDataIntoFile("list.txt", backup);
				AfxMessageBox(_T("车次已成功删除,明日起生效！"));
			}
			else {
				g_TimetableData = backup;
				m_TimetableListCtrl.DeleteItem(selectedIndex);  // 从列表控件中删除选中项
				AfxMessageBox(_T("车次已成功删除！"));
			}
		}
	}
	else
	{
		AfxMessageBox(_T("删除失败，未找到该车次！"));
	}
}

// 列点击事件：点击列头时执行排序
void CAdminDlg::OnLvnColumnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int columnIndex = pNMLV->iSubItem;  // 获取点击的列索引

	SortTimetableData(columnIndex);

	*pResult = 0;
}

// 排序函数，根据点击的列进行排序
void CAdminDlg::SortTimetableData(int columnIndex)
{
	m_bSortAscending = !m_bSortAscending;

	vector<BusTimetable> partlist;

	if (isSearched) partlist = v_TimetableData;
	else partlist = g_TimetableData;

	sort(partlist.begin(), partlist.end(), [=](const BusTimetable& a, const BusTimetable& b) -> bool {
		switch (columnIndex)
		{
		case 1:  // 发车时间
			return m_bSortAscending ? a.departureTime < b.departureTime : a.departureTime > b.departureTime;
			break;
		case 4:  // 途径时间
			return m_bSortAscending ? a.travelTime < b.travelTime : a.travelTime > b.travelTime;
			break;
		case 5:	 // 票价
			return m_bSortAscending ? a.price < b.price : a.price > b.price;
			break;
		case 6:  // 额定载量
			return m_bSortAscending ? a.capacity < b.capacity : a.capacity > b.capacity;
			break;
		case 7:  // 已售票数
			return m_bSortAscending ? a.soldTickets < b.soldTickets : a.soldTickets > b.soldTickets;
			break;
		case 8:  // 已售票数
			return m_bSortAscending ? a.soldTickets < b.soldTickets : a.soldTickets > b.soldTickets;
			break;
		default:
			return false;
		}
		});

	SharedFunc::DisplayTimetableData(p_listCtrl, partlist);
}

//保存并退出程序
void CAdminDlg::OnBnClickedExitButton()
{
	if (isLogged)
	{
		char filename[100];
		time_t t = time(nullptr);
		tm* now = localtime(&t);
		strftime(filename, sizeof(filename), "%Y-%m-%d.log", now);
		if (SharedFunc::SaveDataIntoFile(filename, g_TimetableData))
		{
			AfxMessageBox(_T("数据已成功保存到文件！"));
			EndDialog(IDCANCEL);
		}
		else
		{
			AfxMessageBox(_T("保存信息失败！"));
			EndDialog(IDCANCEL);
		}
	}
	else
	{
		if (SharedFunc::SaveDataIntoFile("list.txt", g_TimetableData))
		{
			AfxMessageBox(_T("数据已成功保存到文件！"));
			EndDialog(IDCANCEL);
		}
		else
		{
			AfxMessageBox(_T("保存信息失败！"));
			EndDialog(IDCANCEL);
		}
	}
}