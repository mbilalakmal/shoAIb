#include "Specs.hpp"
#include "Room.hpp"
#include "CourseClass.hpp"
#include<iostream>
#include<string>
using namespace std;

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
    teachers.clear();
    sections.clear();
    courses.clear();
    rooms.clear();
    courseClasses.clear();

    //take input
}