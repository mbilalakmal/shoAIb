#include"Schedule.hpp"
#include"Specs.hpp"
#include"Room.hpp"
#include<vector>
#include<list>
#include<unordered_set>
using namespace std;

//describes a whole week's schedule in time-space slots occupied by course classes

//default constructor
Schedule::Schedule(){
    
    //resize slots vector
    slots.resize(
        //Weekdays * timeslots * rooms FROM Specs
        1       //PLACEHOLDER!!
    );

    //resize constraints vector
    constraints.resize(
        //Number of hard+soft constraints FROM Specs
        1       //PLACEHOLDER!!
    );

    //get classes and place at random slots

    const list<CourseClass*>& courseClasses = Specs::getInstance().getCourseClasses();

    int numberOfRooms   = Specs::getInstance().getNumberOfRooms();
    int weekDays        = Specs::getInstance().getWeekDays();
    int totalHours      = Specs::getInstance().getTotalHours();

    int duration, day, room, time, position;
    vector<int> positions;

    for(auto& it: courseClasses){

        //determine random day, time, space

        duration    =   it->getCourse().getDuration();
        positions.reserve(duration);

        if(it->getCourse().getIsLabCourse()){
        
            day         =   rand() % weekDays;  //replace rand!!
            room        =   rand() % numberOfRooms;
            time        =   rand() % ( totalHours - duration + 1 ); //wajeeh bhai
            
            //actual index for the vector[day*space*time]
            position    =   time +
                            totalHours * room +
                            totalHours * numberOfRooms * day;

            //fill space-time slots for duration of lab
            for(int i = 0; i < duration; i++){
                slots[position+i].push_back( it );
                positions.push_back(position+i);
            }

            classes.insert( pair< CourseClass*, vector<int> >(it, positions) );

        }
        else{

            for(int i = 0; i < duration; i++){
                day         =   rand() % weekDays;
                room        =   rand() % numberOfRooms;
                time        =   rand() % (totalHours);

                position    =   time +
                                totalHours * room +
                                totalHours * numberOfRooms * day;

                slots[position+i].push_back( it );
                positions.push_back(position+i);
            }

            classes.insert( pair< CourseClass*, vector<int> >(it, positions) );

        }

        

    }

    //every courseClass is randomly placed in time-space slots

    //calculate fitness for newly created
    calculateFitness();

    //welcome to the world

}

//copy constructor
Schedule::Schedule(const Schedule& schedule){
    //resize slots and constraints vectors
    //*Maybe need a copy functions instead of this*//
}

//mutation occurs by randomly swapping some classes within a schedule
void Schedule::mutation(){

    //coin flip // dice roll // rand() call
    if( rand() % 100 > Specs::getInstance().getMutationRate() ){
        return;
    }

    int numberOfClasses =   classes.size();
    int mutationClassPosition;  //position of class whose slots will be relocated
    int numberOfRooms, duration, day, room, time, currentPosition, newPosition;
    vector<int> currentPositions, newPositions;

    numberOfRooms = Specs::getInstance().getNumberOfRooms();
    int weekDays = Specs::getInstance().getWeekDays();
    int totalHours = Specs::getInstance().getTotalHours();

    //for each mutation point select a class and change its position
    for(int i = 0; i < Specs::getInstance().getMutationPoints(); i++){

        //pick random class
        mutationClassPosition = rand() % numberOfClasses;
        auto it = classes.begin();
        //move iterator to mutationPosition
        advance(it, mutationClassPosition);

        CourseClass *courseClass = it->first;

        duration = courseClass->getCourse().getDuration();
        currentPositions.reserve(duration);
        newPositions.reserve(duration);


        //*Lab and Theory courses differ in the manner of relocation
        //*of their slots because lab slots should be contiguous

        if(courseClass->getCourse().getIsLabCourse()){
            currentPosition = it->second[0];

            //pick rando day, room, and time
            day = rand() % weekDays;
            room = rand() % numberOfRooms;
            time = rand() % (totalHours - duration + 1);
            newPosition =   time +
                            totalHours * room +
                            totalHours * numberOfRooms * day;

        
            //relocate all slots
            for(int j = 0; j < duration; j++){

                //get list of classes at current slot
                list<CourseClass*>& classes =
                    slots[currentPosition+j];

                //find target courseClass
                //replace with std::find ==
                for(auto it = classes.begin(); it != classes.end(); it++){
                    //check if it is the target courseClass
                    if( (*it) == courseClass){
                        //erase the pointer NOT THE OBJECT from the list
                        classes.erase( it );
                        break;
                    }
                }

                //relocate at newPositions
                slots[newPosition+j].push_back(courseClass);
                newPositions.push_back(newPosition+j);
                
            }

            //change entries in classes map to new positions
            classes[courseClass] = newPositions;

        }
        else{
            //get current positions of all slots
            currentPositions = it->second;

            for(int j = 0; j < duration; j++){

                
                currentPosition = currentPositions[j];

                //get list of classes at current slot
                list<CourseClass*>& classes =
                    slots[currentPosition];

                //find target courseClass
                for(auto it = classes.begin(); it != classes.end(); it++){
                    if( (*it) == courseClass ){
                        classes.erase( it );
                        break;
                    }
                }

                //pick rando day, room, and time
                day = rand() % weekDays;
                room = rand() % numberOfRooms;
                time = rand() % (totalHours);
                newPosition =   time +
                            totalHours * room +
                            totalHours * numberOfRooms * day;

                //relocate at newPositions
                slots[newPosition+j].push_back(courseClass);
                newPositions.push_back(newPosition+j);

            }

            //change entries in classes map to new positions
            classes[courseClass] = newPositions;

        }

    }

    //mutatedCourseClass:   'You took everything from me!'
    //GeneticAlgorithm:     'I don't even know ;

    //calc new fitness after mutation
    calculateFitness();

}

//calculate score of schedule based on given constraints
void Schedule::calculateFitness(){
    //internally calls two functions
    //first to check constraints and
    //fill relevant boolean flags
    //second to compute a total score
    //by adding constraints with their appropriate weights


}

//crossover occurs by randomly swapping some classes between two schedules
//this is a little different from typical crossover implementations as
//no new offsprings are created. However it still works (uncited)
//might have to implement another
void crossOver(Schedule& schedule1, Schedule& schedule2){

    //coin flip // dice roll // rand() call
    if( rand() % 100 > Specs::getInstance().getCrossoverRate() ){
        return;
    }

    int numberOfClasses = schedule1.classes.size();

    //coPoints will determine which classes are swapped
    unordered_set<int> crossOverPoints;

    while ( crossOverPoints.size() < Specs::getInstance().getCrossoverPoints() )
    {
        //a set only inserts new elements so uniqueness is guaranteed
        int pointPosition = rand() % numberOfClasses;
        crossOverPoints.insert(pointPosition);
    }

    auto it1 = schedule1.classes.begin();
    auto it2 = schedule2.classes.begin();



    for(const auto& point: crossOverPoints){
        //swap classes at point
    }

}
