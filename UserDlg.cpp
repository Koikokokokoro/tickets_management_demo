#include "pch.h"
#include "UserDlg.h"
#include "afxdialogex.h"
#include "SharedFunc.h"

IMPLEMENT_DYNAMIC(CUserDlg, CDialogEx)

CUserDlg::CUserDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_USER_DIALOG, pParent)
{
}

CUserDlg::~CUserDlg()
{
}

void CUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Timetable_List, m_TimetableListCtrl); // 绑定列表控件
	DDX_CBIndex(pDX, IDC_SEARCH_COMBO, m_searchType);
	DDX_Text(pDX, IDC_SEARCH_EDIT, m_searchEdit);
}

BEGIN_MESSAGE_MAP(CUserDlg, CDialogEx)
	ON_BN_CLICKED(IDC_USER_EXIT, &CUserDlg::OnBnClickedUserExit)
	ON_BN_CLICKED(IDC_BUY_BUTTON, &CUserDlg::OnBnClickedBuyButton)
	ON_BN_CLICKED(IDC_SEARCH_BUTTON, &CUserDlg::OnBnClickedSearchButton)
	ON_BN_CLICKED(IDC_REFUND_BUTTON, &CUserDlg::OnBnClickedRefundButton)
	ON_BN_CLICKED(IDC_RESET_BUTTON, &CUserDlg::OnBnClickedResetButton)
	ON_BN_CLICKED(IDC_LOAD_TICKET_BUTTON, &CUserDlg::OnBnClickedLoadTicketButton)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_Timetable_List, &CUserDlg::OnLvnColumnClickUser)
END_MESSAGE_MAP()

BOOL CUserDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 初始化列表控件的列
	m_TimetableListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_TimetableListCtrl.InsertColumn(0, _T("车次"), LVCFMT_LEFT, 100);
	m_TimetableListCtrl.InsertColumn(1, _T("发车时间"), LVCFMT_LEFT, 100);
	m_TimetableListCtrl.InsertColumn(2, _T("起点站"), LVCFMT_LEFT, 100);
	m_TimetableListCtrl.InsertColumn(3, _T("终点站"), LVCFMT_LEFT, 100);
	m_TimetableListCtrl.InsertColumn(4, _T("行车时间"), LVCFMT_LEFT, 100);
	m_TimetableListCtrl.InsertColumn(5, _T("票价"), LVCFMT_LEFT, 100);
	m_TimetableListCtrl.InsertColumn(6, _T("额定载量"), LVCFMT_LEFT, 100);
	m_TimetableListCtrl.InsertColumn(7, _T("已售票数"), LVCFMT_LEFT, 100);

	this->ModifyStyleEx(0, WS_EX_APPWINDOW);

	//m_nSortedColumn = -1;  // 初始无排序列
	m_bSortAscending = TRUE;
	bool isSearched = false;

	// 检查当天的日志文件是否存在
	if (SharedFunc::CheckLogFileExists())
	{
		// 如果当天日志文件存在，从日志文件读取数据
		char filename[100];
		time_t t = time(nullptr);
		tm* now = localtime(&t);
		strftime(filename, sizeof(filename), "%Y-%m-%d.log", now);
		SharedFunc::ReadDataFromFile(filename, g_TimetableData);
	}
	else
	{
		// 否则从list.txt读取数据
		SharedFunc::ReadDataFromFile("list.txt", g_TimetableData);
	}

	v_TimetableData = g_TimetableData;

	SharedFunc::DisplayTimetableData(p_listCtrl, g_TimetableData);

	return TRUE;
}

void CUserDlg::OnBnClickedSearchButton()
{
	UpdateData(TRUE);

	v_TimetableData.clear();

	v_TimetableData = SharedFunc::SearchBus(m_searchType, m_searchEdit);

	if (!v_TimetableData.empty())
	{
		isSearched = true;
		SharedFunc::DisplayTimetableData(p_listCtrl, v_TimetableData);
	}
	else
	{
		v_TimetableData = g_TimetableData;
		AfxMessageBox(_T("未找到符合条件的车次信息！"));
	}
}

// 处理点击购票按钮，打开购票对话框
void CUserDlg::OnBnClickedBuyButton()
{
	HandleTicketTransaction(true);
}

void CUserDlg::OnBnClickedRefundButton()
{
	HandleTicketTransaction(false);
}

void CUserDlg::OnBnClickedResetButton()
{
	isSearched = false;
	v_TimetableData = g_TimetableData;
	SharedFunc::DisplayTimetableData(p_listCtrl, g_TimetableData);
}

void CUserDlg::OnBnClickedLoadTicketButton()
{
	// 清空筛选结果数组
	v_TimetableData.clear();

	// 遍历全局数组，筛选出所有已售票数大于 0 的车次
	for (const auto& timetable : g_TimetableData)
	{
		if (timetable.soldTickets > 0)
		{
			v_TimetableData.push_back(timetable);
		}
	}

	if (!v_TimetableData.empty())
	{
		isSearched = true;
		SharedFunc::DisplayTimetableData(p_listCtrl, v_TimetableData);  // 显示已购票车次
	}
	else
	{
		AfxMessageBox(_T("未找到任何已购买过票的车次！"));
	}
}

// 处理用户退出按钮点击事件
void CUserDlg::OnBnClickedUserExit()
{
	// 获取当前日期并生成文件名
	time_t t = time(nullptr);
	tm* now = localtime(&t);
	char filename[100];
	strftime(filename, sizeof(filename), "%Y-%m-%d.log", now);

	SharedFunc::SaveDataIntoFile(filename, g_TimetableData);

	AfxMessageBox(_T("数据已成功保存到文件！"));
	EndDialog(IDOK);
}

// 列点击事件：点击列头时执行排序
void CUserDlg::OnLvnColumnClickUser(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int columnIndex = pNMLV->iSubItem;  // 获取点击的列索引

	SortTimetableDataUser(columnIndex);

	*pResult = 0;
}

// 排序函数，根据点击的列进行排序
void CUserDlg::SortTimetableDataUser(int columnIndex)
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
			return m_bSortAscending ? _ttof(a.travelTime) < _ttof(b.travelTime) : _ttof(a.travelTime) > _ttof(b.travelTime);
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
		default:
			return false;
		}
		});

	SharedFunc::DisplayTimetableData(p_listCtrl, partlist);
}

void CUserDlg::HandleTicketTransaction(bool isBuying)
{
	POSITION pos = m_TimetableListCtrl.GetFirstSelectedItemPosition();
	if (pos == nullptr)
	{
		AfxMessageBox(_T("请选择一个车次！"));
		return;
	}

	int selectedIndex = m_TimetableListCtrl.GetNextSelectedItem(pos);
	CString selectedBusNumber = m_TimetableListCtrl.GetItemText(selectedIndex, 0); // 获取选中的车次号

	// 在 v_TimetableData 中找到对应的车次
	auto it_v = std::find_if(v_TimetableData.begin(), v_TimetableData.end(),
		[&selectedBusNumber](const BusTimetable& timetable) {
			return timetable.busNumber == selectedBusNumber.GetBuffer();
		});

	if (it_v == v_TimetableData.end())
	{
		AfxMessageBox(_T("未找到该车次！"));
		return;
	}

	BusTimetable& selectedTimetable = *it_v;

	// 检查是否能购票/退票
	if (isBuying)
	{
		if (selectedTimetable.soldTickets >= selectedTimetable.capacity)
		{
			AfxMessageBox(_T("票已售完，无法购票！"));
			return;
		}
		switch (SharedFunc::CheckTime(selectedTimetable.departureTime))
		{
		case 2:
			AfxMessageBox(_T("已超过购票最后时间！"));
			return;
		case 0:
			AfxMessageBox(_T("该车次已发车，无法购票！"));
			return;
		default:
			break;
		}
	}
	else if (!isBuying)
	{
		if (selectedTimetable.soldTickets == 0)
		{
			AfxMessageBox(_T("无票可退！"));
			return;
		}
		switch (SharedFunc::CheckTime(selectedTimetable.departureTime))
		{
		case 2:
			AfxMessageBox(_T("已超过退票最后时间！"));
			return;
		case 0:
			AfxMessageBox(_T("该车次已发车，无法退票！"));
			return;
		default:
			break;
		}
	}

	// 打开确认购票/退票数量的对话框
	CTicketConfirmDlg ticketConfirmDlg;
	if (ticketConfirmDlg.DoModal() == IDOK)
	{
		int ticketCount = ticketConfirmDlg.GetTicketCount();  // 获取用户输入的票数

		if (isBuying)
		{
			if (ticketCount + selectedTimetable.soldTickets > selectedTimetable.capacity)
			{
				AfxMessageBox(_T("余票不足，购票失败！"));
				return;
			}
			selectedTimetable.soldTickets += ticketCount;
			AfxMessageBox(_T("购票成功！"));
		}
		else
		{
			if (ticketCount > selectedTimetable.soldTickets)
			{
				AfxMessageBox(_T("退票数大于已购票数，退票失败！"));
				return;
			}
			selectedTimetable.soldTickets -= ticketCount;
			AfxMessageBox(_T("退票成功！"));
		}

		// 更新显示
		m_TimetableListCtrl.SetItemText(selectedIndex, 7, to_string(selectedTimetable.soldTickets).c_str());

		// 在 g_TimetableData 中找到对应的车次，并同步更新
		auto it_g = std::find_if(g_TimetableData.begin(), g_TimetableData.end(),
			[&selectedBusNumber](const BusTimetable& timetable) {
				return timetable.busNumber == selectedBusNumber.GetBuffer();
			});

		if (it_g != g_TimetableData.end())
		{
			it_g->soldTickets = selectedTimetable.soldTickets;  // 同步已售票数
		}
	}
}