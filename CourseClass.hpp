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

		//add this course class to the given section &
		//add the section to this course class' included section
		void addSection(StudentSection* section){
			section->addCourseClass(this);
			sections.push_back(section);
			strength += section->getStrength();
		}

		//returns whether another course class has a common section with this
		bool sectionsOverlap(const CourseClass& c ) const {
			for(auto it = sections.begin(); it != sections.end(); it++){
				for(auto it_c = c.sections.begin(); it_c != sections.end(); it_c++){
					if( (*it)->getId() == (*it_c)->getId() ){
						return true;
					}
				}
			}
			return false;
		}

		//returns whether another course class has the same teacher
		bool teacherOverlaps(const CourseClass& c ) const {
			return teacher->getId() == c.teacher->getId();
		}

		int getId() {return id;}

		int getStrength() const {return strength;}
		
		const Course& getCourse() const {return *course;}

		const Teacher& getTeacher() const {return *teacher;}

		const list<StudentSection*>& getSections() const {return sections;}
		
		~CourseClass(){}
	
	private:

		int				id;				//unique & auto-inc
		int				strength;		//sum of strengths of sections
		Course*			course;			//course reference
		Teacher*		teacher;		//teacher reference

		list<StudentSection*> sections;	//student sections included
};

#endif
