#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include "gmock/gmock.h"

using namespace std;


const string ATTENDANCE_FILE_NAME = "attendance_weekday_500.txt";
const int ATTENDANCE_DATA_SIZE = 500;

enum DayOfWeek {
	MONDAY = 0,
	TUESDAY,
	WEDNESDAY,
	THURSDAY,
	FRIDAY,
	SATURDAY,
	SUNDAY
};

const int POINT_FOR_GOLD_GRADE = 50;
const int POINT_FOR_SILVER_GRADE = 30;

enum Grade {
	GRADE_NORMAL = 0,
	GRADE_GOLD = 1,
	GRADE_SILVER = 2,
};

#define MAX_ID_CNT 100
#define MAX_DAY_OF_WEEK 7

const int ATTENDANCE_BONUS_DAYS = 10;
const int ATTENDANCE_BONUS_POINT = 10;

map<string, int> idMapOfName;

int countDayOfWeek[MAX_ID_CNT][MAX_DAY_OF_WEEK];
int points[MAX_ID_CNT];
int grade[MAX_ID_CNT];
string names[MAX_ID_CNT];

int countOfWednesday[MAX_ID_CNT];
int countOfweekend[MAX_ID_CNT];


void registerID(string name) {
	if (idMapOfName.count(name) == 0) {
		idMapOfName.insert({ name, idMapOfName.size() + 1 });
		names[idMapOfName.size()] = name;
	}
}

int getPointOfDayIndex(int indexDayOfWeek) {
	switch (indexDayOfWeek) {
		case MONDAY:
		case TUESDAY:
		case THURSDAY:
		case FRIDAY:
			return 1;
		case WEDNESDAY:
			return 3;
		case SATURDAY:
		case SUNDAY:
			return 2;
		default:
			return 0;
	}
}

int getIndexOfDay(string dayOfWeek) {
	if (dayOfWeek == "monday") return MONDAY;
	if (dayOfWeek == "tuesday") return TUESDAY;
	if (dayOfWeek == "wednesday") return WEDNESDAY;
	if (dayOfWeek == "thursday") return THURSDAY;
	if (dayOfWeek == "friday") return FRIDAY;
	if (dayOfWeek == "saturday") return SATURDAY;
	if (dayOfWeek == "sunday") return SUNDAY;
	return -1;
}

void registerDayOfWeek(string name, string dayOfWeek) {
	int id = idMapOfName[name];
	int index = getIndexOfDay(dayOfWeek);
	if (index == -1) return;

	countDayOfWeek[id][index]++;

	if (dayOfWeek == "wednesday") {
		countOfWednesday[id]++;
	}
	
	if (dayOfWeek == "saturday" || dayOfWeek == "sunday") {
		countOfweekend[id]++;
	}
}

void calculateAttendancePoints() {
	for (int id = 1; id <= idMapOfName.size(); id++) {
		for (int indexDayOfWeek = 0; indexDayOfWeek < MAX_DAY_OF_WEEK; indexDayOfWeek++) {
			points[id] += countDayOfWeek[id][indexDayOfWeek] * getPointOfDayIndex(indexDayOfWeek);
		}
	}
}

void registerAttendanceData(string inputFileName, int size)
{
	ifstream fin{ inputFileName };
	for (int i = 0; i <size ; i++) {
		string name, dayOfWeek;
		fin >> name >> dayOfWeek;
		registerID(name);
		registerDayOfWeek(name, dayOfWeek);
	}
}

void calculateBonusPoints() {
	for (int id = 1; id <= idMapOfName.size(); id++) {
		if (countDayOfWeek[id][2] >= ATTENDANCE_BONUS_DAYS) {
			points[id] += ATTENDANCE_BONUS_POINT;
		}

		if (countDayOfWeek[id][5] + countDayOfWeek[id][6] >= ATTENDANCE_BONUS_DAYS) {
			points[id] += ATTENDANCE_BONUS_POINT;
		}
	}
}

void calculateGrade() {
	for (int id = 1; id <= idMapOfName.size(); id++) {
		if (points[id] >= POINT_FOR_GOLD_GRADE) {
			grade[id] = GRADE_GOLD;
		}
		else if (points[id] >= POINT_FOR_SILVER_GRADE) {
			grade[id] = GRADE_SILVER;
		}
		else {
			grade[id] = GRADE_NORMAL;
		}
	}
}

void printAttendanceDataWithGrade() {
	for (int id = 1; id <= idMapOfName.size(); id++) {
		cout << "NAME : " << names[id] << ", ";
		cout << "POINT : " << points[id] << ", ";
		cout << "GRADE : ";

		if (grade[id] == GRADE_GOLD) cout << "GOLD" << "\n";
		if (grade[id] == GRADE_SILVER) cout << "SILVER" << "\n";
		else cout << "NORMAL" << "\n";

	}
}

void printRemovedPlayer() {
	std::cout << "\n";
	std::cout << "Removed player\n";
	std::cout << "==============\n";
	for (int id = 1; id <= idMapOfName.size(); id++) {

		if (grade[id] != 1 && grade[id] != 2 && countOfWednesday[id] == 0 && countOfweekend[id] == 0) {
			std::cout << names[id] << "\n";
		}
	}
}

void analyzeAttendanceData() {
	registerAttendanceData(ATTENDANCE_FILE_NAME, ATTENDANCE_DATA_SIZE);

	calculateAttendancePoints();
	calculateBonusPoints();

	calculateGrade();

	printAttendanceDataWithGrade();	
	printRemovedPlayer();
}

int main() {
#ifdef _DEBUG
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
#else
	analyzeAttendanceData();
#endif
	
}