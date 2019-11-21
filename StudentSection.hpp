#ifndef STUDENTSECTION
#define STUDENTSECTION

#include<vector>
using namespace std;

class Lecture;

//maximum lectures a section can attend consecutively
#define MAX_S_CONT_LECT 5

//maximum lectures a section can attend on a day
#define MAX_S_DAILY_LECT 7

//this batch can have a day off
#define SENIOR_YEAR 2016

#include"json.hpp"
using nlohmann::json;

/*
Describes a section with 6 attributes including offered lectures

CONSTRAINTS:
1. Only senior-year can have a day off
2. Section must not have more than one lab on any day
3. Section must not have zero free hours on any day
*/
class StudentSection{
	
	/*
	these methods are used when creating section objects from json.
	The section must have a default constructor for this purpose
	*/
	friend void to_json(json& , const StudentSection&);
	friend void from_json(const json&, StudentSection&);

	public:

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

		string	id;
		string	name;
		int		strength;
		int		batch;
		string	department;

		//lectures offered to this section
		vector<Lecture*>	lectures;	

};

void to_json(json& j, const StudentSection& section) {
	j = json{
		{"id", section.id},
		{"name", section.name},
		{"strength", section.strength},
		{"batch", section.batch},
		{"department", section.department},
	};
}

void from_json(const json& j, StudentSection& section) {
	j.at("id").get_to(section.id);
	j.at("name").get_to(section.name);
	j.at("strength").get_to(section.strength);
	j.at("batch").get_to(section.batch);
	j.at("department").get_to(section.department);
}

#endif
