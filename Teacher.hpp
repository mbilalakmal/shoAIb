#ifndef TEACHER
#define TEACHER

#include <string>           //string
#include <vector>           //vector
#include <unordered_map>    //unordered_map
#include "json.hpp"         //nlohmann::json

typedef std::vector<std::vector<bool>> AVAILABOOL;

/*
Describes a teacher

CONSTRAINTS:
1. Day, Room, & Time availability can be specified
2. Teacher must not have more than 3 consecutive classes
3. Teacher must not have more than 4 classes in a day
4. Teacher must have a day off
5. Teacher must not have multiple lectures at the same time
*/
class Teacher{

    //creates C++ Teacher object from json Teacher object
    friend void from_json(const nlohmann::json&, Teacher&);

    public:

        const std::string& getId() const {return id;}

        const std::string& getDepartment() const {return department;}

        const std::vector<std::string>& getLectures() const {return lectures;}

        const AVAILABOOL& getAvailableSlots() const {return availableSlots;}

        const std::vector<std::string>& getAvailableRooms() const {return availableRooms;}

        void addLecture(std::string lecture){lectures.push_back(lecture);}

    private:

        std::string id;
        std::string department;

        std::vector<std::string> lectures;

        AVAILABOOL availableSlots;
        std::vector<std::string> availableRooms;

};

void from_json(const nlohmann::json& jteacher, Teacher& teacher) {
	jteacher.at("id").get_to(teacher.id);
	jteacher.at("department").get_to(teacher.department);
	jteacher.at("availableSlots").get_to(teacher.availableSlots);
	jteacher.at("availableRooms").get_to(teacher.availableRooms);

    std::sort(teacher.availableRooms.begin(), teacher.availableRooms.end());
}

#endif