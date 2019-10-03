#include"Schedule.hpp"
#include<vector>
#include<list>
using namespace std;

//describes a whole week's schedule in time-space slots occupied by course classes

//default constructor
Schedule::Schedule(){

}

//copy constructor
Schedule::Schedule(const Schedule& schedule){
    //resize slots and constraints vectors
}

//mutation occurs by randomly swapping some classes within a schedule
void Schedule::mutation(){

}

//calculate score of schedule based on given constraints
void Schedule::calculateFitness(){
    
}

//crossover occurs by randomly swapping some classes between two schedules
void crossOver(Schedule& schedule1, Schedule& schedule2){

}
