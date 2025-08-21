#pragma once

#include <string>

using namespace std;

const int POINT_FOR_GOLD_GRADE = 50;
const int POINT_FOR_SILVER_GRADE = 30;

enum DayOfWeek {
	MONDAY = 0,
	TUESDAY,
	WEDNESDAY,
	THURSDAY,
	FRIDAY,
	SATURDAY,
	SUNDAY
};

enum Grade {
	GRADE_NORMAL = 0,
	GRADE_GOLD = 1,
	GRADE_SILVER = 2,
};

const int ATTENDANCE_BONUS_DAYS = 10;
const int ATTENDANCE_BONUS_POINT = 10;

class AttendancePolicyInterface {
public:
	virtual bool isEliminationCandidate(int grade, int* countDayOfWeek) = 0;
	virtual int getPointForDayOfWeek(int indexDayOfWeek) = 0;
	virtual int getBonusPoint(int* countDayOfWeek) = 0;
	virtual int getGrade(int points) = 0;
	virtual string getGradeString(int grade) = 0;
};

class AttendancePolicy : public AttendancePolicyInterface {
public:

	bool isEliminationCandidate(int grade, int* countDayOfWeek) override {

		if (grade != GRADE_NORMAL) return false;
		if (countDayOfWeek[WEDNESDAY] > 0) return false;
		if (countDayOfWeek[SATURDAY] + countDayOfWeek[SUNDAY] > 0) return false;

		return true;
	}


	int getPointForDayOfWeek(int indexDayOfWeek) override {
		if (indexDayOfWeek == MONDAY || indexDayOfWeek == TUESDAY || indexDayOfWeek == THURSDAY || indexDayOfWeek == FRIDAY) {
			return 1;
		}

		if (indexDayOfWeek == WEDNESDAY) {
			return 3;
		}

		if (indexDayOfWeek == SATURDAY || indexDayOfWeek == SUNDAY) {
			return 2;
		}
		return 0;
	}

	int getBonusPoint(int* countDayOfWeek) override {
		if (countDayOfWeek[WEDNESDAY] >= ATTENDANCE_BONUS_DAYS) {
			return ATTENDANCE_BONUS_POINT;
		}

		if (countDayOfWeek[SATURDAY] + countDayOfWeek[SUNDAY] >= ATTENDANCE_BONUS_DAYS) {
			return ATTENDANCE_BONUS_POINT;
		}
		return 0;
	}

	int getGrade(int points) override {
		if (points >= POINT_FOR_GOLD_GRADE) return GRADE_GOLD;
		if (points >= POINT_FOR_SILVER_GRADE) return GRADE_SILVER;
		return GRADE_NORMAL;
	}

	string getGradeString(int grade) override {
		switch (grade) {
		case GRADE_GOLD: return "GOLD";
		case GRADE_SILVER: return "SILVER";
		default: return "NORMAL";
		}
	}
};