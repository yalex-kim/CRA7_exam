#include <iostream>
#include <fstream>
#include <algorithm>
#include "attendance.h"

using namespace std;

void AttendanceManager::clearAttendanceData() {
	idMapOfName.clear();
	for (int i = 0; i < MAX_ID_CNT; i++) {
		for (int j = 0; j < MAX_DAY_OF_WEEK; j++) {
			countDayOfWeek[i][j] = 0;
		}
		points[i] = 0;
		grade[i] = GRADE_NORMAL;
		names[i] = "";
		countOfWednesday[i] = 0;
		countOfweekend[i] = 0;
	}
}

void AttendanceManager::registerID(string name) {
	if (idMapOfName.count(name) == 0) {
		idMapOfName.insert({ name, idMapOfName.size() + 1 });
		names[idMapOfName.size()] = name;
	}
}

int AttendanceManager::getPointOfDayIndex(int indexDayOfWeek) {
	if (indexDayOfWeek == MONDAY || indexDayOfWeek == TUESDAY || indexDayOfWeek == THURSDAY || indexDayOfWeek == FRIDAY) {
		return 1;
	}

	if (indexDayOfWeek == WEDNESDAY) {
		return 3;
	}

	if (indexDayOfWeek == SATURDAY || indexDayOfWeek == SUNDAY) {
		return 2;
	}
}

int AttendanceManager::getIndexOfDay(string dayOfWeek) {
	if (dayOfWeek == "monday") return MONDAY;
	if (dayOfWeek == "tuesday") return TUESDAY;
	if (dayOfWeek == "wednesday") return WEDNESDAY;
	if (dayOfWeek == "thursday") return THURSDAY;
	if (dayOfWeek == "friday") return FRIDAY;
	if (dayOfWeek == "saturday") return SATURDAY;
	if (dayOfWeek == "sunday") return SUNDAY;
	return -1;
}

bool AttendanceManager::isInvalidID(int id) {
	return (id < 1 || id > MAX_ID_CNT);
}

bool AttendanceManager::isNotRegisteredName(string name) {
	return idMapOfName.count(name) == 0;
}

int AttendanceManager::getIDByName(string name) {
	if (isNotRegisteredName(name)) return -1;
	return idMapOfName[name];

}

int AttendanceManager::getPointsByName(string name) {
	int id = getIDByName(name);
	if (isInvalidID(id)) return -1;
	return points[id];
}

int AttendanceManager::getGradeByName(string name) {
	int id = getIDByName(name);
	if (isInvalidID(id)) return -1;
	return grade[id];
}

int AttendanceManager::getCountOfID() {
	return (int)idMapOfName.size();
}

void AttendanceManager::registerDayOfWeek(string name, string dayOfWeek) {
	int id = getIDByName(name);
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

int AttendanceManager::getCountDayOfWeek(string name, int indexDayOfWeek) {
	int id = getIDByName(name);
	if (id < 1 || id >= MAX_ID_CNT) return 0;
	return countDayOfWeek[id][indexDayOfWeek];
}

void AttendanceManager::calculateAttendancePoints() {
	for (int id = 1; id <= idMapOfName.size(); id++) {
		for (int indexDayOfWeek = 0; indexDayOfWeek < MAX_DAY_OF_WEEK; indexDayOfWeek++) {
			points[id] += countDayOfWeek[id][indexDayOfWeek] * getPointOfDayIndex(indexDayOfWeek);
		}
	}
}

void AttendanceManager::registerUnitAttendanceData(string name, string dayOfWeek) {
	registerID(name);
	registerDayOfWeek(name, dayOfWeek);
}

void AttendanceManager::registerAttendanceDataFromFile(string inputFileName, int size)
{
	ifstream fin{ inputFileName };
	for (int i = 0; i <size ; i++) {
		string name, dayOfWeek;
		fin >> name >> dayOfWeek;
		registerUnitAttendanceData(name, dayOfWeek);
	}
}

void AttendanceManager::calculateBonusPoints() {
	for (int id = 1; id <= idMapOfName.size(); id++) {
		if (countDayOfWeek[id][2] >= ATTENDANCE_BONUS_DAYS) {
			points[id] += ATTENDANCE_BONUS_POINT;
		}

		if (countDayOfWeek[id][5] + countDayOfWeek[id][6] >= ATTENDANCE_BONUS_DAYS) {
			points[id] += ATTENDANCE_BONUS_POINT;
		}
	}
}

void AttendanceManager::calculateGrade() {
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

void AttendanceManager::printAttendanceDataWithGrade() {
	for (int id = 1; id <= idMapOfName.size(); id++) {
		cout << "NAME : " << names[id] << ", ";
		cout << "POINT : " << points[id] << ", ";
		cout << "GRADE : ";

		if (grade[id] == GRADE_GOLD) cout << "GOLD" << "\n";
		else if (grade[id] == GRADE_SILVER) cout << "SILVER" << "\n";
		else cout << "NORMAL" << "\n";

	}
}

void AttendanceManager::printRemovedPlayer() {
	std::cout << "\n";
	std::cout << "Removed player\n";
	std::cout << "==============\n";
	for (int id = 1; id <= idMapOfName.size(); id++) {

		if (grade[id] != 1 && grade[id] != 2 && countOfWednesday[id] == 0 && countOfweekend[id] == 0) {
			std::cout << names[id] << "\n";
		}
	}
}

void AttendanceManager::analyzeAttendanceData() {
	calculateAttendancePoints();
	calculateBonusPoints();

	calculateGrade();

	printAttendanceDataWithGrade();	
	printRemovedPlayer();
}
