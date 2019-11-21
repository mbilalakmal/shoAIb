#ifndef SCHEDULE
#define SCHEDULE

#include<vector>
#include<unordered_map>
#include"Lecture.hpp"
#include"Room.hpp"

using namespace std;

//describes a whole week's schedule in time-space slots occupied by lectures
class Schedule{
	
    //crossover occurs by randomly swapping some classes between two schedules
    friend void crossover(Schedule&, Schedule&, int&);

    //swap function used in assignment operator and other
    friend void swap(Schedule&, Schedule&);

	public:
		
        //fitness divided by the whole population's fitness
        double  relativeFitness;

        //cum probability of being selected in fitness proportionate
        double  cumulativeProb;

        //default constructor
		Schedule(
            int,
            const unordered_map<int, Room*>&,
            const unordered_map<string, Course*>&,
            const unordered_map<string, Teacher*>&,
            const unordered_map<string, StudentSection*>&,
            const vector<Lecture*>&
        );
		
        //copy constructor
        Schedule(const Schedule&);

        //assignment operator
        Schedule& operator = (Schedule);
        
        //destructor
        ~Schedule();

        //initialize schedule with random slots
        void initialize();

        //mutation occurs by randomly swapping some classes within a schedule
        void mutation();

        const vector< vector<Lecture*> >& getSlots() const {return slots;}

        const unordered_map<Lecture*, vector<int> >& getClasses() const {return classes;}

		// const vector<int>& getConstraints() const {return constraints;}

        double getFitness() const {return fitness;}

        /*
        FOR TESTING PURPOSE ONLY
        */
        void printSchedule(bool) const;
		
	private:

        //day * time_slot * rooms
		vector< vector<Lecture*> > slots;

        //Lecture mapped to it's slots
        unordered_map< Lecture*, vector<int> > classes;

        //GA Parameters
        const double mutationRate;
        const double crossoverRate;
        const int mutationSize;
        const int crossoverSize;

        //Aggregated objects
            const unordered_map<int, Room*>&
        rooms;
            const unordered_map<string, Course*>&
        courses;
            const unordered_map<string, Teacher*>&
        teachers;
            const unordered_map<string, StudentSection*>&
        sections;
            const vector<Lecture*>&
        lectures;


        //used to calc fitness [true = that constraint is fulfilled]
            mutable vector< vector< vector<bool> > >
        lectureConstraints;
            mutable vector< vector<bool> >
        teacherConstraints;
            mutable vector< vector<bool> >
        sectionConstraints;

        //schedule's score for complying with constraints
        double fitness;

        //seed value for schedule's random values
        int seed;

        //calculate score of schedule based on given constraints
        void calculateFitness();

        //checks each constraint for all classes and put true/false in the vector
        void checkConstraints() const;

        //adds all the entries in the bool vector, multiplies, and returns a single fitness value
        double addConstraintsWeights();

        Room* getRoomById(int) const;

        // //helper functions to check section & teacher constraints
        // int maxConsecutive(const vector< vector<int> >&) const;
        // int maxDaily(const vector< vector<int> >&) const;
        // int daysOff(const vector< vector<int> >&) const;
        // bool oneAtATime(const vector< vector<int> >&) const;
		
};

#endif
