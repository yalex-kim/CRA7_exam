#include "gmock/gmock.h"
#include "attendance.h"

#include <iostream>
#include <fstream>

using namespace testing;

const string ATTENDANCE_FILE_NAME = "attendance_weekday_500.txt";
const int ATTENDANCE_DATA_SIZE = 500;

class AttendanceAnalyzerTest : public ::testing::Test {
protected:
	AttendancePolicyInterface* policy = new AttendancePolicy();
	AttendanceManager attendManager{policy};
	void SetUp() override {
		attendManager.clearAttendanceData();
	}
	void TearDown() override {
	}
};

class ifstreamMock : public std::ifstream {
public:
	ifstreamMock(const std::string& filename) : std::ifstream(filename) {};
	MOCK_METHOD(void, open, (const std::string& filename));
	MOCK_METHOD(void, close, ());
	//MOCK_METHOD(std::ifstream&, operator>>(std::istream&), (override));	
};

TEST_F(AttendanceAnalyzerTest, registerID) {
	attendManager.registerID("Alice");
	attendManager.registerID("Bob");

	EXPECT_EQ(attendManager.getCountOfID(), 2);
	EXPECT_EQ(attendManager.getIDByName("Alice"), 1);
	EXPECT_EQ(attendManager.getIDByName("Bob"), 2);
}

TEST_F(AttendanceAnalyzerTest, registerIDException) {
	attendManager.registerID("Alice");
	attendManager.registerID("Bob");

	attendManager.registerID("Alice");

	EXPECT_EQ(attendManager.getCountOfID(), 2);
	EXPECT_EQ(attendManager.getIDByName("Alice"), 1);
	EXPECT_EQ(attendManager.getIDByName("Bob"), 2);
}

TEST_F(AttendanceAnalyzerTest, getIDByNameException) {
	attendManager.registerID("Alice");
	attendManager.registerID("Bob");

	EXPECT_EQ(attendManager.getIDByName("Charlie"), -1);
}

TEST_F(AttendanceAnalyzerTest, registerDayOfWeek) {
	attendManager.registerID("Alice");
	attendManager.registerID("Bob");
	attendManager.registerID("Charlie");
	attendManager.registerID("David");
	attendManager.registerID("Eve");
	attendManager.registerID("Frank");
	attendManager.registerID("Grace");

	attendManager.registerDayOfWeek("Alice", "monday");
	attendManager.registerDayOfWeek("Bob", "tuesday");
	attendManager.registerDayOfWeek("Charlie", "wednesday");
	attendManager.registerDayOfWeek("David", "thursday");
	attendManager.registerDayOfWeek("Eve", "friday");
	attendManager.registerDayOfWeek("Frank", "saturday");
	attendManager.registerDayOfWeek("Grace", "sunday");

	EXPECT_EQ(attendManager.getCountOfID(), 7);
	EXPECT_EQ(attendManager.getCountDayOfWeek("Alice", MONDAY), 1);
	EXPECT_EQ(attendManager.getCountDayOfWeek("Bob", TUESDAY), 1);
	EXPECT_EQ(attendManager.getCountDayOfWeek("Charlie", WEDNESDAY), 1);
	EXPECT_EQ(attendManager.getCountDayOfWeek("David", THURSDAY), 1);
	EXPECT_EQ(attendManager.getCountDayOfWeek("Eve", FRIDAY), 1);
	EXPECT_EQ(attendManager.getCountDayOfWeek("Frank", SATURDAY), 1);
	EXPECT_EQ(attendManager.getCountDayOfWeek("Grace", SUNDAY), 1);

}

TEST_F(AttendanceAnalyzerTest, registerDayOfWeekException) {
	attendManager.registerID("Alice");

	attendManager.registerDayOfWeek("Alice", "invalidDay");

	EXPECT_EQ(attendManager.getCountOfID(), 1);
	EXPECT_THAT(attendManager.getCountDayOfWeek("Alice", MONDAY), Eq(0));
	EXPECT_THAT(attendManager.getCountDayOfWeek("Alice", TUESDAY), Eq(0));
	EXPECT_THAT(attendManager.getCountDayOfWeek("Alice", WEDNESDAY), Eq(0));
	EXPECT_THAT(attendManager.getCountDayOfWeek("Alice", THURSDAY), Eq(0));
	EXPECT_THAT(attendManager.getCountDayOfWeek("Alice", FRIDAY), Eq(0));
	EXPECT_THAT(attendManager.getCountDayOfWeek("Alice", SATURDAY), Eq(0));
	EXPECT_THAT(attendManager.getCountDayOfWeek("Alice", SUNDAY), Eq(0));
}

TEST_F(AttendanceAnalyzerTest, registerUnitAttendanceData) {
	attendManager.registerUnitAttendanceData("Alice", "monday");
	attendManager.registerUnitAttendanceData("Bob", "tuesday");

	EXPECT_THAT(attendManager.getCountDayOfWeek("Alice", MONDAY), Eq(1));
	EXPECT_THAT(attendManager.getCountDayOfWeek("Bob", TUESDAY), Eq(1));
}

TEST_F(AttendanceAnalyzerTest, calculateAttendancePoints) {

	attendManager.registerUnitAttendanceData("Alice", "monday");
	attendManager.registerUnitAttendanceData("Alice", "tuesday");
	attendManager.registerUnitAttendanceData("Alice", "wednesday");
	attendManager.registerUnitAttendanceData("Bob", "thursday");
	attendManager.registerUnitAttendanceData("Bob", "saturday");

	attendManager.calculateAttendancePoints();
	
	EXPECT_EQ(attendManager.getPointsByName("Alice"), 5); // Alice: 1+1+3
	EXPECT_EQ(attendManager.getPointsByName("Bob"), 3); // Bob: 1+2
}


TEST_F(AttendanceAnalyzerTest, calculateBonusPointsForWednesday) {

	attendManager.registerUnitAttendanceData("Alice", "monday");
	attendManager.registerUnitAttendanceData("Alice", "tuesday");
	for(int i = 0; i < 10; i++) {
		attendManager.registerUnitAttendanceData("Alice", "wednesday");
	}

	attendManager.calculateAttendancePoints();
	attendManager.calculateBonusPoints();

	EXPECT_EQ(attendManager.getPointsByName("Alice"), 42); // Alice: 1+1+3
}

TEST_F(AttendanceAnalyzerTest, calculateBonusPointsForWekend) {

	attendManager.registerUnitAttendanceData("Alice", "monday");
	attendManager.registerUnitAttendanceData("Alice", "tuesday");
	for (int i = 0; i < 10; i++) {
		attendManager.registerUnitAttendanceData("Alice", "saturday");
	}

	attendManager.registerUnitAttendanceData("Bob", "monday");
	attendManager.registerUnitAttendanceData("Bob", "tuesday");
	for (int i = 0; i < 10; i++) {
		attendManager.registerUnitAttendanceData("Bob", "sunday");
	}

	attendManager.calculateAttendancePoints();
	attendManager.calculateBonusPoints();

	EXPECT_EQ(attendManager.getPointsByName("Alice"), 32); // Alice: 1+1+3
	EXPECT_EQ(attendManager.getPointsByName("Bob"), 32); // Alice: 1+1+3
}

TEST_F(AttendanceAnalyzerTest, getPointOfDayIndexException) {

	attendManager.registerUnitAttendanceData("Alice", "monday");
	attendManager.registerUnitAttendanceData("Alice", "tuesday");
	attendManager.registerUnitAttendanceData("Alice", "wednesday");
	attendManager.registerUnitAttendanceData("Bob", "thursday");
	attendManager.registerUnitAttendanceData("Bob", "saturday");

	attendManager.calculateAttendancePoints();

	EXPECT_EQ(attendManager.getPointsByName("Alice"), 5); // Alice: 1+1+3
	EXPECT_EQ(attendManager.getPointsByName("Bob"), 3); // Bob: 1+2
}


TEST_F(AttendanceAnalyzerTest, calculateGrade) {

	attendManager.registerUnitAttendanceData("Alice", "monday");
	attendManager.registerUnitAttendanceData("Alice", "tuesday");
	for (int i = 0; i < 20; i++) {
		attendManager.registerUnitAttendanceData("Alice", "wednesday");
	}
	attendManager.registerUnitAttendanceData("Bob", "monday");
	attendManager.registerUnitAttendanceData("Bob", "tuesday");
	for (int i = 0; i < 10; i++) {
		attendManager.registerUnitAttendanceData("Bob", "sunday");
	}

	attendManager.registerUnitAttendanceData("Charlie", "monday");

	attendManager.calculateAttendancePoints();
	attendManager.calculateBonusPoints();

	attendManager.calculateGrades();

	EXPECT_EQ(attendManager.getPointsByName("Alice"), 72);
	EXPECT_EQ(attendManager.getGradeByName("Alice"), GRADE_GOLD);

	EXPECT_EQ(attendManager.getPointsByName("Bob"), 32);
	EXPECT_EQ(attendManager.getGradeByName("Bob"), GRADE_SILVER);

	EXPECT_EQ(attendManager.getPointsByName("Charlie"), 1);
	EXPECT_EQ(attendManager.getGradeByName("Charlie"), GRADE_NORMAL);

}

TEST_F(AttendanceAnalyzerTest, printAttendanceDataWithGrade) {
	attendManager.registerUnitAttendanceData("Alice", "monday");
	attendManager.registerUnitAttendanceData("Alice", "tuesday");
	for (int i = 0; i < 20; i++) {
		attendManager.registerUnitAttendanceData("Alice", "wednesday");
	}
	attendManager.registerUnitAttendanceData("Bob", "monday");
	attendManager.registerUnitAttendanceData("Bob", "tuesday");
	for (int i = 0; i < 10; i++) {
		attendManager.registerUnitAttendanceData("Bob", "sunday");
	}

	attendManager.registerUnitAttendanceData("Charlie", "monday");

	attendManager.calculateAttendancePoints();
	attendManager.calculateBonusPoints();
	attendManager.calculateGrades();
	EXPECT_NO_THROW(attendManager.printAttendanceDataWithGrade());
}

TEST_F(AttendanceAnalyzerTest, printRemovedPlayer) {
	attendManager.registerUnitAttendanceData("Alice", "monday");
	attendManager.registerUnitAttendanceData("Alice", "tuesday");
	for (int i = 0; i < 20; i++) {
		attendManager.registerUnitAttendanceData("Alice", "wednesday");
	}
	attendManager.registerUnitAttendanceData("Bob", "monday");
	attendManager.registerUnitAttendanceData("Bob", "tuesday");
	for (int i = 0; i < 10; i++) {
		attendManager.registerUnitAttendanceData("Bob", "sunday");
	}

	attendManager.registerUnitAttendanceData("Charlie", "monday");

	attendManager.calculateAttendancePoints();
	attendManager.calculateBonusPoints();
	attendManager.calculateGrades();
	EXPECT_NO_THROW(attendManager.printRemovedPlayer());
}

TEST_F(AttendanceAnalyzerTest, analyzeAttendanceData) {
	attendManager.registerUnitAttendanceData("Alice", "monday");
	attendManager.registerUnitAttendanceData("Alice", "tuesday");
	for (int i = 0; i < 20; i++) {
		attendManager.registerUnitAttendanceData("Alice", "wednesday");
	}
	attendManager.registerUnitAttendanceData("Bob", "monday");
	attendManager.registerUnitAttendanceData("Bob", "tuesday");
	for (int i = 0; i < 10; i++) {
		attendManager.registerUnitAttendanceData("Bob", "sunday");
	}
	attendManager.registerUnitAttendanceData("Charlie", "monday");
	EXPECT_NO_THROW(attendManager.analyzeAttendanceData());
}

TEST_F(AttendanceAnalyzerTest, registerAttendanceDataFromFile) {	
	attendManager.registerAttendanceDataFromFile(ATTENDANCE_FILE_NAME, ATTENDANCE_DATA_SIZE);
	attendManager.analyzeAttendanceData();
	EXPECT_EQ(attendManager.getCountOfID(), 19); // Assuming the file has 500 unique names
	EXPECT_EQ(attendManager.getPointsByName("Alice"), 61); // Assuming Alice has 1 point in the file
	EXPECT_EQ(attendManager.getGradeByName("Alice"), GRADE_GOLD); // Assuming Alice has normal grade
}
