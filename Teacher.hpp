#ifndef TEACHER
#define TEACHER

#include<vector>
using namespace std;

class Lecture;

//maximum lectures a teacher can teach consecutively
#define MAX_T_CONT_LECT 3

//maximum lectures a teacher can teach on a day
#define MAX_T_DAILY_LECT 4

//maximum days a teacher has to show up
#define MAX_T_WEEKDAYS 4

#include"json.hpp"
using nlohmann::json;

/*
Describes a teacher with 5 attributes including assigned
lectures and available slots according to requirements & policies

CONSTRAINTS:
1. Day, Room, & Time assigned can be specified
2. Teacher must not have more than 2 consecutive classes
3. Teacher must have a day off
*/
class Teacher{
	
	/*
	these methods are used when creating teacher objects from json.
	The teacher must have a default constructor for this purpose
	*/
	friend void to_json(json& , const Teacher&);
	friend void from_json(const json&, Teacher&);

	public:

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

		string	id;
		string	name;
		string	department;

		//lectures assigned to this teacher
		vector<Lecture*> lectures;

		vector<bool> availableSlots;
};

void to_json(json& j, const Teacher& teacher) {
	j = json{
		{"id", teacher.id},
		{"name", teacher.name},
		{"department", teacher.department},
		{"availableSlots", teacher.availableSlots}
	};
}

void from_json(const json& j, Teacher& teacher) {
	j.at("id").get_to(teacher.id);
	j.at("name").get_to(teacher.name);
	j.at("department").get_to(teacher.department);
	j.at("availableSlots").get_to(teacher.availableSlots);
}

#endif
