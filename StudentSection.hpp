#ifndef STUDENTSECTION
#define STUDENTSECTION

#include<string>
#include<list>
using namespace std;

class Lecture;

//describes an atomic student section (A1 OR C2)
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

		~StudentSection(){
			//does not delete the objects
			lectures.clear();
		}

		void addLecture(Lecture* lecture){
			lectures.push_back(lecture);
		}

		int getId() const {return id;}
		
		const string& getName() const {return name;}
		
		int getStrength() const {return strength;}
		
		int getBatch() const {return batch;}

		const string& getDepartment() const {return department;}
		
		const list<Lecture*>& getLectures()
		const	{return lectures;}
	
	private:

	int		id;			//unique identifier for the section
	string	name;		//A1, C2, etc
	int		strength;	//Number of Students ~25
	int		batch;		//year of the batch's admission (2017)
	string	department;	//dept of the section (CS, EE, SE)

	//course classes the section is enrolled in
	list<Lecture*>	lectures;	

};

#endif
