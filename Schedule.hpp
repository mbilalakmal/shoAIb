#ifndef SCHEDULE
#define SCHEDULE

#include<vector>
#include<unordered_map>
#include"Lecture.hpp"
using namespace std;

class Lecture;

//describes a whole week's schedule in time-space slots occupied by course classes
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
		Schedule();
		
        //copy constructor
        Schedule(const Schedule&);

        //assignment operator
        Schedule& operator = (Schedule);
        
        //destructor
        ~Schedule();

        //initialize schedule with random slots
        void initialize(int&);

        //mutation occurs by randomly swapping some classes within a schedule
        void mutation(int&);

        const vector< vector<Lecture*> >& getSlots() const {return slots;}

        const unordered_map<Lecture*, vector<int> >& getClasses() const {return classes;}

		const vector<bool>& getConstraints() const {return constraints;}

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

        //used to calc fitness [true = that constraint is fulfilled]
        vector<bool> constraints;

        //schedule's score for complying with constraints
        double fitness;

        //calculate score of schedule based on given constraints
        void calculateFitness();

        //checks each constraint for all classes and put true/false in the vector
        void checkConstraints();

        //adds all the entries in the bool vector, multiplies, and returns a single fitness value
        double addConstraintsWeights();
		
};

#endif
