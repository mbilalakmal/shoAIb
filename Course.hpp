#ifndef COURSE
#define COURSE

#include <string>           //string
#include <vector>           //vector
#include <unordered_map>    //unordered_map
#include "json.hpp"         //nlohmann::json

typedef std::vector<std::vector<bool>> AVAILABOOL;

/*
Describes a course offered

CONSTRAINTS:
1. Day, Room, & Time availability can be specified
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

    //creates C++ Course object from json Course object
    friend void from_json(const nlohmann::json&, Course&);

    public:

        const std::string& getId() const {return id;}

        const std::string& getCourseCode() const {return courseCode;}

        const std::string& getDepartment() const {return department;}

        int getDuration() const {return duration;}

        bool getIsCoreCourse() const {return isCoreCourse;}

        bool getIsLabCourse() const {return isLabCourse;}

        const std::vector<std::string>& getPrerequisiteIds() const {return prerequisiteIds;}

        const std::vector<std::string>& getElectivePairIds() const {return electivePairIds;}

        const std::string& getTheoryCourseId() const {return theoryCourseId;}

        const AVAILABOOL& getAvailableSlots() const {return availableSlots;}

        const std::vector<std::string>& getAvailableRooms() const {return availableRooms;}

        void addElectivePair(std::string pair){electivePairIds.push_back(pair);}

    private:

        std::string id;
        std::string courseCode;
        std::string department;
        int duration;

        bool isCoreCourse;
        bool isLabCourse;

        std::vector<std::string> prerequisiteIds;
        std::vector<std::string> electivePairIds;
        std::string theoryCourseId;

        AVAILABOOL availableSlots;
        std::vector<std::string> availableRooms;

};

void from_json(const nlohmann::json& jcourse, Course& course) {
	jcourse.at("id").get_to(course.id);
	jcourse.at("courseCode").get_to(course.courseCode);
	jcourse.at("department").get_to(course.department);
    jcourse.at("duration").get_to(course.duration);

	jcourse.at("theoryCourseId").get_to(course.theoryCourseId);
    jcourse.at("prerequisiteIds").get_to(course.prerequisiteIds);

    course.isLabCourse = ! course.theoryCourseId.empty();
    jcourse.at("isCoreCourse").get_to(course.isCoreCourse);
	jcourse.at("availableSlots").get_to(course.availableSlots);
	jcourse.at("availableRooms").get_to(course.availableRooms);

    std::sort(course.availableRooms.begin(), course.availableRooms.end());
}

#endif