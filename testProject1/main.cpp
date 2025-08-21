#include "gmock/gmock.h"
#include "attendance.h"

const string ATTENDANCE_FILE_NAME = "attendance_weekday_500.txt";
const int ATTENDANCE_DATA_SIZE = 500;

int main () {
#ifdef _DEBUG
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
#else
	AttendanceManager manager{ ATTENDANCE_FILE_NAME, ATTENDANCE_DATA_SIZE };
	manager.analyzeAttendanceData();
	return 0;
#endif
}
