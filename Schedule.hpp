#ifndef SCHEDULE
#define SCHEDULE

#include<vector>
#include<list>
#include<unordered_map>
#include"CourseClass.hpp"
using namespace std;

class CourseClass;

//describes a whole week's schedule in time-space slots occupied by course classes
class Schedule{
	
    //crossover occurs by randomly swapping some classes between two schedules
    friend void crossOver(Schedule&, Schedule&, int&);

    //swap function used in assignment operator and other
    friend void swap(Schedule&, Schedule&);

	public:
		
        //fitness divided by the whole population's fitness
        double  relativeFitness;

        //cum probability of being selected in fitness proportionate
        double  cumulativeProb;

        //default constructor
		Schedule(int&);
		
        //copy constructor
        Schedule(const Schedule&);

        //assignment operator
        Schedule& operator = (Schedule);
        
        //destructor
        ~Schedule();

        //mutation occurs by randomly swapping some classes within a schedule
        void mutation(int&);

		int getId() const {return id;}
		
		double getFitness() const {return fitness;}
		
		const vector<bool>& getConstraints() const {return constraints;}

        const unordered_map<CourseClass*, vector<int> >&
            getClasses() const {return classes;}

        const vector< list< CourseClass* > >&
            getSlots() const {return slots;}
		
	private:
		
        //static counter to generate ids for schedules
        static int idCounter;

        int id;             //unique & auto-inc

        //day * time_slot * rooms
		vector< list< CourseClass* > >
            slots;

        //courseClass mapped to it's slots
        unordered_map< CourseClass*, vector< int > >
            classes;

        //used to calc fitness [true = that constraint is fulfilled]
        vector<bool>    constraints;

        //calculate score of schedule based on given constraints
        void calculateFitness();

        //checks each constraint for all classes and put true/false in the vector
        void checkConstraints();

        //adds all the entries in the bool vector, multiplies, and returns a single fitness value
        double addConstraintsWeights();

        //DNA part here
        double          fitness;         //schedule's score for complying with constraints
		
};

#endif
