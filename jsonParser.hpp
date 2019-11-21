#ifndef JSONPARSER
#define JSONPARSER

#include<string>
#include<fstream>

#include"Room.hpp"
#include"Lecture.hpp"

#include"json.hpp"
using nlohmann::json;

class JSONParser{
    public:

        unordered_map<int, Room*> rooms;
        unordered_map<string, Course*> courses;
        unordered_map<string, Teacher*> teachers;
        unordered_map<string, StudentSection*> sections;
        vector<Lecture*> lectures;

        void parseFile(std::string fileName = "data-sample.json"){
            std::ifstream jsonFile(fileName);
            object = json::parse(jsonFile);

            jRooms = object.at("rooms");
            jCourses = object.at("courses");
            jTeachers = object.at("teachers");
            jSections = object.at("sections");
            jLectures = object.at("entries");
            jConstraints = object.at("constraints");

        }

        void deserialize(){

            for(const auto& jroom: jRooms){
                Room* room = new Room(jroom.get<Room>());
                rooms.insert(
                    make_pair( room->getId(), room )
                    );
            }

            for(const auto& jcourse: jCourses){
                Course* course = new Course(jcourse.get<Course>());
                courses.insert(
                    make_pair( course->getId(), course )
                    );
            }

            for(const auto& jteacher: jTeachers){
                Teacher* teacher = new Teacher(jteacher.get<Teacher>());
                teachers.insert(
                    make_pair( teacher->getId(), teacher )
                    );
            }

            for(const auto& jsection: jSections){
                StudentSection* section = new StudentSection(jsection.get<StudentSection>());
                sections.insert(
                    make_pair( section->getId(), section )
                    );
            }

            for(const auto& jlecture: jLectures){
                string lectureID =  jlecture.at("id");
                string courseID =   jlecture.at("courseId");

                vector<string> sectionIDs =    jlecture.at("sectionIds");
                vector<string> teacherIDs =    jlecture.at("teacherIds");

                Course* course = courses.at(courseID);
                Lecture* lecture = new Lecture(course, lectureID);

                for(const auto& sectionID: sectionIDs){
                    StudentSection* section = sections.at(sectionID);
                    lecture->addSection(section);
                }

                for(const auto& teacherID: teacherIDs){
                    Teacher* teacher = teachers.at(teacherID);
                    lecture->addTeacher(teacher);
                }

                lectures.push_back( lecture );

            }

        }

    private:

        json object, jRooms, jCourses,
        jTeachers, jSections, jLectures, jConstraints;

};

#endif