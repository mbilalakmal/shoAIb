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
				bool	hasProjector,
				bool	hasAllInOne)
			:	id(id),
				name(name),
				capacity(capacity),
				building(building),
				floor(floor),
				isComputerLab(isComputerLab),
				isElectricalLab(isElectricalLab),
				hasProjector(hasProjector),
				hasAllInOne(hasAllInOne){}
		
		int getId() const {return id;}
		
		int getCapacity() const {return capacity;}

		const string& getName() const {return name;}
		
		const string& getBuilding() const {return building;}
		
		int getFloor() const {return floor;}

		bool getIsComputerLab() const {return isComputerLab;}

		bool getIsElectricLab() const {return isElectricalLab;}

		bool getHasProjector() const {return hasProjector;}

		bool getHasAllInOne() const {return hasAllInOne;}
		
	private:
		
		int		id;				//unique & assigned auto-inc
		int		capacity;		//50, 100
		string	name;			//CR-10, R-109
		string	building;		//CS, EE
		int		floor;			//0, 1, 2(in EE)

		//facilities
		/*
		//need to add more like language lab and EE & BBA stuff
		*/
		bool	isComputerLab;			//true = is a computer lab
		bool	isElectricalLab;		//true = is an electrical lab
		bool	hasProjector;			//true = has a working projector
		bool	hasAllInOne;			//true = has a viper in class
		
};

#endif
