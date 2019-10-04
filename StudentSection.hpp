#ifndef STUDENTSECTION
#define STUDENTSECTION

#include<string>
#include<list>
using namespace std;

class CourseClass;

//describes a student section which can be parent(A/B) OR child(GR-1/GR-2)
class StudentSection{
	
	public:
		
		StudentSection(	int		id,
						string&	name,
						int		strength,
						int		batch,
						string&	department)
					:	id(id),
						name(name),
						strength(strength),
						batch(batch),
						department(department){}

		void addCourseClass(CourseClass* courseClass){
			courseClasses.push_back(courseClass);
		}
		
		int getId() const {return id;}
		
		const string& getName() const {return name;}
		
		int getStrength() const {return strength;}
		
		int getBatch() const {return batch;}

		const string& getDepartment() const {return department;}
		
		const list<CourseClass*>& getCourseClasses()
		const	{return courseClasses;}
	
	private:

	int					id;					//unique identifier for the section
	string				name;				//A, C OR GR-1, GR-2
	int					strength;			//Number of Students ~50
	int					batch;				//year of the batch's admission (2017)
	string				department;			//dept of the section (CS, EE, SE)
	list<CourseClass*>	courseClasses;	//course classes the section is registered in

};

#endif
