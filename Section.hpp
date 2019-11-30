#ifndef SECTION
#define SECTION

#include <string>           //string
#include <vector>           //vector
#include "json.hpp"         //nlohmann::json

/*
Describes a section

CONSTRAINTS:
1. Section must not have more than one labs in a day
2. Section must not have more than 6 consecutive classes
3. Section must not have more than 7 classes in a day
4. Section must not have a day off except for seniors
5. Section must not have multiple lectures at the same time
*/
class Section{

    //creates C++ Section object from json Section object
    friend void from_json(const nlohmann::json&, Section&);

    public:

        const std::string& getId() const {return id;}

        int getBatch() const {return batch;}

        const std::string& getDepartment() const {return department;}

        const std::vector<std::string>& getLectures() const {return lectures;}

        void addLecture(std::string lecture){lectures.push_back(lecture);}

    private:

        std::string id;
        int         batch;
        std::string department;

        std::vector<std::string> lectures;

};

void from_json(const nlohmann::json& jsection, Section& section) {
	jsection.at("id").get_to(section.id);
	jsection.at("batch").get_to(section.batch);
    jsection.at("department").get_to(section.department);
}

#endif