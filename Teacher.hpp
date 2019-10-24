#ifndef TEACHER
#define TEACHER

#include<string>
#include<list>
#include<vector>
using namespace std;

class CourseClass;

//describes a teacher which is assigned to one or more course classes
class Teacher{
	
	public:
		
		Teacher(	int		id,
					string& name,
					string&	department,
					bool	isSenior,
					bool	needsComputer,
					string	building,
					int		floor,
					vector<bool>	preferredSlots)
				: 	id(id),
					name(name),
					department(department),
					isSenior(isSenior),
					building(building),
					floor(floor){}

		~Teacher(){
			//does not delete the objects
			courseClasses.clear();
		}

		void addCourseClass(CourseClass* courseClass){
			courseClasses.push_back(courseClass);
		}
		
		int getId() const {return id;}
		
		const string& getName() const {return name;}
		
		const string& getDepartment() const {return department;}

		bool getIsSenior() const {return isSenior;}

		const string& getBuilding() const {return building;}

		int getFloor() const {return floor;}

		const list<CourseClass*>& getCourseClasses() const
			{return courseClasses;}
	
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
	list<CourseClass*>	courseClasses;
};

#endif
