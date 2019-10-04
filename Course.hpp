#ifndef COURSE
#define COURSE

#include<string>
using namespace std;

//describes a course which can be core/elective & theory/lab
class Course{
	public:
		
		Course(	int		id,
				string&	courseCode,
				string&	title,
				int		creditHours,
				string&	department,
				string&	courseDiscipline,
				bool	isCoreCourse)
			:	id(id),
				courseCode(courseCode),
				title(title),
				creditHours(creditHours),
				department(department),
				isCoreCourse(isCoreCourse){}
		
		int getId() const {return id;}

		const string& getCourseCode() const {return courseCode;}

		const string& getDepartment() const {return department;}
		
		const string& getSchool() const {return school;}
		
		const string& getTitle() const {return title;}

		const string& getShortTitle() const {return shortTitle;}

		int getCreditHours() const {return creditHours;}

		const vector<int>& getOfferedToBatches() const {return offeredToBatches;}

		bool getIsCoreCourse() const {return isCoreCourse;}
		
	private:
		
		int			id;					//unique course identifier
		string		courseCode;			//CS205
		string		department;			//CS OR EE OR BBA
		string		school;				//CS OR EE OR MG OR MT OR SS
		string		title;				//Computer Architecture
		string		shortTitle;			//CA, PROB, OS-LAB
		int			creditHours;		//1, 3, 4
		vector<int>	offeredToBatches;	//2017 & 2018
		bool		isCoreCourse;		//true = is a core course

		//facilities
		/*
		//need to add more like language lab and EE & BBA stuff
		*/
		bool		needsComputerLab;	//true = is a computer lab
		bool		needsElectricalLab;	//true = is an electrical lab
		bool		needsProjector;		//true = has a working projector
		bool		needsAllInOne;		//true = has a viper in class
	
};

#endif
