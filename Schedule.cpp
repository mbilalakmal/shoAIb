#include"Schedule.hpp"
#include"Specs.hpp"
#include"Room.hpp"
#include"Random.hpp"
#include<vector>
#include<list>
#include<unordered_set>
using namespace std;

//describes a whole week's schedule in time-space slots occupied by course classes

//default constructor
Schedule::Schedule(int &seed){
    
    //resize slots vector
    slots.resize(
        Specs::getInstance().getWeekDays *
        Specs::getInstance().getTotalHours *
        Specs::getInstance().getNumberOfRooms()
        //Weekdays * timeslots * rooms FROM Specs
    );

    //resize constraints vector
    constraints.resize(
        Specs::getInstance().getNumberOfCourseClasses() *
        Specs::getInstance().getNumberOfConstraints(),
        true
        //Number of hard+soft constraints * classes FROM Specs
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
        
            day     = i4_uniform_ab(0, weekDays - 1, seed);
            room    = i4_uniform_ab(0, numberOfRooms - 1, seed);
            time    = i4_uniform_ab(0, totalHours - duration, seed);
            
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
                day         = i4_uniform_ab(0, weekDays - 1, seed);
                room        = i4_uniform_ab(0, numberOfRooms - 1, seed);
                time        = i4_uniform_ab(0, totalHours - 1, seed);

                position    =   time +
                                totalHours * room +
                                totalHours * numberOfRooms * day;

                slots[position+i].push_back( it );
                positions.push_back(position+i);
            }

            classes.insert( pair< CourseClass*, vector<int> >(it, positions) );

        }

        //resize the array to zero for next class
        positions.clear();

    }

    //every courseClass is randomly placed in time-space slots

    //calculate fitness for newly created
    calculateFitness();

    relativeFitness = 0.0;
    cumulativeProb = 0.0;

}

//copy constructor
Schedule::Schedule(const Schedule& schedule){
    //copy rhs entries to this schedule

    slots       = schedule.slots;
    classes     = schedule.classes;

    constraints = schedule.constraints;
    fitness     = schedule.fitness;

}

//copy assignment operator
Schedule& Schedule::operator = (Schedule schedule){
            //this schedule is swapped with temporary
            swap(*this, schedule);
            //swapped is returned while the old's lifecycle ends here
            return *this;
}

//destructor
Schedule::~Schedule(){
    slots.clear();
    classes.clear();
    constraints.clear();
    fitness = 0;
}

//mutation occurs by randomly swapping some classes within a schedule
void Schedule::mutation(int &seed){

    //coin flip
    double random = r8_uniform_ab(0.0, 1.0, seed);
    if ( random > Specs::getInstance().getMutationRate() ){
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
        mutationClassPosition = i4_uniform_ab(0, numberOfClasses - 1, seed);
        
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
            day     = i4_uniform_ab(0, weekDays - 1, seed);
            room    = i4_uniform_ab(0, numberOfRooms - 1, seed);
            time    = i4_uniform_ab(0, totalHours - duration, seed);

            newPosition =   time +
                            totalHours * room +
                            totalHours * numberOfRooms * day;

        
            //relocate all slots
            for(int j = 0; j < duration; j++){

                //get list of classes at current slot
                list<CourseClass*>& classesList =
                    slots[currentPosition+j];

                //find target courseClass
                //replace with std::find ==
                for(auto it = classesList.begin(); it != classesList.end(); it++){
                    //check if it is the target courseClass
                    if( (*it)->getId() == courseClass->getId() ){
                        //erase the pointer NOT THE OBJECT from the list
                        classesList.erase( it );
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
                    if( (*it)->getId() == courseClass->getId() ){
                        classes.erase( it );
                        break;
                    }
                }

                //pick rando day, room, and time
                day     = i4_uniform_ab(0, weekDays - 1, seed);
                room    = i4_uniform_ab(0, numberOfRooms - 1, seed);
                time    = i4_uniform_ab(0, totalHours - 1, seed);
                
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

        currentPositions.clear();
        newPositions.clear();

    }

    //calc new fitness after mutation
    calculateFitness();

}

//calculate score of schedule based on given constraints
void Schedule::calculateFitness(){

    //check each constraint for all classes and mark true/false
    checkConstraints();

    //add weighted contraints and return total fitness
    fitness = addConstraintsWeights();

}

void Schedule::checkConstraints(){

    int classCounter         = 0;

    int numberOfRooms   = Specs::getInstance().getNumberOfRooms();
    int totalHours      = Specs::getInstance().getTotalHours();
    int spaceTimePerDay = totalHours * numberOfRooms;

    //each iteration runs for one class of this schedule
    for(const auto& classIterator: classes){

        //get const pointer to course class
        const CourseClass* courseClass = classIterator.first;

        //flags to see if both (a section clash and a teacher clash)
        //are found to exit searching 
        bool sectionFound = false, teacherFound = false;

        //each iteration runs for one slot of this class
        for(const auto& i: classIterator.second){

            //get coordinates from position
            int position    = i;
            int day         = position / spaceTimePerDay;
            int spaceTime   = position % spaceTimePerDay;
            int roomID      = spaceTime / totalHours;
            int time        = spaceTime % totalHours;

            Room* room      = Specs::getInstance().getRoomById(roomID);

            /*
            Check for INHERENT::ROOM constraints
            */

            //no more than one class
            if( slots[position].size() > 1 ){
                constraints[classCounter + 0] = false;
            }

            //capacity at least equal to strength
            if( room->getCapacity() < courseClass->getStrength() ){
                constraints[classCounter + 1] = false;
            }

            //theory/lab in theory/lab
            if(
                courseClass->getCourse().getNeedsElectricalLab() != room->getIsElectricalLab() ||
                courseClass->getCourse().getNeedsComputerLab() != room->getIsComputerLab() ||
                (
                    courseClass->getTeachers().front()->getNeedsComputer() == true &&
                    room->getHasComputer() == false
                )
            ){
                constraints[classCounter + 2] = false;
            }

            /*
            check for INHERENT::SECTION & TEACHER constraints
            */

            //each iteration runs for one room 
            for(int j = 0, dayTime = day * spaceTimePerDay + time;
                j < numberOfRooms, ( !sectionFound || !teacherFound );
                j++, dayTime += totalHours)
            {

                const list<CourseClass*>& ccList = slots[dayTime];

                //for this room check for clash with the original class [section & teacher]

                for(auto iterator = ccList.begin();
                    iterator != ccList.end(), ( !sectionFound || !teacherFound );
                    iterator++
                ){
                    //dont compare class with it self
                    if( courseClass->getId() == (*iterator)->getId() ){continue;}

                    //if original class and this class has same teacher, it's a clash
                    if(courseClass->teachersOverlap( **iterator )){
                        teacherFound = true;}

                    //if original class and this class has common section(s), it's a clash
                    if(courseClass->sectionsOverlap( **iterator )){
                        sectionFound = true;}
                }

           }

            //no other class for one section
            if(sectionFound){
                constraints[classCounter + 3] = false;
            }

            //no other class for one teacher
            if(teacherFound){
                constraints[classCounter + 4] = false;
            }

        }
        
        classCounter++;

    }

}

double Schedule::addConstraintsWeights(){
    double totalScore = 0;

    for(const auto& i: constraints){
        totalScore += (int) i;
    }

    //fitness returned will be between 0 to 1 inclusive
    double constraintsSize = (double) constraints.size();
    return (
        (double) totalScore / constraintsSize
    );

}

//crossover occurs by randomly swapping some classes between two schedules
//this is a little different from typical crossover implementations as
//no new offsprings are created. However it still works (uncited)
//might have to implement another
void crossOver(Schedule& schedule1, Schedule& schedule2, int &seed){

    //coin flip
    double random = r8_uniform_ab(0, 1.0, seed);
    if( random > Specs::getInstance().getCrossoverRate() ){
        return;
    }

    int numberOfClasses = schedule1.classes.size();

    //coPoints will determine which classes are swapped
    unordered_set<int> crossOverPoints;

    while ( crossOverPoints.size() < Specs::getInstance().getCrossoverPoints() )
    {
        //a set only inserts new elements so uniqueness is guaranteed
        int pointPosition = i4_uniform_ab(0, numberOfClasses - 1, seed);
        crossOverPoints.insert(pointPosition);
    }

    auto it1    = schedule1.classes.begin();
    auto it2    = schedule2.classes.begin();

    //get reference to both schedules' slots
    auto slots1 = schedule1.getSlots();
    auto slots2 = schedule2.getSlots();

    bool swap   = i4_uniform_ab(0, 1, seed);    //0 OR 1

    for(int i = 0; i < numberOfClasses; i++, it1++, it2++){

        auto iterator = crossOverPoints.find(i);
        //if i is a crossover point
        if( iterator != crossOverPoints.end() ){
            swap = !swap;
        }

        if(swap == false){
            continue;
        }
        //crossover occurs here
        
        CourseClass *courseClass = it1->first;

        int duration = courseClass->getCourse().getDuration();
        vector<int> tempPositions;  tempPositions.resize(duration); //temporary vector used in two way swap of positions

        //each iteration swaps(only removes) one slot in the slots entries of both schedules
        for(int j = 0; j < duration; j++){

            //get list of classes at sechedule 1 and 2 slot
            list<CourseClass*>& classesList1 = slots1[
                it1->second[j]
            ];
            list<CourseClass*>& classesList2 = slots2[
                it2->second[j]
            ];

            //remove entry from schedule 1's slot
            for(auto classesList1Iterator = classesList1.begin(); classesList1Iterator != classesList1.end(); classesList1Iterator++){
                //check if it is the target courseClass
                if( (*classesList1Iterator)->getId() == courseClass->getId() ){
                    //remove pointer from this slot
                    classesList1.erase( classesList1Iterator );
                    break;
                }
            }

            //remove entry from schedule 2's slots
            for(auto classesList2Iterator = classesList2.begin(); classesList2Iterator != classesList2.end(); classesList2Iterator++){
                //check if it's the target
                if( (*classesList2Iterator)->getId() == courseClass->getId() ){
                    classesList2.erase( classesList2Iterator );
                    break;
                }
            }


        }

        //swap entries in classes map to new positions
        tempPositions   = it2->second;
        it2->second     = it1->second;
        it1->second     = tempPositions;

        //now we need to insert courseclass pointers in the lists at these new slots
        for(const auto& positions1: it1->second){
            slots1[positions1].push_back(courseClass);
        }
        for(const auto& positions2: it2->second){
            slots2[positions2].push_back(courseClass);
        }

    }

    schedule1.calculateFitness();
    schedule2.calculateFitness();

    //switcheroo complete

}

//swaps two schedules, used in ass operator
void swap(Schedule& first, Schedule& second){
    
    using std::swap;

    swap(first.classes, second.classes);
    swap(first.slots, second.slots);
    swap(first.constraints, second.constraints);
    swap(first.fitness, second.fitness);
}

/*comparator function for schedule's fitness, used in algorithm.cpp(selection)
std::sort will use them to sort best and worst for ( elitism and weeding )
This is a non-friend non-member function to ensure safety and encapsulation
*/
bool compareSchedules(Schedule const *schedule1, Schedule const *schedule2){
    return (
        schedule1->getFitness() < schedule2->getFitness()
    );
}