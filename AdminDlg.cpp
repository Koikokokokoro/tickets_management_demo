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
	DDX_Control(pDX, IDC_Timetable_List, m_TimetableListCtrl);  // ���б�ؼ�
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

	// �����б�ؼ�Ϊ������ͼ
	m_TimetableListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// ��ʼ���б�ؼ���
	m_TimetableListCtrl.InsertColumn(0, _T("����"), LVCFMT_LEFT, 80);
	m_TimetableListCtrl.InsertColumn(1, _T("����ʱ��"), LVCFMT_LEFT, 100);
	m_TimetableListCtrl.InsertColumn(2, _T("���վ"), LVCFMT_LEFT, 100);
	m_TimetableListCtrl.InsertColumn(3, _T("�յ�վ"), LVCFMT_LEFT, 100);
	m_TimetableListCtrl.InsertColumn(4, _T("�г�ʱ��"), LVCFMT_LEFT, 100);
	m_TimetableListCtrl.InsertColumn(5, _T("Ʊ��"), LVCFMT_LEFT, 80);
	m_TimetableListCtrl.InsertColumn(6, _T("�����"), LVCFMT_LEFT, 100);
	m_TimetableListCtrl.InsertColumn(7, _T("����Ʊ��"), LVCFMT_LEFT, 100);

	this->ModifyStyleEx(0, WS_EX_APPWINDOW);

	m_bSortAscending = TRUE;
	bool isSearched = false;

	// ��鵱�����־�ļ��Ƿ����
	if (SharedFunc::CheckLogFileExists())
	{
		isLogged = true;
		// ���������־�ļ����ڣ�����־�ļ���ȡ����
		char filename[100];
		time_t t = time(nullptr);
		tm* now = localtime(&t);
		strftime(filename, sizeof(filename), "%Y-%m-%d.log", now);
		SharedFunc::ReadDataFromFile(filename, g_TimetableData);
	}
	else
	{
		isLogged = false;
		// �����list.txt��ȡ����
		SharedFunc::ReadDataFromFile("list.txt", g_TimetableData);
	}

	v_TimetableData = g_TimetableData;

	SharedFunc::ReadDataFromFile("list.txt", backup);

	SharedFunc::DisplayTimetableData(p_listCtrl, v_TimetableData);

	return TRUE;
}

// ʵ�ְ�ť����¼�������
void CAdminDlg::OnBnClickedAddButton()
{
	// ��������ʾ��ӳ��εĶԻ���
	CAddDlg addDlg;
	if (addDlg.DoModal() == IDOK)
	{
		if (isLogged)
		{
			SharedFunc::SaveDataIntoFile("list.txt", backup);
			AfxMessageBox(_T("�������γɹ�����������Ч��"));
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
		AfxMessageBox(_T("��ѡ��һ�����ν��б༭��"));
		return;
	}

	int selectedIndex = m_TimetableListCtrl.GetNextSelectedItem(pos);
	CString selectedBusNumber = m_TimetableListCtrl.GetItemText(selectedIndex, 0); // ��ȡѡ�еĳ��κ�

	// ��ȫ���������ҵ���Ӧ�ĳ���
	auto it = find_if(g_TimetableData.begin(), g_TimetableData.end(),
		[&selectedBusNumber](const BusTimetable& timetable) {
			return timetable.busNumber == selectedBusNumber.GetBuffer();
		});

	if (it == g_TimetableData.end())
	{
		AfxMessageBox(_T("δ�ҵ��ó��Σ�"));
		return;
	}

	// �򿪱༭���ζԻ��򣬲�����ѡ�еĳ�����Ϣ
	BusTimetable& selectedTimetable = *it;

	// �򿪱༭���ζԻ���
	CEditDlg editDlg;
	editDlg.SetBusInfo(selectedTimetable);  // ��䳵����Ϣ

	if (editDlg.DoModal() == IDOK)
	{
		// ��ȡ�޸ĺ�ĳ�����Ϣ
		editDlg.GetUpdatedBusInfo(selectedTimetable);

		// ����ȫ�������еĳ�����Ϣ����ˢ����ʾ
		auto itG = std::find_if(v_TimetableData.begin(), v_TimetableData.end(),
			[&selectedBusNumber](const BusTimetable& timetable) {
				return timetable.busNumber == CStringA(selectedBusNumber).GetString();
			});

		if (itG != v_TimetableData.end())
		{
			*itG = selectedTimetable;  // ͬ������ȫ������
		}

		// ������ʾ
		SharedFunc::DisplayTimetableData(p_listCtrl, v_TimetableData);
		AfxMessageBox(_T("������Ϣ�ѳɹ����£�"));
	}
}

void CAdminDlg::OnBnClickedResetButton()
{
	isSearched = false;
	v_TimetableData = g_TimetableData;
	SharedFunc::DisplayTimetableData(p_listCtrl, g_TimetableData);
}

//��ѯ����
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
		AfxMessageBox(_T("δ�ҵ����������ĳ�����Ϣ��"));
	}
}

void CAdminDlg::OnBnClickedDeleteButton()
{
	POSITION pos = m_TimetableListCtrl.GetFirstSelectedItemPosition();
	if (pos == nullptr)
	{
		AfxMessageBox(_T("��ѡ��Ҫɾ���ĳ��Σ�"));
		return;
	}

	int selectedIndex = m_TimetableListCtrl.GetNextSelectedItem(pos);
	CString selectedTrainNumber = m_TimetableListCtrl.GetItemText(selectedIndex, 0); // ��ȡѡ�еĳ��κ�

	// ��ȫ���������ҵ���ɾ����Ӧ�ĳ���
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
				AfxMessageBox(_T("�����ѳɹ�ɾ��,��������Ч��"));
			}
			else {
				g_TimetableData = backup;
				m_TimetableListCtrl.DeleteItem(selectedIndex);  // ���б�ؼ���ɾ��ѡ����
				AfxMessageBox(_T("�����ѳɹ�ɾ����"));
			}
		}
	}
	else
	{
		AfxMessageBox(_T("ɾ��ʧ�ܣ�δ�ҵ��ó��Σ�"));
	}
}

// �е���¼��������ͷʱִ������
void CAdminDlg::OnLvnColumnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int columnIndex = pNMLV->iSubItem;  // ��ȡ�����������

	SortTimetableData(columnIndex);

	*pResult = 0;
}

// �����������ݵ�����н�������
void CAdminDlg::SortTimetableData(int columnIndex)
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
			return m_bSortAscending ? a.travelTime < b.travelTime : a.travelTime > b.travelTime;
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
		case 8:  // ����Ʊ��
			return m_bSortAscending ? a.soldTickets < b.soldTickets : a.soldTickets > b.soldTickets;
			break;
		default:
			return false;
		}
		});

	SharedFunc::DisplayTimetableData(p_listCtrl, partlist);
}

//���沢�˳�����
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
			AfxMessageBox(_T("�����ѳɹ����浽�ļ���"));
			EndDialog(IDCANCEL);
		}
		else
		{
			AfxMessageBox(_T("������Ϣʧ�ܣ�"));
			EndDialog(IDCANCEL);
		}
	}
	else
	{
		if (SharedFunc::SaveDataIntoFile("list.txt", g_TimetableData))
		{
			AfxMessageBox(_T("�����ѳɹ����浽�ļ���"));
			EndDialog(IDCANCEL);
		}
		else
		{
			AfxMessageBox(_T("������Ϣʧ�ܣ�"));
			EndDialog(IDCANCEL);
		}
	}
}