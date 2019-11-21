#ifndef LECTURE
#define LECTURE

#include<vector>

#include "Course.hpp"
#include "Teacher.hpp"
#include "StudentSection.hpp"

using namespace std;

/*
Describes a lecture with 5 attributes including assigned
teachers and enrolled sections with cumulative strength
*/
class Lecture{
	
	public:
		
		Lecture(
			Course*	course, const string& id
		): course(course), id(id), strength(0){}
		
		~Lecture(){
			//this only removes the pointers
			//from vector without deleting
			teachers.clear();
			sections.clear();
		}

		/*
		add this lecture to the given section & add
		the section to this lecture's included sections
		*/
		void addSection(StudentSection* section){
			section->addLecture(this);
			sections.push_back(section);

			//add the section's strength
			strength += section->getStrength();
		}

		/*
		add this lecture to the given teacher & add
		the teacher to this lecture's assigned sections
		*/
		void addTeacher(Teacher* teacher){
			teacher->addLecture(this);
			teachers.push_back(teacher);
		}

		const string& getId() const {return id;}

		int getStrength() const {return strength;}
		
		const Course& getCourse() const {return *course;}

		const vector<Teacher*>& getTeachers() const {return teachers;}

		const vector<StudentSection*>& getSections() const {return sections;}
	
	private:

		const string id;
		int strength;

		const Course* course;
		
		//teacher(s) assigned to this lecture
		vector<Teacher*> teachers;

		//section(s) enrolled in this lecture
		vector<StudentSection*> sections;

};

#endif
