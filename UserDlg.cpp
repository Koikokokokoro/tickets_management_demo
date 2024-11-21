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
	DDX_Control(pDX, IDC_Timetable_List, m_TimetableListCtrl); // ���б�ؼ�
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

	// ��ʼ���б�ؼ�����
	m_TimetableListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_TimetableListCtrl.InsertColumn(0, _T("����"), LVCFMT_LEFT, 100);
	m_TimetableListCtrl.InsertColumn(1, _T("����ʱ��"), LVCFMT_LEFT, 100);
	m_TimetableListCtrl.InsertColumn(2, _T("���վ"), LVCFMT_LEFT, 100);
	m_TimetableListCtrl.InsertColumn(3, _T("�յ�վ"), LVCFMT_LEFT, 100);
	m_TimetableListCtrl.InsertColumn(4, _T("�г�ʱ��"), LVCFMT_LEFT, 100);
	m_TimetableListCtrl.InsertColumn(5, _T("Ʊ��"), LVCFMT_LEFT, 100);
	m_TimetableListCtrl.InsertColumn(6, _T("�����"), LVCFMT_LEFT, 100);
	m_TimetableListCtrl.InsertColumn(7, _T("����Ʊ��"), LVCFMT_LEFT, 100);

	this->ModifyStyleEx(0, WS_EX_APPWINDOW);

	//m_nSortedColumn = -1;  // ��ʼ��������
	m_bSortAscending = TRUE;
	bool isSearched = false;

	// ��鵱�����־�ļ��Ƿ����
	if (SharedFunc::CheckLogFileExists())
	{
		// ���������־�ļ����ڣ�����־�ļ���ȡ����
		char filename[100];
		time_t t = time(nullptr);
		tm* now = localtime(&t);
		strftime(filename, sizeof(filename), "%Y-%m-%d.log", now);
		SharedFunc::ReadDataFromFile(filename, g_TimetableData);
	}
	else
	{
		// �����list.txt��ȡ����
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
		AfxMessageBox(_T("δ�ҵ����������ĳ�����Ϣ��"));
	}
}

// ��������Ʊ��ť���򿪹�Ʊ�Ի���
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
	// ���ɸѡ�������
	v_TimetableData.clear();

	// ����ȫ�����飬ɸѡ����������Ʊ������ 0 �ĳ���
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
		SharedFunc::DisplayTimetableData(p_listCtrl, v_TimetableData);  // ��ʾ�ѹ�Ʊ����
	}
	else
	{
		AfxMessageBox(_T("δ�ҵ��κ��ѹ����Ʊ�ĳ��Σ�"));
	}
}

// �����û��˳���ť����¼�
void CUserDlg::OnBnClickedUserExit()
{
	// ��ȡ��ǰ���ڲ������ļ���
	time_t t = time(nullptr);
	tm* now = localtime(&t);
	char filename[100];
	strftime(filename, sizeof(filename), "%Y-%m-%d.log", now);

	SharedFunc::SaveDataIntoFile(filename, g_TimetableData);

	AfxMessageBox(_T("�����ѳɹ����浽�ļ���"));
	EndDialog(IDOK);
}

// �е���¼��������ͷʱִ������
void CUserDlg::OnLvnColumnClickUser(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int columnIndex = pNMLV->iSubItem;  // ��ȡ�����������

	SortTimetableDataUser(columnIndex);

	*pResult = 0;
}

// �����������ݵ�����н�������
void CUserDlg::SortTimetableDataUser(int columnIndex)
{
	m_bSortAscending = !m_bSortAscending;

	vector<BusTimetable> partlist;

	if (isSearched) partlist = v_TimetableData;
	else partlist = g_TimetableData;

	sort(partlist.begin(), partlist.end(), [=](const BusTimetable& a, const BusTimetable& b) -> bool {
		switch (columnIndex)
		{
		case 1:  // ����ʱ��
			return m_bSortAscending ? a.departureTime < b.departureTime : a.departureTime > b.departureTime;
			break;
		case 4:  // ;��ʱ��
			return m_bSortAscending ? _ttof(a.travelTime) < _ttof(b.travelTime) : _ttof(a.travelTime) > _ttof(b.travelTime);
			break;
		case 5:	 // Ʊ��
			return m_bSortAscending ? a.price < b.price : a.price > b.price;
			break;
		case 6:  // �����
			return m_bSortAscending ? a.capacity < b.capacity : a.capacity > b.capacity;
			break;
		case 7:  // ����Ʊ��
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
		AfxMessageBox(_T("��ѡ��һ�����Σ�"));
		return;
	}

	int selectedIndex = m_TimetableListCtrl.GetNextSelectedItem(pos);
	CString selectedBusNumber = m_TimetableListCtrl.GetItemText(selectedIndex, 0); // ��ȡѡ�еĳ��κ�

	// �� v_TimetableData ���ҵ���Ӧ�ĳ���
	auto it_v = std::find_if(v_TimetableData.begin(), v_TimetableData.end(),
		[&selectedBusNumber](const BusTimetable& timetable) {
			return timetable.busNumber == selectedBusNumber.GetBuffer();
		});

	if (it_v == v_TimetableData.end())
	{
		AfxMessageBox(_T("δ�ҵ��ó��Σ�"));
		return;
	}

	BusTimetable& selectedTimetable = *it_v;

	// ����Ƿ��ܹ�Ʊ/��Ʊ
	if (isBuying)
	{
		if (selectedTimetable.soldTickets >= selectedTimetable.capacity)
		{
			AfxMessageBox(_T("Ʊ�����꣬�޷���Ʊ��"));
			return;
		}
		switch (SharedFunc::CheckTime(selectedTimetable.departureTime))
		{
		case 2:
			AfxMessageBox(_T("�ѳ�����Ʊ���ʱ�䣡"));
			return;
		case 0:
			AfxMessageBox(_T("�ó����ѷ������޷���Ʊ��"));
			return;
		default:
			break;
		}
	}
	else if (!isBuying)
	{
		if (selectedTimetable.soldTickets == 0)
		{
			AfxMessageBox(_T("��Ʊ���ˣ�"));
			return;
		}
		switch (SharedFunc::CheckTime(selectedTimetable.departureTime))
		{
		case 2:
			AfxMessageBox(_T("�ѳ�����Ʊ���ʱ�䣡"));
			return;
		case 0:
			AfxMessageBox(_T("�ó����ѷ������޷���Ʊ��"));
			return;
		default:
			break;
		}
	}

	// ��ȷ�Ϲ�Ʊ/��Ʊ�����ĶԻ���
	CTicketConfirmDlg ticketConfirmDlg;
	if (ticketConfirmDlg.DoModal() == IDOK)
	{
		int ticketCount = ticketConfirmDlg.GetTicketCount();  // ��ȡ�û������Ʊ��

		if (isBuying)
		{
			if (ticketCount + selectedTimetable.soldTickets > selectedTimetable.capacity)
			{
				AfxMessageBox(_T("��Ʊ���㣬��Ʊʧ�ܣ�"));
				return;
			}
			selectedTimetable.soldTickets += ticketCount;
			AfxMessageBox(_T("��Ʊ�ɹ���"));
		}
		else
		{
			if (ticketCount > selectedTimetable.soldTickets)
			{
				AfxMessageBox(_T("��Ʊ�������ѹ�Ʊ������Ʊʧ�ܣ�"));
				return;
			}
			selectedTimetable.soldTickets -= ticketCount;
			AfxMessageBox(_T("��Ʊ�ɹ���"));
		}

		// ������ʾ
		m_TimetableListCtrl.SetItemText(selectedIndex, 7, to_string(selectedTimetable.soldTickets).c_str());

		// �� g_TimetableData ���ҵ���Ӧ�ĳ��Σ���ͬ������
		auto it_g = std::find_if(g_TimetableData.begin(), g_TimetableData.end(),
			[&selectedBusNumber](const BusTimetable& timetable) {
				return timetable.busNumber == selectedBusNumber.GetBuffer();
			});

		if (it_g != g_TimetableData.end())
		{
			it_g->soldTickets = selectedTimetable.soldTickets;  // ͬ������Ʊ��
		}
	}
}