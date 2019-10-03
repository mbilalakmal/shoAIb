#ifndef STUDENTSECTION
#define STUDENTSECTION

#include<string>
#include<list>
using namespace std;

class CourseClass;

class Batch;

//describes a student section which can be parent(A/B) OR child(GR-1/GR-2)
class StudentSection{
	
	public:
		
		StudentSection(	int		id,
						string&	name,
						int		strength,
						Batch*	batch)
					:	id(id),
						name(name),
						strength(strength),
						batch(batch){}

		void addCourseClass(CourseClass* courseClass){
			courseClasses.push_back(courseClass);
		}
		
		int getId() const {return id;}
		
		const string& getName() const {return name;}
		
		int getStrength() const {return strength;}
		
		const list<CourseClass*>& getCourseClasses()
		const	{return courseClasses;}

		const Batch& getBatch() const {return *batch;}
	
	private:

	int				id;					//unique & assigned auto-inc
	string			name;				//A, C OR GR-1, GR-2
	int				strength;			//Number of Students ~50
	
	list<CourseClass*> courseClasses;	//course classes the section is registered in

	Batch*			batch;				//the batch which the section belongs to

};

#endif
