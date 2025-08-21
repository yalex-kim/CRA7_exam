#pragma once

#include <string>
#include <vector>
#include <map>

#include "policy.h"

using namespace std;

#define MAX_ID_CNT 100
#define MAX_DAY_OF_WEEK 7

class AttendanceManager {
public:
	AttendanceManager(AttendancePolicyInterface* policy) : policy{ policy } {};

	bool isInvalidID(int id);
	bool isNotRegisteredName(string name);
	//bool isEliminationCandidate(int id); // policy
	int getIDByName(string name);
	int getPointsByName(string name);
	int getGradeByName(string name);
	int getCountOfID();
	int getCountDayOfWeek(string name, int indexDayOfWeek);

	void clearAttendanceData();
	void registerID(string name);
	//int getPointOfDayIndex(int indexDayOfWeek); // policy
	int getIndexOfDay(string dayOfWeek);
	void registerDayOfWeek(string name, string dayOfWeek);
	void calculateAttendancePoints();
	void registerAttendanceDataFromFile(string inputFileName, int size);
	void registerUnitAttendanceData(string name, string dayOfWeek);
	void calculateBonusPoints();
	void calculateGrades();
	void printAttendanceDataWithGrade();
	void printRemovedPlayer();
	void analyzeAttendanceData();
private:
	AttendancePolicyInterface* policy;
	map<string, int> idMapOfName;

	int countDayOfWeek[MAX_ID_CNT][MAX_DAY_OF_WEEK];
	int points[MAX_ID_CNT];
	int grade[MAX_ID_CNT];
	string names[MAX_ID_CNT];

	int countOfWednesday[MAX_ID_CNT];
	int countOfweekend[MAX_ID_CNT];

};
