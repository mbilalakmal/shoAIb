#include "Specs.hpp"
#include "Room.hpp"
#include "CourseClass.hpp"
#include<iostream>
#include<string>
#include<fstream>
using namespace std;


//declaration of global static instance of specification
Specs Specs::instance;

//destructor for specifications, deletes the stored
//teachers, sections, courses, classes, and rooms
Specs::~Specs(){
    //delete all the teachers
	for( auto it = teachers.begin();
		it != teachers.end(); it++ ){
		delete it->second;
	}
	teachers.clear();

    //delete all the sections
	for( auto it = sections.begin();
		it != sections.end(); it++ ){
		delete it->second;
	}
	sections.clear();

    //delete all the courses
	for( auto it = courses.begin();
		it != courses.end(); it++ ){
		delete it->second;
	}
	courses.clear();

    //delete all the rooms
	for( auto it = rooms.begin();
		it != rooms.end(); it++ ){
		delete it->second;
	}
	rooms.clear();

    //delete all the classes
	for( auto it = courseClasses.begin();
		it != courseClasses.end(); it++ ){
		delete *it;
	}
	courseClasses.clear();

	isEmpty = true;
}

void Specs::parseFile(string& fileName) {

    //clear all the maps
	courseClasses.clear();
    teachers.clear();
    sections.clear();
    courses.clear();
    rooms.clear();

	Room::restartIDs();

    //take input from json here

	int id, cap, floor, dur;
	string name, building, cc, title, dept, sch;
	bool isCL, isEL, hasC, isCC, isLC, needsCL, needsEL, isSenior;

	fstream input(fileName);

	// cout << "10 ROOMS: cap name bu fl isCL isEL hC" << endl;
	for(int i = 0; i < 5; i++){
		input >> cap >> name >> building >> floor >> isCL >> isEL >> hasC;
		Room *room = new Room(cap, name, building, floor, isCL, isEL, hasC);
		rooms.insert(
			pair<int, Room*>
			(room->getId(), room)
		);
	}

	// cout << "10 COURSES: id code title dur dept school isCC isLC needsCL needsEL" << endl;
	for(int i = 0; i < 10; i++){
		input >> id >> cc >> title >> dur >> dept >> sch >> isCC >> isLC >> needsCL >> needsEL;
		Course *course = new Course(id, cc, title, dur, dept, sch, isCC, isLC, needsCL, needsEL);
		courses.insert(
			pair<int, Course*>
			(course->getId(), course)
		);
	}

	// cout << "10 TEACHERS: id name dept isSin bu floor" << endl;
	for(int i = 0; i < 5; i++){
		input >> id >> name >> dept >> isSenior >> building >> floor;
		Teacher *teacher = new Teacher(id, name, dept, isSenior, building, floor);
		teachers.insert(
			pair<int, Teacher*>
			(teacher->getId(), teacher)
		);
	}

	int strength, batch;

	for(int i = 0; i < 5; i++){
		input >> id >> name >> strength >> batch >> dept;
		StudentSection *section = new StudentSection(id, name, strength, batch, dept);
		sections.insert(
			pair<int, StudentSection*>
			(section->getId(), section)
		);
	}

	int cid, tid,  sid;

	for(int i = 0; i < 10; i++){
		input >> cid >> tid >> sid;
		Course *c = getCourseById(cid);
		Teacher *t = getTeacherById(tid);
		StudentSection *s = getSectionById(sid);
		CourseClass *courseClass = new CourseClass(c);
		courseClass->addSection(s);
		courseClass->addTeacher(t);
		s->addCourseClass(courseClass);
		t->addCourseClass(courseClass);
		courseClasses.push_back(courseClass);
	}


	/*
	In Rooms.JSON
	for loop
		Room* newRoom = JSON_to_CPP_Object();

		rooms.emplace()	//proper syntax OR
		rooms.insert( pair<int, Room*>(newRoom->getId(), newRoom) );
	*/

	/*
	maybe call functions for each user-defined type
	courses, rooms, sections, and teachers,
	then CourseClasses which will link actual pointers using IDs
	*/
}