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

		int getYearOffered() const {return yearOffered;}

		int getSemesterOffered() const {return semesterOffered;}

		bool getIsCoreCourse() const {return isCoreCourse;}
		
	private:
		
		int		id;					//unique & assigned auto-inc
		string	courseCode;			//CS205
		string	department;			//CS OR EE OR BBA
		string	school;				//CS OR EE OR MG OR MT OR SS
		string	title;				//Computer Architecture, Theory Of Automata
		string	shortTitle;			//CA, PROB, OS-LAB
		int		creditHours;		//1, 3, 4
		int		yearOffered;		//1, 2, 3, 4
		int		semesterOffered;	//1, 2, 8
		bool	isCoreCourse;		//true = is a core course

		//facilities
		/*
		//need to add more like language lab and EE & BBA stuff
		*/
		bool	isComputerLab;		//true = is a computer lab
		bool	isElectricalLab;	//true = is an electrical lab
		bool	hasProjector;		//true = has a working projector
		bool	hasAllInOne;		//true = has a viper in class
	
};

#endif
