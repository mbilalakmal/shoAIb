#ifndef SPECS
#define SPECS

#include<unordered_map>
#include<fstream>

using namespace std;

class Room;
class Course;
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
		
        //return pointer to a teacher OR nullptr if teacher doesn't exist
		Teacher* getTeacherById(const string& id){
			auto it = teachers.find( id );
			return ( it != teachers.end() ? it->second: nullptr );
		}

        //return pointer to a section OR nullptr if section doesn't exist
		StudentSection* getSectionById(const string& id){
            auto it = sections.find( id );
			return ( it != sections.end() ? it->second: nullptr );
		}
		
        //return pointer to a room OR nullptr if room doesn't exist
		Room* getRoomById(int id){
            auto it = rooms.find( id );
			return ( it != rooms.end() ? it->second: nullptr );
		}
		
        //return pointer to a course OR nullptr if course doesn't exist
		Course* getCourseById(const string& courseCode){
            auto it = courses.find( courseCode );
			return ( it != courses.end() ? it->second: nullptr );
		}

		int getNumberOfTeachers() const { return teachers.size();}
				
		int getNumberOfStudentSection() const {return sections.size();}

        int getNumberOfRooms() const {return rooms.size();}

		int getNumberOfCourses() const { return courses.size(); }
		
		const vector<Lecture*>& getLectures() const {return lectures;}
		
		int getNumberOfLectures() const {return lectures.size();}	
		
		/*
		FOR TESTING PURPOSE ONLY!!!
		*/

		int getNumberOfConstraints() const {return 4;}//numberOfConstraints;}

		int getWeekDays() const {return	3;}//weekDays;}

		int getTotalHours() const {return 4;}//totalHours;}

		double getMutationRate() const {return 0.05;}//mutationRate;}

		double getCrossoverRate() const {return 0.65;}//crossoverRate;}

		int getMutationPoints() const {return 2;}//mutationPoints;}

		int getCrossoverPoints() const {return 2;}//crossoverPoints;}

		int getPopulationSize() const {return 60;}//populationSize;}

		int getMaxGenerations() const {return 2000;}//maxGenerations;}

		int getBestSize() const {return 4;}//bestSize;}

		int getWorstSize() const {return 4;}//worstSize;}

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
		unordered_map<string, Course*>		courses;
		//rooms mapped with their ids
		unordered_map<int, Room*>		rooms;
		//teachers mapped with their ids
		unordered_map<string, Teacher*>	teachers;
		//sections mapped with their ids
		unordered_map<string, StudentSection*>	sections;
		//list of all parsed course classes
		
		vector<Lecture*>	lectures;

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
