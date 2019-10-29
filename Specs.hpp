#ifndef SPECS
#define SPECS

#include<list>
#include<unordered_map>
#include<fstream>

using namespace std;

class Course;
class Room;
class Lecture;
class Teacher;
class StudentSection;


/*
The static instance is responsible for 
1. reading from input json file(s)
2. creating courses, teachers, sections, and rooms
3. creating Lectures with existing courses, teachers, and sections
4. providing read-access to these resources to schedule and algorithm
*/

//describes the specifications of courses, rooms, classes, teachers, and sections
class Specs{
	public:
	
		~Specs();
		
		/*
		Either a single parseFile functions for all types or
		individual functions for each type
		*/

        //read the file and parse objects from JSON
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
		
		const list<Lecture*>& getLectures() const {return lectures;}
		
		int getNumberOfLectures() const {return lectures.size();}	
		
		/*
		FOR TESTING PURPOSE ONLY!!!
		*/

		int getNumberOfConstraints() const {return 4;}//numberOfConstraints;}

		int getWeekDays() const {return	4;}//weekDays;}

		int getTotalHours() const {return 4;}//totalHours;}

		double getMutationRate() const {return 0.05;}//mutationRate;}

		double getCrossoverRate() const {return 0.6;}//crossoverRate;}

		int getMutationPoints() const {return 2;}//mutationPoints;}

		int getCrossoverPoints() const {return 2;}//crossoverPoints;}

		int getPopulationSize() const {return 100;}//populationSize;}

		int getMaxGenerations() const {return 1000;}//maxGenerations;}

		int getBestSize() const {return 10;}//bestSize;}

		int getWorstSize() const {return 0;}//worstSize;}

		/*^^^
		FOR TESTING PURPOSE ONLY!!!
		*/

		bool getIsEmpty() const {return isEmpty;}

		static Specs& getInstance() { return instance;}
	
	private:
		
		//global static instance of specifications
		static Specs	instance;

		/*
			void parseRooms(string&);
		*/

		
		//courses mapped with their ids
		unordered_map<int, Course*>		courses;
		//rooms mapped with their ids
		unordered_map<int, Room*>		rooms;
		//teachers mapped with their ids
		unordered_map<int, Teacher*>	teachers;
		//sections mapped with their ids
		unordered_map<int, StudentSection*>	sections;
		//list of all parsed course classes
		
		list<Lecture*>	lectures;

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
