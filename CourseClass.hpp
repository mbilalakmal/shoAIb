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
		
		CourseClass(int			id,
					Teacher*	teacher,
					Course*		course)
				:	id(id),
					teacher(teacher),
					course(course),
					strength(0){
			
			//add this course class to the given teacher's assigned CCs
			teacher->addCourseClass(this);
		}
		
		~CourseClass(){}

		//add this course class to the given section &
		//add the section to this course class' included section
		void addSection(StudentSection* section){
			section->addCourseClass(this);
			sections.push_back(section);

			//add the section's strength to the course class
			strength += section->getStrength();
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
			//No common sections
			return false;
		}

		//returns whether another course class has the same teacher
		bool teacherOverlaps(const CourseClass& courseClass ) const {
			return teacher->getId() == courseClass.teacher->getId();
		}

		int getId() {return id;}

		int getStrength() const {return strength;}
		
		const Course& getCourse() const {return *course;}

		const Teacher& getTeacher() const {return *teacher;}

		const list<StudentSection*>& getSections() const
			{return sections;}
	
	private:

		int			id;			//unique & auto-inc
		int			strength;	//sum of strengths of sections
		Course*		course;		//course reference
		Teacher*	teacher;	//teacher reference

		//parent sections included in this course class
		list<StudentSection*> sections;
};

#endif
