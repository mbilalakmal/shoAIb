#ifndef COURSE
#define COURSE

#include<vector>
using namespace std;

/*
Describes a course offered with 9 attributes including
availableSlots for course according to requirements & policies

CONSTRAINTS:
1. Day, Room, & Time assigned can be specified
2. Lab courses should have continuous slots
3. Theory courses should be split across days
4. Theory courses should be schedule before their
	lab courses for first-year
5. Repeat course can be scheduled alongside thier
	successor courses (from the first to last)
6. Elective courses offered in pairs can be scheduled
	alongside each other
*/
class Course{

	public:
		
		Course(
			const string&	courseCode,
			const string&	department,
			int				offeredToBatch,
			const string&	school,
			int				duration,
			bool			isCoreCourse,
			bool			isLabCourse,
			bool			isRepeatCourse,
			const vector<bool>&
							availableSlots
		):
			courseCode(courseCode),
			department(department),
			offeredToBatch(offeredToBatch),
			school(school),
			duration(duration),
			isCoreCourse(isCoreCourse),
			isLabCourse(isLabCourse),
			isRepeatCourse(isRepeatCourse),
			availableSlots(availableSlots)
		{}
		
		const string& getCourseCode() const {return courseCode;}

		const string& getDepartment() const {return department;}

		int getOfferedToBatch() const {return offeredToBatch;}
		
		const string& getSchool() const {return school;}

		int getDuration() const {return duration;}

		bool getIsCoreCourse() const {return isCoreCourse;}

		bool getIsLabCourse() const { return isLabCourse;}

		bool getIsRepeatCourse() const { return isRepeatCourse;}

		bool getAvailableSlot(int position) const {return availableSlots[position];}

	private:
		
		const string	courseCode;
		const string	department;
		const int		offeredToBatch;
		const string	school;
		
		const int		duration;
		
		const bool		isCoreCourse;
		const bool		isLabCourse;
		const bool		isRepeatCourse;

		const vector<bool>	availableSlots;
	
};

#endif
