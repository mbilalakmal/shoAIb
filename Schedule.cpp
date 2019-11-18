#include"Schedule.hpp"
#include"Random.hpp"
#include"ConstraintsHelper.hpp"
#include<vector>
#include<unordered_set>
#include<iostream>

using namespace std;

#define WEEKDAYS 5
#define TOTALHOURS 8

//default constructor
Schedule::Schedule(
    int seed,
    double mutationRate,
    double crossoverRate,
    int mutationSize,
    int crossoverSize,
    const unordered_map<int, Room*>& rooms,
    const unordered_map<string, Course*>& courses,
    const unordered_map<string, Teacher*>& teachers,
    const unordered_map<string, StudentSection*>& sections,
    const vector<Lecture*>& lectures
):
    seed(seed),
    mutationRate(mutationRate),
    crossoverRate(crossoverRate),
    mutationSize(mutationSize),
    crossoverSize(crossoverSize),
    rooms(rooms),
    courses(courses),
    teachers(teachers),
    sections(sections),
    lectures(lectures)
{
    //resize slots vector
    slots.resize( WEEKDAYS * TOTALHOURS * rooms.size() );

    // lectureConstraints.resize( lectures.size() );
    // teacherConstraints.resize( teachers.size() );
    // sectionConstraints.resize( sections.size() );

}

//copy constructor
Schedule::Schedule(const Schedule& schedule):
    seed(seed),
    mutationRate(mutationRate),
    crossoverRate(crossoverRate),
    mutationSize(mutationSize),
    crossoverSize(crossoverSize),
    rooms(schedule.rooms),
    courses(schedule.courses),
    teachers(schedule.teachers),
    sections(schedule.sections),
    lectures(schedule.lectures)
{
    //copy rhs entries to this schedule

    slots       = schedule.slots;
    classes     = schedule.classes;

    lectureConstraints = schedule.lectureConstraints;
    teacherConstraints = schedule.teacherConstraints;
    sectionConstraints = schedule.sectionConstraints;

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
    lectureConstraints.clear();
    teacherConstraints.clear();
    sectionConstraints.clear();
    fitness = 0;
}

//initialize schedule with random slots
void Schedule::initialize(){

    //get classes and place at random slots

    int totalRooms   = rooms.size();

    vector<int> positions;

    for(auto& lecture: lectures){

        int duration = lecture->getCourse().getDuration();
        positions.clear();
        positions.reserve(duration);

        if( lecture->getCourse().getIsLabCourse() ){
        
            int day     = i4_uniform_ab(0, WEEKDAYS - 1, seed);
            int room    = i4_uniform_ab(0, totalRooms - 1, seed);
            int time    = i4_uniform_ab(0, TOTALHOURS - duration, seed);
            
            //actual index for the vector[day*space*time]
            int position = time + TOTALHOURS * room + TOTALHOURS * totalRooms * day;

            //fill space-time slots for duration of lab
            for(int i = 0; i < duration; i++){
                // slots.at(position + i).push_back( lecture ); EDGE
                slots[ position+i ].push_back(lecture);
                positions.push_back( position + i );
            }

        }
        else{

            unordered_set<int> uniquePositions;

            while(uniquePositions.size() < duration){
                int position = i4_uniform_ab(0, WEEKDAYS * totalRooms * TOTALHOURS - 1, seed);
                bool inserted = uniquePositions.insert(position).second;
                if(inserted) {
                    // slots.at(position).push_back( lecture ); EDGE
                    slots[position].push_back(lecture);
                    positions.push_back(position);
                }
            }

        }

        classes.insert( make_pair(lecture, positions) );

    }

    //every courseClass is randomly placed in time-space slots

    //calculate fitness for newly created
    calculateFitness();

    relativeFitness = 0.0;
    cumulativeProb = 0.0;

}

//mutation occurs by randomly swapping some classes within a schedule
void Schedule::mutation(){

    // cout << "SOMUT" << endl;

    //coin flip
    double random = r8_uniform_ab(0.0, 1.0, seed);
    if ( random > mutationRate ){
        return;
    }

    int totalClasses = classes.size();
    int totalRooms = rooms.size();
    vector<int> currentPositions, newPositions;

    //for each mutation point select a class and change its position
    for(int i = 0; i < mutationSize; i++){

        //pick random class
        int mutationPosition = i4_uniform_ab(0, totalClasses - 1, seed);
        
        auto it = classes.begin();
        //move iterator to mutationPosition
        advance(it, mutationPosition);

        Lecture *lecture = it->first;

        int duration = lecture->getCourse().getDuration();
        currentPositions.clear();
        newPositions.clear();
        currentPositions.reserve(duration);
        newPositions.reserve(duration);


        //*Lab and Theory courses differ in the manner of relocation
        //*of their slots because lab slots should be contiguous

        if(lecture->getCourse().getIsLabCourse()){

            int currentPosition = it->second.front();

            for(int j = 0; j < duration; j++){

                //get list of classes at current slot
                vector<Lecture*>& lecturesList = slots.at( currentPosition + j );

                //remove target lecture
                lecturesList.erase(
                    remove( lecturesList.begin(), lecturesList.end(), lecture ),
                    lecturesList.end()
                );
            }

            //pick rando day, room, and time
            int day     = i4_uniform_ab(0, WEEKDAYS - 1, seed);
            int room    = i4_uniform_ab(0, totalRooms - 1, seed);
            int time    = i4_uniform_ab(0, TOTALHOURS - duration, seed);

            int newPosition = time + TOTALHOURS * room + TOTALHOURS * totalRooms * day;
        
            //relocate all slots
            for(int j = 0; j < duration; j++){

                //relocate at newPositions
                // slots.at(newPosition + j).push_back( lecture ); EDGE
                slots[ newPosition + j ].push_back(lecture);
                newPositions.push_back(newPosition + j);
            }

        }
        else{

            // cout << "SOEMUT" << endl;

            //get current positions of all slots
            currentPositions = it->second;

            //remove lecture from current positions
            for(int j = 0; j < duration; j++){
                int currentPosition = currentPositions.at(j);

                //get list of classes at current slot
                vector<Lecture*>& lecturesList = slots.at( currentPosition);

                //remove target lecture
                lecturesList.erase(
                    remove( lecturesList.begin(), lecturesList.end(), lecture ),
                    lecturesList.end()
                );
            }

            // cout << "MOEMUT" << endl;

            unordered_set<int> uniquePositions;

            //pick new positions and insert lecture at their slots
            while(uniquePositions.size() < duration){
                int newPosition = i4_uniform_ab(0, WEEKDAYS * totalRooms * TOTALHOURS - 1, seed);
                // cout << newPosition << endl;
                bool inserted = uniquePositions.insert(newPosition).second;
                if(inserted) {
                    // slots.at(newPosition).push_back( lecture ); EDGE
                    slots[ newPosition ].push_back(lecture);
                    newPositions.push_back(newPosition);
                }
            }

            // cout << "EOEMUT" << endl;

        }

        //change entries in classes map to new positions
        classes[lecture] = newPositions;

    }

    // cout << "MOMUT" << endl;

    //calc new fitness after mutation
    calculateFitness();

    // cout << "EOMUT" << endl;

}

//calculate score of schedule based on given constraints
void Schedule::calculateFitness(){

    //check each constraint for all classes and mark true/false
    checkConstraints();

    //add weighted contraints and return total fitness
    fitness = addConstraintsWeights();
}

void Schedule::checkConstraints() const{
    // cout << "SOC" << endl;

    //reset all constraints to zero
    lectureConstraints.clear();
    lectureConstraints.reserve(lectures.size());
    teacherConstraints.clear();
    teacherConstraints.reserve(teachers.size());
    sectionConstraints.clear();
    sectionConstraints.reserve(sections.size());

    int totalRooms = rooms.size();
    int spacetimePerDay = TOTALHOURS * totalRooms;

    //main lectures loop {R1, R2, R3, C1, T1}
    //sets bools for lectureConstraints
    for(const auto& classIterator: classes){

        const Lecture* lecture = classIterator.first;
        const Course& l_course = lecture->getCourse();
        const vector<Teacher*>& l_teachers = lecture->getTeachers();

        //constraints for this lecture (duration*5)
        vector< vector<bool> > _lectureConstraints;
        _lectureConstraints.reserve(l_course.getDuration());

        //each iteration runs for one slot of this lecture
        for(const auto& position: classIterator.second){

            //constraints for this slot of lecture (5)
            vector<bool> _durationConstraints(5, false);

            int day         = position / spacetimePerDay;
            int spacetime   = position % spacetimePerDay;
            int roomID      = spacetime / TOTALHOURS;
            int time        = spacetime % TOTALHOURS;

            Room* room      = getRoomById(roomID);

            /*
            ROOM1
            Multiple lectures at a time not allowed in room
            */
            if( slots[position].size() <= 1 ){
                _durationConstraints[0] = true;
            }

            /*
            ROOM2
            Lecture strength <= room capacity
            */
            if( room->getCapacity() >= lecture->getStrength() ){
                _durationConstraints[1] = true;
            }

            /*
            ROOM3
            Room must be available at this time & day
            */
            int dayTime = time + TOTALHOURS * day;
            if( room->getAvailableSlot(dayTime) ){
                _durationConstraints[2] = true;
            }

            /*
            COURSE1
            Course must be available at this time, room, & day
            */
            if( l_course.getAvailableSlot(position) ){
                _durationConstraints[3] = true;
            }

            /*
            TEACHER1
            Teacher(s) must be available at this time, room, & day
            */
           for(const auto& teacher: l_teachers){
               if( teacher->getAvailableSlot(position) ){
                   _durationConstraints[4] = true;
               }
           }

           //push these 5 bools to lecture's slot
           _lectureConstraints.push_back(_durationConstraints);

        }

        //push the duration*5 bools to lecture
        lectureConstraints.push_back( _lectureConstraints );

    }

    //main teachers loop {T2, T3, T4, T5}
    for(const auto& i: teachers){

        const Teacher* teacher = i.second;
        const auto& t_lectures = teacher->getLectures();

        vector<bool> _teacherConstraints(4, false);

        //teachers' slots seperated by day & impervious of room
        vector< vector<int> > slotsSansRoom(5);

        for(const auto& lecture: t_lectures){
            
            auto it = classes.at(lecture);

            for(const auto& position: it){
                int day         = position / spacetimePerDay;
                int spacetime   = position % spacetimePerDay;
                int time        = spacetime % TOTALHOURS;

                //insert in slotsSansRoom container
                slotsSansRoom[day].push_back(time);
            }

        }
        // cout << "IN"<<endl;
        // for(const auto& i: slotsSansRoom){
        //     for(const auto& j: i){
        //         cout << j << "  ";
        //     }
        //     cout << endl;
        // }
        // cout << "OUT"<<endl;

        /*
        TEACHER2
        Teacher must have <= 3 consecutive slots
        */
        int consecutive = maxConsecutive( slotsSansRoom );
        if( consecutive <= MAX_T_CONT_LECT ){
            _teacherConstraints[0] = true;
        }

        /*
        TEACHER3
        Teacher must have <= 4 daily slots
        */
        int daily = maxDaily( slotsSansRoom );
        if( daily <= MAX_T_DAILY_LECT ){
            _teacherConstraints[1] = true;
        }

        /*
        TEACHER4
        Teacher must have 1 day off
        */
        int weekdays = weekDays( slotsSansRoom );
        if( weekdays <= MAX_T_WEEKDAYS ){
            _teacherConstraints[2] = true;
        }

        /*
        TEACHER5
        Multiple lectures at a time not allowed for a teacher
        */
        if( oneAtATime( slotsSansRoom ) ){
            _teacherConstraints[3] = true;
        }

        //push the 4 bools to teacher
        teacherConstraints.push_back( _teacherConstraints );

    }

    //main sections loop {S1, S2, S3, S4, S5}
    for(const auto& i: sections){

        const StudentSection* section = i.second;
        const auto& s_lectures = section->getLectures();

        vector<bool> _sectionConstraints(5, false);

        //sections' slots seperated by day & impervious of room
        vector< vector<int> > slotsSansRoom(5);

        for(const auto& lecture: s_lectures){
            
            auto it = classes.at(lecture);

            for(const auto& position: it){
                int day         = position / spacetimePerDay;
                int spacetime   = position % spacetimePerDay;
                int time        = spacetime % TOTALHOURS;

                //insert in slotsSansRoom container
                slotsSansRoom[day].push_back(time);
            }

        }

        /*
        SECTION1
        section must have <= 5 consecutive slots
        */
        int consecutive = maxConsecutive( slotsSansRoom );
        if( consecutive <= MAX_S_CONT_LECT ){
            _sectionConstraints[0] = true;
        }

        /*
        SECTION2
        section must have <= 7 daily slots
        */
        int daily = maxDaily( slotsSansRoom );
        if( daily <= MAX_S_DAILY_LECT ){
            _sectionConstraints[1] = true;
        }

        /*
        SECTION3
        Only seniors can have day off
        */
        int weekdays = weekDays( slotsSansRoom );
        if( weekdays == WEEKDAYS || section->getBatch() == SENIOR_YEAR){
            _sectionConstraints[2] = true;
        }

        /*
        SECTION4
        section must have <= 1 lab in a day
        */
        // int weekdays = weekDays( slotsSansRoom );
        // if( weekdays == WEEKDAYS || section->getBatch() == SENIOR_YEAR){
        //     _sectionConstraints[2] = true;
        // }
        _sectionConstraints[3] = true;

        /*
        SECTION5
        Multiple lectures at a time not allowed for a section
        */
        if( oneAtATime( slotsSansRoom ) ){
            _sectionConstraints[4] = true;
        }

        //push the 5 bools to section
        sectionConstraints.push_back( _sectionConstraints );

    }

    // cout << "EOC" << endl;
}
/*
void Schedule::checkConstraints(){

    // for(const auto& teacher: teachers){

    // } RCTS



    int classCounter         = 0;

    //reset all constraints to TRUE(default)
    fill(constraints.begin(), constraints.end(), 0);

    int totalRooms   = rooms.size();
    int spaceTimePerDay = TOTALHOURS * totalRooms;

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
            int roomID      = spaceTime / TOTALHOURS;
            int time        = spaceTime % TOTALHOURS;

            Room* room      = getRoomById(roomID);

            //Check for INHERENT::ROOM constraints
            

            //no more than one class DONE
            if( slots.at(position).size() > 1 ){
                constraints.at( classCounter + 0 ) = false;
            }

            //capacity at least equal to strength DONE
            if( room->getCapacity() < lecture->getStrength() ){
                constraints.at( classCounter + 1 ) = false;
            }

            
            //check for INHERENT::SECTION & TEACHER constraints
            

            //each iteration runs for one room 
            for(int j = 0, dayTime = day * spaceTimePerDay + time;
                j < totalRooms && ( !sectionFound || !teacherFound );
                j++, dayTime += TOTALHOURS)
            {

                const vector<Lecture*>& ccList = slots.at(dayTime);

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
*/

double Schedule::addConstraintsWeights(){
    double fitness = 0.0;
    int totalScore = 0;
    int totalSize = 0;

    for(const auto& lecture: lectureConstraints){
        for(const auto& slot: lecture){
            for(const auto& constraint: slot){
                totalScore += (int) constraint;
                totalSize++;
            }
        }
    }

    for(const auto& teacher: teacherConstraints){
        for(const auto& constraint: teacher){
            totalScore += (int) constraint;
            totalSize++;
        }
    }

    for(const auto& section: sectionConstraints){
        for(const auto& constraint: section){
            totalScore += (int) constraint;
            totalSize++;
        }
    }

    //fitness returned will be between 0 to 1 inclusive
    fitness = totalScore / (double) totalSize;
    return (fitness);

}

//return pointer to a room OR nullptr if room doesn't exist
Room* Schedule::getRoomById(int id) const {
    auto it = rooms.find( id );
    return ( it != rooms.end() ? it->second: nullptr );
}

/*
FOR TESTING PURPOSE ONLY
*/
void Schedule::printSchedule(bool full = false) const {

    cout << "fitness: " << fitness << endl;

    if(!full)   return;

    cout << endl << "LCONST:" << endl;
    for(const auto& lecture: lectureConstraints){
        for(const auto& slot: lecture){
            for(const auto& constraint: slot){
                cout << constraint << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    cout << endl << "TCONST:" << endl;
    for(const auto& teacher: teacherConstraints){
        for(const auto& constraint: teacher){
            cout << constraint << " ";
        }
        cout << endl;
    }

    cout << endl << "SCONST:" << endl;
    for(const auto& section: sectionConstraints){
        for(const auto& constraint: section){
            cout << constraint << " ";
        }
        cout << endl;
    }

    int day, spaceTime, roomID, time;
    int totalRooms   = rooms.size();
    int spaceTimePerDay = totalRooms * TOTALHOURS;

    int lectureCount = 0;

    for(int position = 0; position < slots.size(); position++){

        day         = position / spaceTimePerDay;
        spaceTime   = position % spaceTimePerDay;
        roomID      = spaceTime / TOTALHOURS;
        time        = spaceTime % TOTALHOURS;

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
                << "ROOM: " << getRoomById(roomID)->getName()
                << "SLOT: " << time+1
                << endl;

        cout
            << lecture->getCourse().getCourseCode()
            << " "
            << lecture->getTeachers().front()->getName()
            << " "
            << lecture->getSections().front()->getName()
            << endl;

    }
    cout << "L: " << lectureCount << endl;

    for(const auto& lecture: lectures){
        cout << lecture->getId() << endl;
    }
    cout << endl;

    for(const auto& teacher: teachers){
        cout << teacher.second->getName();
        cout << endl;
        for(const auto& l: teacher.second->getLectures()){
            cout << l->getId() << endl;
        }
    }

}

//crossover occurs by randomly swapping some classes between two schedules
//this is a little different from typical crossover implementations as
//no new offsprings are created. However it still works (uncited)
//might have to implement another

void crossover(Schedule& schedule1, Schedule& schedule2, int &seed){

    //check probability
    double random = r8_uniform_ab(0.0, 1.0, seed);
    if( random > schedule1.crossoverRate ){
        return;
    }

    unordered_set<int> crossoverPoints;

    int totalClasses = schedule1.classes.size();
    int coPointsSize = schedule1.crossoverSize;

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

    for(int i = 0; i < totalClasses; i++, s1Classes++, s2Classes++){
        
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
            int position1 = s1Classes->second[j];
            int position2 = s2Classes->second[j];

            vector<Lecture*>& lecturesList1 = slots1[position1];
            vector<Lecture*>& lecturesList2 = slots2[position2];

            //remove entry from both
            lecturesList1.erase(
                    remove( lecturesList1.begin(), lecturesList1.end(), lecture ),
                    lecturesList1.end()
            );
            lecturesList2.erase(
                    remove( lecturesList2.begin(), lecturesList2.end(), lecture ),
                    lecturesList2.end()
                );

        }

        //now swap entries in classes map and insert at the
        //new positions in each schedules' slots (vector)
        std::swap( s1Classes->second, s2Classes->second );

        //insert entries at new slots
        for(const auto& position1: s1Classes->second){
            slots1[position1].push_back( lecture );
        }
        for(const auto& position2: s2Classes->second){
            slots2[position2].push_back( lecture );
        }

    }

}

//swaps two schedules, used in ass operator
void swap(Schedule& first, Schedule& second){
    
    using std::swap;

    swap(first.classes, second.classes);
    swap(first.slots, second.slots);
    swap(first.lectureConstraints, second.lectureConstraints);
    swap(first.teacherConstraints, second.teacherConstraints);
    swap(first.sectionConstraints, second.sectionConstraints);
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