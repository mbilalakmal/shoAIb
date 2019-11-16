#ifndef STUDENTSECTION
#define STUDENTSECTION

#include<vector>
using namespace std;

class Lecture;

//maximum lectures a section can be offered
#define MAX_S_LECT 16

//maximum lectures a section can attend consecutively
#define MAX_S_CONT_LECT 4

//maximum lectures a section can attend on a day
#define MAX_S_DAILY_LECT 7

/*
Describes a section with 6 attributes including offered lectures

CONSTRAINTS:
1. Only senior-year can have a day off
2. Section must not have more than one lab on any day
3. Section must not have zero free hours on any day
*/
class StudentSection{
	
	public:
		
		StudentSection(
			string			id,
			const string&	name,
			int				strength,
			int				batch,
			const string&	department
		):
			id(id),
			name(name),
			strength(strength),
			batch(batch),
			department(department)
		{
			lectures.reserve(MAX_S_LECT);
		}

		~StudentSection(){
			//this only removes the pointers
			//from vector without deleting
			lectures.clear();
		}

		//this will be called when creating lecture entries
		void addLecture(Lecture* lecture){
			lectures.push_back(lecture);
		}

		const string& getId() const {return id;}
		
		const string& getName() const {return name;}
		
		int getStrength() const {return strength;}
		
		int getBatch() const {return batch;}

		const string& getDepartment() const {return department;}
		
		const vector<Lecture*>& getLectures() const {return lectures;}
	
	private:

	const string	id;
	const string	name;
	const int		strength;
	const int		batch;
	const string	department;

	//lectures offered to this section
	vector<Lecture*>	lectures;	

};

#endif
