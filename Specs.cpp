#include "Specs.hpp"
#include "Room.hpp"
#include "CourseClass.hpp"
#include<iostream>
#include<string>
using namespace std;


//declaration of global static instance of specification
Specs Specs::instance;

//destructor for specifications, deletes the stored
//teachers, sections, courses, classes, and rooms
Specs::~Specs(){
    //delete all the teachers
	for( auto it = teachers.begin();
		it != teachers.end(); it++ )
		delete it->second;

    //delete all the sections
	for( auto it = sections.begin();
		it != sections.end(); it++ )
		delete it->second;

    //delete all the courses
	for( auto it = courses.begin();
		it != courses.end(); it++ )
		delete it->second;

    //delete all the rooms
	for( auto it = rooms.begin();
		it != rooms.end(); it++ )
		delete it->second;

    //delete all the classes
	for( auto it = courseClasses.begin();
		it != courseClasses.end(); it++ )
		delete *it;
}

void Specs::parseFile(string& fileName) {

    //clear all the maps
	courseClasses.clear();
    teachers.clear();
    sections.clear();
    courses.clear();
    rooms.clear();

    //take input from json here

	/*
	maybe call one function for each user-defined type
	parseCourses, parseTeachers, parseSections, parseRooms
	then parseCourseClasses which will link actual pointers using IDs
	*/
}