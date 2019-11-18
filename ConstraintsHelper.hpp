#ifndef C_HELPER
#define C_HELPER

#include<vector>
#include<unordered_set>
using namespace std;

/*
HELPER FUNCTIONS FOR CHECK CONSTRAINTS
*/

//returns length of the longest consecutive subsequence
int longestConsecutive(const vector<int>& dailySlots){
    unordered_set<int> dailySlotsSet;
    int result = 0;

    //insert into set
    for(const auto& slot: dailySlots){
        dailySlotsSet.insert(slot);
    }
  
    /*
    check each subsequence & update max
    */
    for(const auto& slot: dailySlots){
        //check if previous slot exists otherwise this is the start
        if( dailySlotsSet.find( slot - 1 ) == dailySlotsSet.end() ){
            //check for next slots in sequence
            int j = slot;
            while ( dailySlotsSet.find(j) != dailySlotsSet.end() ){
                j++;
            }
            //update max
            result = max( result, j - slot );
           
        }
    }

    return result;
}

int maxConsecutive(const vector< vector<int> >& slotsByDay){
    int max = 0;
    for(const auto& slots: slotsByDay){
        int consecutive = longestConsecutive( slots );
        if( consecutive > max ){
            max = consecutive;
        }
    }
    return max;
}

int maxDaily(const vector< vector<int> >& slotsByDay){
    int max = 0;
    for(const auto& slots: slotsByDay){
        int daily = slots.size();
        if( daily > max ){
            max = daily;
        }
    }
    return max;
}

int weekDays(const vector< vector<int> >& slotsByDay){
    int days = 0;
    for(const auto& slots: slotsByDay){
        if( slots.size() ){
            days++;
        }
    }
    return days;
}

bool oneAtATime(const vector< vector<int> >& slotsByDay){
    unordered_set<int> slotsSet;
    for(const auto& slots: slotsByDay){
        slotsSet.clear();
        for(const auto& slot: slots){
            bool inserted = slotsSet.insert( slot ).second;
            if( !inserted ){
                return false;
            }
        }
    }
    return true;
}

#endif