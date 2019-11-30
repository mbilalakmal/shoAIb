#ifndef LECTURE
#define LECTURE

#include <string>           //string
#include <vector>           //vector
#include <unordered_map>    //unordered_map
#include "json.hpp"         //nlohmann::json

typedef std::vector<std::vector<bool>> AVAILABOOL;

/*
Describes a lecture : a course, teacher(s), and section(s)
*/
class Lecture{

    //creates C++ Lecture object from json Lecture object
    friend void from_json(const nlohmann::json&, Lecture&);

    public:

        const std::string& getId() const {return id;}

		int getStrength() const {return strength;}
		
		const std::string& getCourse() const {return course;}

		const std::vector<std::string>& getTeachers() const {return teachers;}

		const std::vector<std::string>& getSections() const {return sections;}

        const AVAILABOOL& getAvailableSlots() const {return availableSlots;}

        const std::vector<std::string>& getAvailableRooms() const {return availableRooms;}

        void setAvailableSlots(AVAILABOOL availableSlots){this->availableSlots = availableSlots;}

        void setAvailableRooms(std::vector<std::string> availableRooms){this->availableRooms = availableRooms;}

    private:

        std::string id;
        int         strength;

        std::string course;
        std::vector<std::string> teachers;
        std::vector<std::string> sections;

        AVAILABOOL availableSlots;
        std::vector<std::string> availableRooms;

};

void from_json(const nlohmann::json& jlecture, Lecture& lecture) {
	jlecture.at("id").get_to(lecture.id);
	jlecture.at("strength").get_to(lecture.strength);
    jlecture.at("courseId").get_to(lecture.course);
    jlecture.at("teacherIds").get_to(lecture.teachers);
    jlecture.at("sectionIds").get_to(lecture.sections);
}

#endif