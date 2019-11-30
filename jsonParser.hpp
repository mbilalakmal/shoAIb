#ifndef JSONPARSER
#define JSONPARSER

#include <string>           //string
#include <vector>           //vector
#include <unordered_map>    //unordered_map
#include <fstream>          //ifstream
#include "json.hpp"         //nlohmann::json

#include "room.hpp"
#include "course.hpp"
#include "teacher.hpp"
#include "section.hpp"
#include "lecture.hpp"

class JSONParser{

    public:

        JSONParser(std::string fileName = "data-sample.json"){
            std::ifstream jsonFile(fileName);
            everything = nlohmann::json::parse(jsonFile);

            jrooms = everything.at("rooms");
            jcourses = everything.at("courses");
            jteachers = everything.at("teachers");
            jsections = everything.at("sections");
            jlectures = everything.at("entries");
            jconstraints = everything.at("constraints");

            deserialize();

        }

        ~JSONParser(){
            everything.clear(); jrooms.clear(); jcourses.clear();
            jteachers.clear(); jsections.clear(); jlectures.clear(); jconstraints.clear();

            rooms.clear(); courses.clear(); teachers.clear();
            sections.clear(); lectures.clear(); electives.clear();
        }

    private:

        //populates 'rooms' map from json objects
        void decodeRooms(){
            //creating rooms
            for(const auto& jroom: jrooms){
                rooms.insert(
                    std::make_pair( jroom.at("id"), jroom.get<Room>() )
                );
            }
        }

        //populates 'courses' map from json objects
        void decodeCourses(){
            //creating courses
            for(const auto& jcourse: jcourses){
                courses.insert(
                    std::make_pair( jcourse.at("id"), jcourse.get<Course>() )
                );
            }
        }

        //populates 'teachers' map from json objects
        void decodeTeachers(){
            //creating teachers
            for(const auto& jteacher: jteachers){
                teachers.insert(
                    std::make_pair( jteacher.at("id"), jteacher.get<Teacher>() )
                );
            }
        }

        //populates 'sections' map from json objects
        void decodeSections(){
            //creating sections
            for(const auto& jsection: jsections){
                sections.insert(
                    std::make_pair( jsection.at("id"), jsection.get<Section>() )
                );
            }
        }

        //populates 'lectures' map from json objects
        void decodeLectures(){
            //creating lectures
            for(const auto& jlecture: jlectures){
                lectures.insert(
                    std::make_pair( jlecture.at("id"), jlecture.get<Lecture>() )
                );
            }
        }

        //populates 'electives' map from json objects
        void decodeElectives(){
            //creating elective pairs
            for(const auto& jconstraint: jconstraints){
                std::vector<std::string>  pairs = jconstraint.at("pairedCourses");
                electives.insert(
                    std::make_pair( jconstraint.at("id"), pairs )
                );
            }
        }

        //use course & teachers to determine lectures' availability
        void setLecturesAvailability(){

            for(auto& lecturePair: lectures){

                Lecture& lecture = lecturePair.second;
                // const Course& course = courses.at( lecture.course );
                const Course& course = courses.at( lecture.getCourse() );

                //temporary vars assigned to lecture later
                auto lectureSlots = course.getAvailableSlots();
                auto lectureRooms = course.getAvailableRooms();

                //intersection with each teacher
                for(const auto& teacherID: lecture.getTeachers()){

                    const Teacher& teacher = teachers.at(teacherID);

                    //get teacher's available slots&rooms (copy instead of ref)
                    auto teacherSlots = teacher.getAvailableSlots();
                    auto teacherRooms = teacher.getAvailableRooms();

                    //for each day, perform AND for each slot
                    for(int i = 0; i < teacherSlots.size(); i++){
                        //perform bit-wise AND
                        std::transform(
                        teacherSlots.at(i).begin(), teacherSlots.at(i).end(),
                        lectureSlots.at(i).begin(), lectureSlots.at(i).begin(),
                        std::logical_and<>()
                        );
                    }

                    //move into temp because set_intersection works out-of-place only
                    auto temporaryRooms = std::move(lectureRooms);
                    lectureRooms.clear();   //no-op because it will be empty after move in most imp

                    //only keep rooms available for this teacher
                    std::set_intersection(
                        temporaryRooms.begin(), temporaryRooms.end(), teacherRooms.begin(),
                        teacherRooms.end(), std::back_inserter( lectureRooms )
                    );

                }

                lecture.setAvailableSlots( lectureSlots );
                lecture.setAvailableRooms( lectureRooms );

            }

        }

        //add lectureID to teacher(s) and section(s)
        void reflectLectureAssociation(){

            for(const auto& lecturePair: lectures){

                const Lecture& lecture = lecturePair.second;

                //add lecture to all included teachers
                for(const auto& teacherID: lecture.getTeachers()){
                    Teacher& teacher = teachers.at( teacherID );
                    teacher.addLecture( lecture.getId() );
                }

                //add lecture to all included sections
                for(const auto& sectionID: lecture.getSections()){
                    Section& section = sections.at( sectionID );
                    section.addLecture( lecture.getId() );
                }

            }

        }

        //add elective pairs ID to courses where applicble
        void addElectivePairs(){

            for(const auto& electivePair: electives){

                std::string pairID = electivePair.first;

                for(const auto& courseID: electivePair.second){
                    Course& course = courses.at( courseID );
                    course.addElectivePair( pairID );
                }
            }
        }

        void deserialize(){

            decodeRooms();
            decodeCourses();
            decodeTeachers();
            decodeSections();
            decodeLectures();

            reflectLectureAssociation();
            setLecturesAvailability();
            decodeElectives();
        }

        nlohmann::json everything, jrooms, jcourses,
        jteachers, jsections, jlectures, jconstraints;

    public:

        std::unordered_map<std::string, Room> rooms;
        std::unordered_map<std::string, Course> courses;
        std::unordered_map<std::string, Teacher> teachers;
        std::unordered_map<std::string, Section> sections;
        std::unordered_map<std::string, Lecture> lectures;
        std::unordered_map<std::string, std::vector<std::string>> electives;

};

#endif