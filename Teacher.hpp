#ifndef TEACHER
#define TEACHER

#include<string>
#include<list>
using namespace std;

class CourseClass;

//describes a teacher which is assigned to one or more course classes
class Teacher{
	
	public:
		
		Teacher(int id,
				string& name)
			: 	id(id),
				name(name){}

		void addCourseClass(CourseClass* courseClass){
			courseClasses.push_back(courseClass);
		}
		
		int getId() const {return id;}
		
		const string& getName() const {return name;}
		
		const string& getDepartment() const {return department;}

		const list<CourseClass*>& getCourseClasses() const
			{return courseClasses;}
	
	private:

	int					id;				//unique identifier for the teacher
	string				name;			//full name of the teacher
	string				department;		//dept of the teacher
	
	list<CourseClass*>	courseClasses;	//courseClasses taught by the teacher
};

#endif
