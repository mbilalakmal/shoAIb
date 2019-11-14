#ifndef ROOM
#define ROOM

#include<vector>
using namespace std;

/*
Describes a room with 4 attributes including availableSlots
for room according to requirements and policies

CONSTRAINTS:
1. Day & Time available can be specified
2. Room capacity should be greater than students' strength
*/
class Room{
	
	public:
		
		Room(
			const string&	name,
			int				capacity,
			const vector<bool>&
							availableSlots
		):
			id(nextId++),
			name(name),
			capacity(capacity),
			availableSlots(availableSlots)
		{}

		int getId() const {return id;}
		
		const string& getName() const {return name;}

		int getCapacity() const {return capacity;}

		const vector<bool>& getAvailableSlots() const {return availableSlots;}

		//Reset ID counter to 0
		static void restartIDs() {nextId = 0;}
		
	private:

		const int		id;
		const string	name;
		const int		capacity;

		const vector<bool>	availableSlots;

		//RoomId counter to assign IDs
		static int		nextId;
		
};

//Init ID counter
int Room::nextId = 0;

#endif
