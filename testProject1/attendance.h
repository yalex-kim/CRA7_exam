#pragma once

#include <string>
#include <vector>
#include <map>

using namespace std;

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

class AttendanceManager {
public:
	AttendanceManager(string inputFileName, int size) {
		registerAttendanceDataFromFile(inputFileName, size);
	}

	AttendanceManager() {
		clearAttendanceData();
	}

	bool isInvalidID(int id);
	bool isNotRegisteredName(string name);
	int getIDByName(string name);
	int getPointsByName(string name);
	int getGradeByName(string name);
	int getCountOfID();
	int getCountDayOfWeek(string name, int indexDayOfWeek);

	void clearAttendanceData();
	void registerID(string name);
	int getPointOfDayIndex(int indexDayOfWeek);
	int getIndexOfDay(string dayOfWeek);
	void registerDayOfWeek(string name, string dayOfWeek);
	void calculateAttendancePoints();
	void registerAttendanceDataFromFile(string inputFileName, int size);
	void registerUnitAttendanceData(string name, string dayOfWeek);
	void calculateBonusPoints();
	void calculateGrade();
	void printAttendanceDataWithGrade();
	void printRemovedPlayer();
	void analyzeAttendanceData();
private:
	map<string, int> idMapOfName;

	int countDayOfWeek[MAX_ID_CNT][MAX_DAY_OF_WEEK];
	int points[MAX_ID_CNT];
	int grade[MAX_ID_CNT];
	string names[MAX_ID_CNT];

	int countOfWednesday[MAX_ID_CNT];
	int countOfweekend[MAX_ID_CNT];

};
