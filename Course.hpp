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
				int		duration,
				string&	department,
				string&	school,
				bool	isCoreCourse)
			:	id(id),
				courseCode(courseCode),
				title(title),
				duration(duration),
				department(department),
				school(school),
				isCoreCourse(isCoreCourse){}
		
		int getId() const {return id;}

		const string& getCourseCode() const {return courseCode;}

		const string& getDepartment() const {return department;}
		
		const string& getSchool() const {return school;}
		
		const string& getTitle() const {return title;}

		const string& getShortTitle() const {return shortTitle;}

		int getDuration() const {return duration;}

		const vector<int>& getOfferedToBatches() const {return offeredToBatches;}

		bool getIsCoreCourse() const {return isCoreCourse;}

		bool getIsLabCourse() const { return isLabCourse;}

		bool getNeedsComputerLab() const {return needsComputerLab;}
		bool getNeedsElectricalLab() const {return needsElectricalLab;}
		bool getNeedsProjector() const {return needsProjector;}
		bool getNeedsComputer() const {return needsComputer;}
		
	private:
		
		int			id;					//unique course identifier
		string		courseCode;			//CS205
		string		department;			//CS OR EE OR BBA
		string		school;				//CS OR EE OR MG OR MT OR SS
		string		title;				//Computer Architecture
		string		shortTitle;			//CA, PROB, OS-LAB
		int			duration;		//1, 3, 4
		vector<int>	offeredToBatches;	//2017 & 2018
		bool		isCoreCourse;		//true = is a core course
		bool		isLabCourse;		//true = duration needs to
										//be continous and lab constraints apply

		//facilities
		/*
		//need to add more like language lab and EE & BBA stuff
		*/
		bool		needsComputerLab;	//true = is a computer lab
		bool		needsElectricalLab;	//true = is an electrical lab
		bool		needsProjector;		//true = has a working projector
		bool		needsComputer;		//true = has a viper in class
	
};

#endif
