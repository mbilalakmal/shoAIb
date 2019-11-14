#ifndef LECTURE
#define LECTURE

#include<vector>
#include<algorithm>

#include "Course.hpp"
#include "Teacher.hpp"
#include "StudentSection.hpp"

using namespace std;

//maximum teachers assigned to a lecture
#define MAXTEACHERS 2
//maximum sections enrolled in a lecture
#define MAXSECTIONS 10

/*
Describes a lecture with 5 attributes including assigned
teachers and enrolled sections with cumulative strength
*/
class Lecture{
	
	public:
		
		Lecture(
			Course*	course
		):
			id(nextId++),
			course(course),
			strength(0)
		{
			teachers.reserve(MAXTEACHERS);
			sections.reserve(MAXSECTIONS);
		}
		
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

		//Returns whether another lecture has a common section
		bool sectionsOverlap(const Lecture& lecture ) const {

			auto &sections2 = lecture.getSections();
			for(const auto& section: sections){
				if(
					sections2.end() !=
					find(sections2.begin(), sections2.end(), section)
				){	return true;}
			}

			//No common section
			return false;
		}

		//Returns whether another lecture has a common teacher
		bool teachersOverlap(const Lecture& lecture ) const {

			auto &teachers2 = lecture.getTeachers();
			for(const auto& teacher: teachers){
				if(
					teachers2.end() !=
					find(teachers2.begin(), teachers2.end(), teacher)
				){	return true;}
			}

			//No common teacher
			return false;
		}

		int getId() const {return id;}

		int getStrength() const {return strength;}
		
		const Course& getCourse() const {return *course;}

		const vector<Teacher*>& getTeachers() const {return teachers;}

		const vector<StudentSection*>& getSections() const {return sections;}

		//Reset ID counter to 0
		static void RestartIDs() {nextId = 0;}
	
	private:

		const int		id;
		int				strength;

		const Course*	course;
		
		//teacher(s) assigned to this lecture
		vector<Teacher*> teachers;

		//section(s) enrolled in this lecture
		vector<StudentSection*> sections;

		//LectureId counter to assign IDs
		static int		nextId;

};

//Init ID counter
int Lecture::nextId = 0;

#endif
