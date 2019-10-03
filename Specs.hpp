#ifndef SPECS
#define SPECS

#include<list>
#include<unordered_map>
#include<fstream>

using namespace std;

class Course;
class Room;
class CourseClass;
class Teacher;
class StudentSection;


//describes the specifications of courses, rooms, classes, teachers, and sections
class Specs{
	public:
		
		Specs() : isEmpty(true){}
		~Specs();
		
        //read the file and parse objects
		void parseFile(string&);
		
        //return pointer to a teacher OR NULL if teacher doesn't exist
		Teacher* getTeacherById(int id){
            //auto
			unordered_map<int, Teacher*>:: iterator it =
				teachers.find(id);
			if(it != teachers.end()){	return it->second;}
			else{	return	NULL;}
		}

        //return pointer to a section OR NULL if section doesn't exist
		StudentSection* getSectionById(int id){
            //auto
			unordered_map<int, StudentSection*>:: iterator it =
				sections.find(id);
			if(it != sections.end()){
				return it->second;}
			else{	return	NULL;}
		}
		
        //return pointer to a room OR NULL if room doesn't exist
		Room* getRoomById(int id){
            //auto
			unordered_map<int, Room*>:: iterator it =
				rooms.find(id);
			if(it != rooms.end()){
				return it->second;}
			else{	return	NULL;}
		}
		
        //return pointer to a course OR NULL if course doesn't exist
		Course* getCourseById(int id){ 
            //auto
			unordered_map<int, Course*>:: iterator it =
				courses.find(id);
			if(it != courses.end()){
				return it->second;}
			else{	return	NULL;}
		}

		int getNumberOfTeachers() const { return teachers.size();}
				
		int getNumberOfStudentSection() const {return sections.size();}

        int getNumberOfRooms() const {return rooms.size();}

		int getNumberOfCourses() const { return courses.size(); }
		
		const list<CourseClass*>& getCourseClasses() const {return courseClasses;}
		
		int getNumberOfCourseClasses() const {return courseClasses.size();}	
		
		bool getIsEmpty() const {return isEmpty;}
	
	private:
		
		unordered_map<int, Course*>			courses;            //courses mapped with their ids
		unordered_map<int, Room*>			rooms;              //rooms mapped with their ids
		unordered_map<int, Teacher*>		teachers;           //teachers mapped with their ids
		unordered_map<int, StudentSection*>	sections;           //sections mapped with their ids
		list<CourseClass*>					courseClasses;      //list of all parsed course classes
		
		bool isEmpty;     //true = specs haven't been parsed yet
};

#endif
