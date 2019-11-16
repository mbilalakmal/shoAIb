#ifndef TEACHER
#define TEACHER

#include<vector>
using namespace std;

class Lecture;

//maximum lectures a teacher can be assigned
#define MAX_T_LECT 10

//maximum lectures a teacher can teach consecutively
#define MAX_T_CONT_LECT 3

//maximum lectures a teacher can teach on a day
#define MAX_T_DAILY_LECT 4

/*
Describes a teacher with 5 attributes including assigned
lectures and available slots according to requirements & policies

CONSTRAINTS:
1. Day, Room, & Time assigned can be specified
2. Teacher must not have more than 2 consecutive classes
3. Teacher must have a day off
*/
class Teacher{
	
	public:
		
		Teacher(
			const string&	id,
			const string&	name,
			const string&	department,
			const vector<bool>&
							availableSlots
			
		):
			id(id),
			name(name),
			department(department),
			availableSlots(availableSlots)
		{
			lectures.reserve(MAX_T_LECT);
		}

		~Teacher(){
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
		
		const string& getDepartment() const {return department;}

		const vector<Lecture*>& getLectures() const {return lectures;}

		bool getAvailableSlot(int position) const {return availableSlots[position];}

	private:

	const string	id;
	const string	name;
	const string	department;

	//lectures assigned to this teacher
	vector<Lecture*>	lectures;

	const vector<bool>	availableSlots;
};

#endif
