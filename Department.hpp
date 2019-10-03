#ifndef DEPARTMENT
#define DEPARTMENT

#include<string>
#include<list>
#include "Batch.hpp"

using namespace std;

//describes a department which has 4 batches
class Department{
	
	public:
		
		Department(	    int		id,
						string&	name,
						string& shortName)
					:	id(id),
						name(name),
						shortName(shortName),
                        strength(0){}

		void addBatch(Batch* batch){
			batches.push_back(batch);

            //Add batch strength to total strength
            strength += batch->getStrength();
		}
		
		int getId() const {return id;}
		
		const string& getName() const {return name;}
		
		const string& getShortName() const {return shortName;}
		
        int getStrength() const {return strength;}

		const list<Batch*>& getBatches()
		const	{return batches;}
	
	private:

	int		id;					//unique & assigned auto-inc
	string	name;				//Computer Science OR Electrical Eng
    string  shortName;          //CS OR SE OR EE
	int		strength;			//Number of Students ~1000
	
	list<Batch*> batches;	    //batches in that department
};

#endif
