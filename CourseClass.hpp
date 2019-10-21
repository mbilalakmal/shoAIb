#ifndef COURSECLASS
#define COURSECLASS

#include<list>
#include "Course.hpp"
#include "Teacher.hpp"
#include "StudentSection.hpp"

using namespace std;

//describes a union of a course, a teacher, and one or more sections
class CourseClass{
	
	public:
		
		CourseClass(	Course*		course)
					:	id(nextId++),
						course(course),
						strength(0){}
		
		~CourseClass(){}

		//add this course class to the given section &
		//add the section to this course class' included section
		void addSection(StudentSection* section){
			section->addCourseClass(this);
			sections.push_back(section);

			//add the section's strength to the course class
			strength += section->getStrength();
		}

		//add this course class to the given teacher &
		//add the teacher to this course class' teachers
		void addTeacher(Teacher* teacher){
			teacher->addCourseClass(this);
			teachers.push_back(teacher);
		}

		//returns whether another course class has a common section
		bool sectionsOverlap(const CourseClass& courseClass ) const {

			for(auto it = sections.begin(); it != sections.end(); it++){
				for(auto it_c = courseClass.getSections().begin();
					it_c != courseClass.getSections().end(); it_c++){
					//If both course classes contain a section (ID)
					if( ( *it )->getId() == ( *it_c )->getId() ){
						return true;
					}
				}
			}
			//No common section
			return false;
		}

		//returns whether another course class has a common teacher
		bool teachersOverlap(const CourseClass& courseClass ) const {

			for(auto it = teachers.begin(); it != teachers.end(); it++){
				for(auto it_c = courseClass.getTeachers().begin();
					it_c != courseClass.getTeachers().end(); it_c++){
					//If both course classes contain a teacher (ID)
					if( ( *it )->getId() == ( *it_c )->getId() ){
						return true;
					}
				}
			}
			//No common teacher
			return false;
		}

		int getId() const {return id;}

		int getStrength() const {return strength;}
		
		const Course& getCourse() const {return *course;}

		const list<Teacher*>& getTeachers() const
			{return teachers;}

		const list<StudentSection*>& getSections() const
			{return sections;}

		//Reset ID counter to 0
		static void RestartIDs() {nextId = 0;}
	
	private:

		static int	nextId;		//CCId counter to assign IDs

		int			id;			//unique & auto-inc
		int			strength;	//sum of strengths of sections
		Course*		course;		//course reference
		
		//teacher(s) teaching this course class (multiple for freshies' labs)
		list<Teacher*> teachers;

		//atomic sections included in this course class
		list<StudentSection*> sections;
};

//Init ID counter
int CourseClass::nextId = 0;

#endif
