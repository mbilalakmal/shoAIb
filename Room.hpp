#ifndef ROOM
#define ROOM

#include<vector>
using namespace std;

#include"json.hpp"
using nlohmann::json;

/*
Describes a room with 4 attributes including availableSlots
for room according to requirements and policies

CONSTRAINTS:
1. Day & Time available can be specified
2. Room capacity should be greater than students' strength
*/
class Room{
	
	/*
	these methods are used when creating room objects from json.
	The room must have a default constructor for this purpose
	*/
	friend void to_json(json& , const Room&);
	friend void from_json(const json&, Room&);

	public:
		
		Room():id(nextId++){}

		int getId() const {return id;}
		
		const string& getName() const {return name;}

		int getCapacity() const {return capacity;}

		bool getAvailableSlot(int position) const {return availableSlots[position];}

		//Reset ID counter to 0
		static void restartIDs() {nextId = 0;}
		
	private:

		const int	id;
		string		name;
		int			capacity;

		vector<bool> availableSlots;

		//RoomId counter to assign IDs
		static int	nextId;
		
};

//Init ID counter
int Room::nextId = 0;

void to_json(json& j, const Room& room) {
    j = json{
        {"name", room.name},
        {"capacity", room.capacity},
        {"availableSlots", room.availableSlots}
    };
}

void from_json(const json& j, Room& room) {
    j.at("name").get_to(room.name);
    j.at("capacity").get_to(room.capacity);
    j.at("availableSlots").get_to(room.availableSlots);
}

#endif
