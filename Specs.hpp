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


/*
The static instance is responsible for 
1. reading from input json file(s)
2. creating courses, teachers, sections, and rooms
3. creating courseclasses with existing courses, teachers, and sections
4. providing read-access to these resources to schedule and algorithm
*/

//describes the specifications of courses, rooms, classes, teachers, and sections
class Specs{
	public:
		
		Specs() : isEmpty(true){}
		~Specs();
		
        //read the file and parse objects
		void parseFile(string&);
		
        //return pointer to a teacher OR NULL if teacher doesn't exist
		Teacher* getTeacherById(int id){
			auto it = teachers.find( id );
			return ( it != teachers.end() ? it->second: NULL );
		}

        //return pointer to a section OR NULL if section doesn't exist
		StudentSection* getSectionById(int id){
            auto it = sections.find( id );
			return ( it != sections.end() ? it->second: NULL );
		}
		
        //return pointer to a room OR NULL if room doesn't exist
		Room* getRoomById(int id){
            auto it = rooms.find( id );
			return ( it != rooms.end() ? it->second: NULL );
		}
		
        //return pointer to a course OR NULL if course doesn't exist
		Course* getCourseById(int id){
            auto it = courses.find( id );
			return ( it != courses.end() ? it->second: NULL );
		}

		int getNumberOfTeachers() const { return teachers.size();}
				
		int getNumberOfStudentSection() const {return sections.size();}

        int getNumberOfRooms() const {return rooms.size();}

		int getNumberOfCourses() const { return courses.size(); }
		
		const list<CourseClass*>& getCourseClasses() const {return courseClasses;}
		
		int getNumberOfCourseClasses() const {return courseClasses.size();}	
		
		int getNumberOfConstraints() const {return numberOfConstraints;}

		int getWeekDays() const {return	weekDays;}

		int getTotalHours() const {return totalHours;}

		int getMutationRate() const {return mutationRate;}

		int getCrossoverRate() const {return crossoverRate;}

		int getMutationPoints() const {return mutationPoints;}

		int getCrossoverPoints() const {return crossoverPoints;}

		int getPopulationSize() const {return populationSize;}

		int getMaxGenerations() const {return maxGenerations;}

		int getBestSize() const {return bestSize;}

		int getWorstSize() const {return worstSize;}

		bool getIsEmpty() const {return isEmpty;}

		static Specs& getInstance() { return instance;}
	
	private:
		
		//global static instance of specifications
		static Specs	instance;

		unordered_map<int, Course*>			courses;		//courses mapped with their ids
		unordered_map<int, Room*>			rooms;			//rooms mapped with their ids
		unordered_map<int, Teacher*>		teachers;		//teachers mapped with their ids
		unordered_map<int, StudentSection*>	sections;		//sections mapped with their ids
		list<CourseClass*>					courseClasses;	//list of all parsed course classes

		//number of constraints for each class(related to room, course, teacher, and section)
		int		numberOfConstraints;
		
		//uni specific details weekdays hours etc

		//number of days uni is open (5)
		int		weekDays;
		//number of hours uni is open
		int		totalHours;

		/*algo specific details tweak to check performance*/

		//probability of mutation occuring
		double	mutationRate;

		//probability of crossover occuring
        double	crossoverRate;

		//number of points moved by a mutation
        int		mutationPoints;

		//number of points where parents' points will crossover
        int		crossoverPoints;

		//number of chromosomes in one population
		int		populationSize;

		//maximum number of generations the algorithm can run
		int		maxGenerations;

		//number of best chromosomes to track
		int		bestSize;

		//number of worst chromosomes to track
		int		worstSize;


		//true = specs haven't been parsed yet
		bool	isEmpty;
};

#endif
