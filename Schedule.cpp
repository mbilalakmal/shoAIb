#include"Schedule.hpp"
#include"Specs.hpp"
#include"Room.hpp"
#include"Random.hpp"
#include<vector>
#include<list>
#include<unordered_set>
#include<iostream>

using namespace std;

//describes a whole week's schedule in time-space slots occupied by course classes

//default constructor
Schedule::Schedule(int &seed){
    //resize slots vector
    slots.resize(
        Specs::getInstance().getWeekDays() *
        Specs::getInstance().getTotalHours() *
        Specs::getInstance().getNumberOfRooms()
        //Weekdays * timeslots * rooms FROM Specs
    );

    //resize constraints vector
    constraints.resize(
        Specs::getInstance().getNumberOfLectures() *
        Specs::getInstance().getNumberOfConstraints()
        //Number of hard+soft constraints * classes FROM Specs
    );

    //get classes and place at random slots

    const list<Lecture*>& lectures = Specs::getInstance().getLectures();

    int numberOfRooms   = Specs::getInstance().getNumberOfRooms();
    int weekDays        = Specs::getInstance().getWeekDays();
    int totalHours      = Specs::getInstance().getTotalHours();

    int duration, day, room, time, position;
    vector<int> positions;

    for(auto& lecture: lectures){

        //determine random day, time, space

        duration    =   lecture->getCourse().getDuration();
        positions.reserve(duration);

        if(lecture->getCourse().getIsLabCourse()){
        
            day     = i4_uniform_ab(0, weekDays - 1, seed);
            room    = i4_uniform_ab(0, numberOfRooms - 1, seed);
            time    = i4_uniform_ab(0, totalHours - duration, seed);
            
            //actual index for the vector[day*space*time]
            position    =   time +
                            totalHours * room +
                            totalHours * numberOfRooms * day;

            //fill space-time slots for duration of lab
            for(int i = 0; i < duration; i++){
                slots.at(position + i).push_front( lecture );
                positions.push_back( position + i );
            }

            classes.insert( pair< Lecture*, vector<int> >(lecture, positions) );

        }
        else{

            unordered_set<int> uniquePositions;

            while(uniquePositions.size() < duration){
                position = i4_uniform_ab(0, weekDays * numberOfRooms * totalHours - 1, seed);
                bool inserted = uniquePositions.insert(position).second;
                if(inserted) {
                    slots.at(position).push_back( lecture );
                    positions.push_back(position);
                }
            }

            // for(const auto& i: positions){
            //     cout << i << "  ";
            // }
            // cout << endl;

            classes.insert( pair< Lecture*, vector<int> >(lecture, positions) );

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

    int mutationPoints = Specs::getInstance().getMutationPoints();

    //for each mutation point select a class and change its position
    for(int i = 0; i < mutationPoints; i++){

        //pick random class
        mutationClassPosition = i4_uniform_ab(0, numberOfClasses - 1, seed);
        
        auto it = classes.begin();
        //move iterator to mutationPosition
        advance(it, mutationClassPosition);

        Lecture *lecture = it->first;

        duration = lecture->getCourse().getDuration();
        currentPositions.reserve(duration);
        newPositions.reserve(duration);


        //*Lab and Theory courses differ in the manner of relocation
        //*of their slots because lab slots should be contiguous

        if(lecture->getCourse().getIsLabCourse()){

            currentPosition = it->second.at(0);

            for(int j = 0; j < duration; j++){

                //get list of classes at current slot
                list<Lecture*>& lecturesList = slots.at( currentPosition + j );

                //remove target lecture
                lecturesList.remove( lecture );
            }

            //pick rando day, room, and time
            day     = i4_uniform_ab(0, weekDays - 1, seed);
            room    = i4_uniform_ab(0, numberOfRooms - 1, seed);
            time    = i4_uniform_ab(0, totalHours - duration, seed);

            newPosition =   time +
                            totalHours * room +
                            totalHours * numberOfRooms * day;

        
            //relocate all slots

            for(int j = 0; j < duration; j++){

                //relocate at newPositions
                slots.at(newPosition + j).push_back( lecture );
                newPositions.push_back(newPosition + j);
            }

            // cout << "MUT: ";
            // for(const auto& i: newPositions){
            //     cout << i << "  ";
            // }
            // cout << endl;

            //change entries in classes map to new positions
            classes.at( lecture ) = newPositions;

        }
        else{
            //get current positions of all slots
            currentPositions = it->second;

            unordered_set<int> uniquePositions;

            //remove lecture from current positions
            for(int j = 0; j < duration; j++){
                currentPosition = currentPositions.at(j);

                //get list of classes at current slot
                list<Lecture*>& lecturesList = slots.at( currentPosition);

                //remove target lecture
                lecturesList.remove( lecture );
            }

            //pick new positions and insert lecture at their slots
            while(uniquePositions.size() < duration){
                newPosition = i4_uniform_ab(0, weekDays * numberOfRooms * totalHours - 1, seed);
                bool inserted = uniquePositions.insert(newPosition).second;
                if(inserted) {
                    slots.at(newPosition).push_back( lecture );
                    newPositions.push_back(newPosition);
                }
            }

            // cout << "MUT: ";
            // for(const auto& i: newPositions){
            //     cout << i << "  ";
            // }
            // cout << endl;

            //change entries in classes map to new positions
            classes.at(lecture) = newPositions;

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

    //reset all constraints to TRUE(default)
    fill(constraints.begin(), constraints.end(), true);

    int numberOfRooms   = Specs::getInstance().getNumberOfRooms();
    int totalHours      = Specs::getInstance().getTotalHours();
    int spaceTimePerDay = totalHours * numberOfRooms;

    //each iteration runs for one class of this schedule
    for(const auto& classIterator: classes){

        //get const pointer to course class
        const Lecture* lecture = classIterator.first;

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
            if( slots.at(position).size() > 1 ){
                constraints.at( classCounter + 0 ) = false;
            }

            //capacity at least equal to strength
            if( room->getCapacity() < lecture->getStrength() ){
                constraints.at( classCounter + 1 ) = false;
            }

            /*
            check for INHERENT::SECTION & TEACHER constraints
            */

            //each iteration runs for one room 
            for(int j = 0, dayTime = day * spaceTimePerDay + time;
                j < numberOfRooms && ( !sectionFound || !teacherFound );
                j++, dayTime += totalHours)
            {

                const list<Lecture*>& ccList = slots.at(dayTime);

                //for this room check for clash with the original class [section & teacher]

                for(auto iterator = ccList.begin();
                    iterator != ccList.end() && ( !sectionFound || !teacherFound );
                    iterator++
                ){
                    //dont compare class with it self
                    if( lecture->getId() == (*iterator)->getId() ){continue;}
                    
                    //if original class and this class has same teacher, it's a clash
                    if(lecture->teachersOverlap( **iterator )){
                        teacherFound = true;}
                        
                    //if original class and this class has common section(s), it's a clash
                    if(lecture->sectionsOverlap( **iterator )){
                        sectionFound = true;}
                        
                }
           }

            //no other class for one section
            if(sectionFound){
                constraints.at( classCounter + 2 ) = false;
            }

            //no other class for one teacher
            if(teacherFound){
                constraints.at( classCounter + 3 ) = false;
            }

        }
        
        classCounter +=
        Specs::getInstance().getNumberOfConstraints();

    }
}

double Schedule::addConstraintsWeights(){
    double fitness = 0.0;
    int totalScore = 0;

    for(const auto& i: constraints){
        totalScore += (int) i;
    }

    //fitness returned will be between 0 to 1 inclusive
    int numberOfConstraints = constraints.size();
    fitness = totalScore / (double) numberOfConstraints;
    return ( fitness );

}

/*
FOR TESTING PURPOSE ONLY
*/
void Schedule::printSchedule(bool full = false) const {

    cout << "fitness: " << fitness << endl;

    if(!full)   return;

    for(const auto& it: constraints){
            cout << it << " ";
    }
    cout << endl;

    int day, spaceTime, roomID, time;
    int totalHours      = Specs::getInstance().getTotalHours();
    int numberOfRooms   = Specs::getInstance().getNumberOfRooms();
    int weekDays        = Specs::getInstance().getWeekDays();
    int spaceTimePerDay = numberOfRooms * totalHours;

    int lectureCount = 0;

    for(int position = 0; position < slots.size(); position++){

        day         = position / spaceTimePerDay;
        spaceTime   = position % spaceTimePerDay;
        roomID      = spaceTime / totalHours;
        time        = spaceTime % totalHours;

        if( slots.at(position).empty() ){
            continue;
        }
        lectureCount++;

        if( slots.at(position).size() > 1){
            //IF working correctly this should never be printed
            cout << endl <<" MULTISLOT:" <<
            (
                slots.at(position).back() == slots.at(position).front()
            )
            << slots.at(position).size() << endl;

        }

        const Lecture *lecture = slots.at(position).front();

        cout    << "DAY: " << day+1
                << "ROOM: " << Specs::getInstance().getRoomById(roomID)->getName()
                << "SLOT: " << time+1
                << endl;

        cout << lecture->getId()
            << " "
            << lecture->getCourse().getTitle()
            << " "
            << lecture->getTeachers().front()->getName()
            << " "
            << lecture->getSections().front()->getName()
            << endl;

    }
    cout << "L: " << lectureCount << endl;

}

//crossover occurs by randomly swapping some classes between two schedules
//this is a little different from typical crossover implementations as
//no new offsprings are created. However it still works (uncited)
//might have to implement another

void crossover(Schedule& schedule1, Schedule& schedule2, int &seed){

    //check probability
    double random = r8_uniform_ab(0.0, 1.0, seed);
    if( random > Specs::getInstance().getCrossoverRate() ){
        return;
    }

    unordered_set<int> crossoverPoints;

    int totalClasses = schedule1.classes.size();
    int coPointsSize = Specs::getInstance().getCrossoverPoints();

    while( crossoverPoints.size() < coPointsSize ){
        //a set inserts unique elements auto
        int point = i4_uniform_ab(0, totalClasses - 1, seed);
        crossoverPoints.insert( point );
    }

    auto s1Classes  = schedule1.classes.begin();
    auto s2Classes  = schedule2.classes.begin();

    auto& slots1    = schedule1.slots;
    auto& slots2    = schedule2.slots;

    /*
    this decides whether to crossover or skip over classes
    */
    bool swap       = i4_uniform_ab(0, 1, seed);

    for(int i = 0; i < totalClasses;
        i++, s1Classes++, s2Classes++){
        
        //check if it's a crossover point
        auto coPIterator = crossoverPoints.find(i);
        if( coPIterator != crossoverPoints.end() ){
            swap != swap;
        }

        if( swap == false){
            continue;
        }

        //crossover starts here for one class

        //pointer to the lecture in both schedules (same)
        Lecture *lecture = s1Classes->first;

        int duration = lecture->getCourse().getDuration();

        //each iteration removes one slot of this lecture
        //from both schedules
        for(int j = 0; j < duration; j++){

            //get lists of lectures at schedule 1 & 2's slots
            int position1 = s1Classes->second.at(j);
            int position2 = s2Classes->second.at(j);

            list<Lecture*>& lecturesList1 = slots1.at( position1 );
            list<Lecture*>& lecturesList2 = slots2.at( position2 );

            //remove entry from both

            lecturesList1.remove(lecture);
            lecturesList2.remove(lecture);

        }

        //now swap entries in classes map and insert at the
        //new positions in each schedules' slots (vector)
        std::swap( s1Classes->second, s2Classes->second );

        //insert entries at new slots
        for(const auto& position1: s1Classes->second){
            slots1.at( position1 ).push_back( lecture );
        }
        for(const auto& position2: s2Classes->second){
            slots2.at( position2 ).push_back( lecture );
        }

    }

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
This is a non-friend non-member function to ensure safety and encapsulation
*/
bool compareSchedulesDesc(Schedule const *schedule1, Schedule const *schedule2){
    return (
        schedule1->getFitness() > schedule2->getFitness()
    );
}

bool compareSchedulesAsc(Schedule const *schedule1, Schedule const *schedule2){
    return (
        schedule1->getFitness() < schedule2->getFitness()
    );
}