#pragma comment(lib, "libssl.lib")
#pragma comment(lib, "libcrypto.lib")

#include "pch.h"
#include "SharedFunc.h"

vector<BusTimetable> g_TimetableData;  // ����ȫ������
vector<BusTimetable> v_TimetableData;  // ����ȫ������
vector<BusTimetable> backup;
extern const string key = "hZ4}c=thlmm3r(2H(W*?e=Z >:|.lnCM";
bool isLogged = false;

int SharedFunc::CheckTime(CTime ntime)
{
    CTime current = CTime::GetCurrentTime();
    CTimeSpan delta = ntime - current;
    int deltasec = delta.GetTotalSeconds();

    if (deltasec >= 0 && deltasec < 600)
    {
        return 2;   //ʱ������ʮ���ӣ���������Ʊ
    }
    else if (deltasec >= 600)
    {
        return 1;    //ʱ�����ʮ���ӣ�������Ʊ
    }
    else
    {
        return 0;   //ʱ���ѹ�����������Ʊ
    }
}

void SharedFunc::ReadDataFromFile(const string& filename, vector<BusTimetable>& TimetableData)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        AfxMessageBox(_T("�޷����ļ���"));
        return;
    }

    TimetableData.clear();  // ���ȫ�����飬׼�����¶�ȡ

    string line;
    while (getline(file, line))
    {
        istringstream stream(line);
        string token;
        BusTimetable timetable;

        // ʹ�ö��ŷָ�ÿ������
        if (getline(stream, token, ',')) timetable.busNumber = token.c_str();
        if (getline(stream, token, ','))
        {
            int ny, nmm, nd, nh, nm;
            CTime currentTime = CTime::GetCurrentTime();
            ny = currentTime.GetYear();
            nmm = currentTime.GetMonth();
            nd = currentTime.GetDay();
            istringstream iss(token);
            string str;
            char interval = ':';
            iss >> nh >> interval >> nm;
            CTime nt(ny, nmm, nd, nh, nm, 0);
            timetable.departureTime = nt;
        }
        if (getline(stream, token, ',')) timetable.startStation = token.c_str();
        if (getline(stream, token, ',')) timetable.endStation = token.c_str();
        if (getline(stream, token, ',')) timetable.travelTime = token.c_str();
        if (getline(stream, token, ',')) timetable.price = atoi(token.c_str());
        if (getline(stream, token, ',')) timetable.capacity = stoi(token);
        if (getline(stream, token, ',')) timetable.soldTickets = stoi(token);

        TimetableData.push_back(timetable);  // ��ӵ�ȫ������
    }
    file.close();
}

bool SharedFunc::SaveDataIntoFile(const string& filename, vector<BusTimetable> TimetableData)
{
    ofstream file(filename);
    if (!file.is_open())
    {
        AfxMessageBox(_T("�޷����ļ��������ݣ�"));
        return false;
    }

    CString cstrdepartureTime;
    string strdepartureTime;

    // �������鲢д���ļ�
    for (const auto& timetable : TimetableData)
    {
        cstrdepartureTime.Format("%d:%d", timetable.departureTime.GetHour(), timetable.departureTime.GetMinute());
        strdepartureTime = cstrdepartureTime.GetBuffer();

        file << timetable.busNumber << ","
            << strdepartureTime << ","
            << timetable.startStation << ","
            << timetable.endStation << ","
            << timetable.travelTime << ","
            << timetable.price << ","
            << timetable.capacity << ","
            << timetable.soldTickets << "\n";
    }

    file.close();

    return true;
}

void SharedFunc::DisplayTimetableData(CListCtrl* m_listCtrl, vector<BusTimetable> TimetableData)
{
    m_listCtrl->DeleteAllItems();  // ����б�ؼ�

    CString c_strdepartureTime;

    // ����ȫ�����飬��ʾ������Ϣ
    for (size_t i = 0; i < TimetableData.size(); ++i)
    {
        const auto& timetable = TimetableData[i];

        string realMin;
        int minute = timetable.departureTime.GetMinute();
        if (minute == 0) realMin = "00";
        else realMin = to_string(minute);
        c_strdepartureTime.Format("%d:%s", timetable.departureTime.GetHour(), realMin.c_str());

        m_listCtrl->InsertItem(i, timetable.busNumber);
        m_listCtrl->SetItemText(i, 1, c_strdepartureTime);
        m_listCtrl->SetItemText(i, 2, timetable.startStation);
        m_listCtrl->SetItemText(i, 3, timetable.endStation);
        m_listCtrl->SetItemText(i, 4, timetable.travelTime);
        m_listCtrl->SetItemText(i, 5, to_string(timetable.price).c_str());
        m_listCtrl->SetItemText(i, 6, to_string(timetable.capacity).c_str());
        m_listCtrl->SetItemText(i, 7, to_string(timetable.soldTickets).c_str());
    }
}

vector<BusTimetable> SharedFunc::SearchBus(int m_searchType, CString m_searchEdit)
{
    string nh, nm, nt;

    vector<BusTimetable> temp;

    switch (m_searchType)
    {
    case 0: //����
        for (const auto& timetable : g_TimetableData)
        {
            if (timetable.busNumber == m_searchEdit)
            {
                temp.push_back(timetable);
            }
        }
        break;
    case 1: //����ʱ��
        for (const auto& timetable : g_TimetableData)
        {
            nh = timetable.departureTime.GetHour();
            nm = timetable.departureTime.GetMinute();
            nt = nh + ":" + nm;
            if (nt.c_str() == m_searchEdit)
            {
                temp.push_back(timetable);
            }
        }
        break;
    case 2: //���վ
        for (const auto& timetable : g_TimetableData)
        {
            if (timetable.startStation == m_searchEdit)
            {
                temp.push_back(timetable);
            }
        }
        break;
    case 3: //�յ�վ
        for (const auto& timetable : g_TimetableData)
        {
            if (timetable.endStation == m_searchEdit)
            {
                temp.push_back(timetable);
            }
        }
        break;
    case 4: //����
        for (const auto& timetable : g_TimetableData)
        {
            if (timetable.travelTime == m_searchEdit)
            {
                temp.push_back(timetable);
            }
        }
        break;
    case 5: //Ʊ��
        for (const auto& timetable : g_TimetableData)
        {
            if (timetable.price == _ttoi(m_searchEdit))
            {
                temp.push_back(timetable);
            }
        }
        break;
    case 6: //�����
        for (const auto& timetable : g_TimetableData)
        {
            if (timetable.capacity == _ttoi(m_searchEdit))
            {
                temp.push_back(timetable);
            }
        }
    default:
        break;
    }
    return temp;
}

bool SharedFunc::CheckLogFileExists()
{
    // ��ȡ��ǰ���ڲ������ļ���
    char filename[100];
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    strftime(filename, sizeof(filename), "%Y-%m-%d.log", now);

    // ����ļ��Ƿ����
    ifstream file(filename);
    return file.good();
}