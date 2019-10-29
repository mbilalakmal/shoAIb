#ifndef TEACHER
#define TEACHER

#include<string>
#include<list>
#include<vector>
using namespace std;

class Lecture;

//describes a teacher which is assigned to one or more course classes
class Teacher{
	
	public:
		
		Teacher(	int		id,
					string& name,
					string&	department,
					bool	isSenior,
					string	building,
					int		floor)
				: 	id(id),
					name(name),
					department(department),
					isSenior(isSenior),
					building(building),
					floor(floor){}

		~Teacher(){
			//does not delete the objects
			lectures.clear();
		}

		void addLecture(Lecture* lecture){
			lectures.push_back(lecture);
		}
		
		int getId() const {return id;}
		
		const string& getName() const {return name;}
		
		const string& getDepartment() const {return department;}

		bool getIsSenior() const {return isSenior;}

		const string& getBuilding() const {return building;}

		int getFloor() const {return floor;}

		const list<Lecture*>& getLectures() const
			{return lectures;}
	
	private:

	int		id;				//unique identifier for the teacher
	string	name;			//full name of the teacher
	string	department;		//dept of the teacher

	bool	isSenior;		//true = teacher is a doctor (phd) senior

	/*
	Teacher's location
	*/
	string	building;		//CS, EE
	int		floor;			//0, 1, 2(in EE)

	//course classes taught by the teacher
	list<Lecture*>	lectures;
};

#endif
