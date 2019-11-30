#ifndef ROOM
#define ROOM

#include <string>           //string
#include <vector>           //vector
#include <unordered_map>    //unordered_map
#include "json.hpp"         //nlohmann::json

typedef std::vector<std::vector<bool>> AVAILABOOL;

/*
Describes a room where lectures can be scheduled

CONSTRAINTS:
1. Day & Time availability can be specified
2. Room capacity should be greater than lecture's strength
*/
class Room{

    //creates C++ Room object from json Room object
    friend void from_json(const nlohmann::json&, Room&);

    public:

        const std::string& getId() const {return id;}

        const std::string& getName() const {return name;}

        int getCapacity() const {return capacity;}

        const AVAILABOOL& getAvailableSlots() const {return availableSlots;}

    private:

        std::string id;
        std::string name;
        int capacity;

        AVAILABOOL availableSlots;

};

void from_json(const nlohmann::json& jroom, Room& room){
    jroom.at("id").get_to(room.id);
    jroom.at("name").get_to(room.name);
    jroom.at("capacity").get_to(room.capacity);
    jroom.at("availableSlots").get_to(room.availableSlots);
}

#endif