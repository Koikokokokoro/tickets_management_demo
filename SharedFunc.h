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
    CString busNumber;   // 车次
    CTime departureTime; // 发车时间
    CString startStation;  // 起点站
    CString endStation;    // 终点站
    CString travelTime;     // 行车时间
    int price;              // 票价
    int capacity;              // 额定载量
    int soldTickets;           // 已售票数

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

//extern BOOL m_bSortAscending;    //全局变量表示排序方向
extern vector<BusTimetable> g_TimetableData;
extern vector<BusTimetable> backup;
extern vector<BusTimetable> v_TimetableData;

extern const string key;

extern bool isLogged;