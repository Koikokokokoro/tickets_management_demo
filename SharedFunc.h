#pragma once

#include "afxdialogex.h"
#include "resource.h"
#include "Bus_TimetableDlg.h"

#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <time.h>

using namespace std;

class BusTimetable {
public:
    CString busNumber;   // ����
    CTime departureTime; // ����ʱ��
    CString startStation;  // ���վ
    CString endStation;    // �յ�վ
    CString travelTime;     // �г�ʱ��
    int price;              // Ʊ��
    int capacity;              // �����
    int soldTickets;           // ����Ʊ��

    //BusTimetable(string busn = "", CTime departn = (0, 0, 0, 0, 0, 0), string statn = "", string endsn = "", string travn = "", string prin = "", int capn = 0, int soln = 0)
    //    :busNumber(busn), departureTime(departn), startStation(statn), endStation(endsn), travelTime(travn), price(prin), capacity(capn), soldTickets(soln){}
    //~BusTimetable(){}
};

class SharedFunc {
public:
	static int CheckTime(CTime ntime);
	static void ReadDataFromFile(const string& filename, vector<BusTimetable>& TimetableData);
	static bool SaveDataIntoFile(const string& filename, vector<BusTimetable> TimetableData);
	static void DisplayTimetableData(CListCtrl* m_lisCtrl, vector<BusTimetable> g_TimetableData);

    static vector<BusTimetable> SearchBus(int SearchType, CString m_searchEdit);

    static bool CheckLogFileExists();
};

//extern BOOL m_bSortAscending;    //ȫ�ֱ�����ʾ������
extern vector<BusTimetable> g_TimetableData;
extern vector<BusTimetable> backup;
extern vector<BusTimetable> v_TimetableData;

extern const string key;

extern bool isLogged;