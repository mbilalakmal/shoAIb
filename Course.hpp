#ifndef COURSE
#define COURSE

#include<vector>
using namespace std;

#include"json.hpp"
using nlohmann::json;

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
	successor courses
6. Elective courses offered in pairs can be scheduled
	alongside each other
*/
class Course{

	/*
	these methods are used when creating course objects from json.
	The course must have a default constructor for this purpose
	*/
	friend void to_json(json& , const Course&);
	friend void from_json(const json&, Course&);

	public:
		
		const string& getId() const {return id;}

		const string& getCourseCode() const {return courseCode;}

		const string& getDepartment() const {return department;}

		int getBatch() const {return batch;}
		
		const string& getSchool() const {return school;}

		int getDuration() const {return duration;}

		bool getIsCoreCourse() const {return isCoreCourse;}

		bool getIsLabCourse() const { return isLabCourse;}

		bool getIsRepeatCourse() const { return isRepeatCourse;}

		bool getAvailableSlot(int position) const {return availableSlots[position];}

	private:
		
		string	id;
		string	courseCode;
		string	department;
		int		batch;
		string	school;
		
		int		duration;
		
		bool	isCoreCourse;
		bool	isLabCourse;
		bool	isRepeatCourse;

		vector<bool> availableSlots;
	
};

void to_json(json& j, const Course& course) {
	j = json{
		{"id", course.id},
		{"courseCode", course.courseCode},
		{"department", course.department},
		{"batch", course.batch},

		{"duration", course.duration},

		{"isCoreCourse", course.isCoreCourse},
		// {"isLabCourse", course.isLabCourse},
		{"isRepeatCourse", course.isRepeatCourse},

		{"availableSlots", course.availableSlots},
	};
}

void from_json(const json& j, Course& course) {	
	j.at("id").get_to(course.id);
	j.at("courseCode").get_to(course.courseCode);
	j.at("department").get_to(course.department);
	j.at("batch").get_to(course.batch);

	j.at("duration").get_to(course.duration);

	j.at("isCoreCourse").get_to(course.isCoreCourse);
	// j.at("isLabCourse").get_to(course.isLabCourse);
	j.at("isRepeatCourse").get_to(course.isRepeatCourse);

	j.at("availableSlots").get_to(course.availableSlots);
}

#endif
