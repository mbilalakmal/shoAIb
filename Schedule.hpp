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
    friend void crossOver(Schedule&, Schedule&);

	public:
		
        //default constructor
		Schedule();
		
        //copy constructor
        Schedule(const Schedule&);

        //mutation occurs by randomly swapping some classes within a schedule
        void mutation();

        //calculate score of schedule based on given constraints
        void calculateFitness();

		int getId() const {return id;}
		
		int getFitness() const {return fitness;}

		int getRelativeFitness() const {return relativeFitness;}
		
		int getCumulativeProb() const {return cumulativeProb;}
		
		const vector<bool>& getConstraints() const {return constraints;}

        const unordered_map<CourseClass*, vector<int> >&
            getClasses() const {return classes;}

        const vector< list< CourseClass* > >&
            getSlots() const {return slots;}
		
	private:
		
        int             id;             //unique & auto-inc

		vector<                         //day * time_slot * rooms
            list<                           //candidate course classes
        CourseClass*>
        >               slots;          //time-space slots for course classes

        unordered_map<
            CourseClass*,
            vector<int>
        >               classes;         //courseClass mapped to it's slots

        vector<bool>    constraints;     //used to calc fitness [true = that constraint is fulfilled]

        //DNA part here
        double          fitness;         //schedule's score for complying with constraints

        double          relativeFitness; //fitness divided by the whole population's fitness
        double          cumulativeProb;  //cum probability of being selected in fitness proportionate

		
};

#endif
