#ifndef ROOM
#define ROOM

#include<string>
using namespace std;

//describes any generic room where a lecture/lab can be held
class Room{
	
	public:
		
		Room(	int		id,
				int		capacity,
				string&	name,
				string&	building,
				int		floor,
				bool	isComputerLab,
				bool	isElectricalLab,
				bool	hasComputer)
			:	id(id),
				name(name),
				capacity(capacity),
				building(building),
				floor(floor),
				isComputerLab(isComputerLab),
				isElectricalLab(isElectricalLab),
				hasComputer(hasComputer){}
		
		int getId() const {return id;}
		
		int getCapacity() const {return capacity;}

		const string& getName() const {return name;}
		
		const string& getBuilding() const {return building;}
		
		int getFloor() const {return floor;}

		bool getIsComputerLab() const {return isComputerLab;}

		bool getIsElectricalLab() const {return isElectricalLab;}

		bool getHasComputer() const {return hasComputer;}
		
	private:
		
		int		id;					//unique & auto-inc (starting 0)!!
		int		capacity;			//50, 100
		string	name;				//CR-10, R-109
		string	building;			//CS, EE
		int		floor;				//0, 1, 2(in EE)

		bool	isComputerLab;		//true = is a computer lab
		bool	isElectricalLab;	//true = is an electrical lab
		
		// bool	hasProjector;		//Not needed
		bool	hasComputer;		//true = has a viper in class
		
};

#endif
