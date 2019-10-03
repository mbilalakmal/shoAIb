#ifndef BATCH
#define BATCH

#include<string>
#include<list>
#include "StudentSection.hpp"

using namespace std;

class Department;

//describes a batch within a dept which has student sections in it
class Batch{
	
	public:
		
		Batch(          int		id,
						int     year)
					:	id(id),
						year(year),
                        strength(0){}

		void addStudentSection(StudentSection* section){
			sections.push_back(section);

            //Add section strength to total strength
            strength += section->getStrength();
		}
		
		int getId() const {return id;}
		
		int getYear() const {return year;}

        int getStrength() const {return strength;}

		const list<StudentSection*>& getSections()
        const {return sections;}

        const Department& getDepartment() const {return *department;}
	
	private:

	int		id;					    //unique & assigned auto-inc
	int     year;                   //2017
	int		strength;			    //Number of Students ~700
	
	list<StudentSection*> sections;	//student sections in the batch

    Department* department;         //dept which the batch belongs to

};

#endif