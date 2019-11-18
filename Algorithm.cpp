#include"Schedule.cpp"
#include"Algorithm.hpp"
#include"Specs.cpp"

#include<ctime>
#include<algorithm>

#define POPSIZE 100
#define MAXGEN  4000
#define BSTSZE  10
#define WSTSZE  10

#define MRATE   0.05
#define CRATE   0.65
#define MSIZE   2
#define CSIZE   2

Algorithm::Algorithm(
    const unordered_map<int, Room*>& rooms,
    const unordered_map<string, Course*>& courses,
    const unordered_map<string, Teacher*>& teachers,
    const unordered_map<string, StudentSection*>& sections,
    const vector<Lecture*>& lectures
):
    rooms(rooms),
    courses(courses),
    teachers(teachers),
    sections(sections),
    lectures(lectures)
{

        seed = time(nullptr);

        populationSize = POPSIZE;
        maxGenerations = MAXGEN;

        bestSize    = BSTSZE;
        worstSize   = WSTSZE;

        population.resize( populationSize );
        newPopulation.resize( populationSize );

}

//destructor
Algorithm::~Algorithm(){

    bestFitness = 0.0;
    avgFitness  = 0.0;
    
    for(auto& i: population){
        delete i;
    }

    for(auto& j: newPopulation){
        delete j;
    }

    population.clear();
    newPopulation.clear();

}

//initialize first gen with random schedules
void Algorithm::initialize(){

    for(int i = 0; i < populationSize; i++){
        delete population[i];
        population.at(i) = new Schedule(
            seed, MRATE, CRATE, MSIZE, CSIZE,
            rooms, courses, teachers, sections, lectures
        );
        population.at(i)->initialize();
    }

    currentGeneration = 0;

    calculateFitness();
}

//Main Execution of the algorithm
void Algorithm::run(){

    while(
        // population.front()->getFitness() < 1.0 &&
        // bestFitness < 1.0 &&
        currentGeneration < maxGenerations
        //also a time limit to break the loop
    ){
        reproduction();
        if(currentGeneration%100 == 0){
            cout << endl;
            cout << "gen: " << currentGeneration
            << " B: " << bestFitness
            << " A:" << avgFitness;
            cout << endl;
        }

        //population.front()->printSchedule(false);
        currentGeneration++;
    }
    //report best chromosome here?

    /*
    FOR TESTING PURPOSE ONLY
    */
    cout << "GEN: " << currentGeneration << endl;
    population.front()->printSchedule(true);
}

//calc avg and best fitness for this generation
//and set relativeFitness and cumProb
//the individual schedules in this population
//will have already calculated their fitness
void Algorithm::calculateFitness(){

    //partially sort best And Worst
    //Replace worst with new and set bestFitness
    sortBestAndWorst();

    double sum = 0.0;

    //add up fitness of every schedule
    for(const auto& i: population){
        sum += i->getFitness();
    }

    //sum / number = avg
    avgFitness =  (double) sum / populationSize;

    //set relative fitness for each schedule
    double currentCumProb = 0.0;
    for(auto& i: population){
        i->relativeFitness  = (double) i->getFitness() / sum;
        i->cumulativeProb   = currentCumProb + i->relativeFitness;
        currentCumProb      = i->cumulativeProb;
    }

}

//fitness propotionate selection returns index of schedule
int Algorithm::rouletteSelection(){

    double x = r8_uniform_ab ( 0.0, 1.0, seed );
    if( x < population.at(0)->cumulativeProb ) return 0;

    for (int j = 0; j < populationSize - 1; j++ ){
        if (
            x >= population.at(j)->cumulativeProb &&
            x < population.at(j + 1)->cumulativeProb
            ){
            return j+1;
            }
    }
    //to avoid compiler warnings
    return 0;
}

//tournament selection returns index of schedule
int Algorithm::tournamentSelection(int selectionPressure = 10){

    //index of schedule with max fitness within the participants
    int maxIndex = 0;
    //max fitness within the participants
    double maxFitness = 0.0;


    /*
    tournament selection with replacement
    */

    // for(int i = 0; i < selectionPressure; i++){
    //     //pick a random index
    //     int randex = i4_uniform_ab(0, populationSize - 1, seed);
    //     if( population[randex]->getFitness() > maxFitness){
    //         maxIndex    = randex;
    //         maxFitness  = population[randex]->getFitness();
    //     }
    // }


    /*
    No-replacement tournament selection
    */
    //number of schedules in this tournament
    unordered_set<int> participants;

    while( participants.size() < selectionPressure )
    {
        int y = i4_uniform_ab(0, populationSize - 1, seed);
        bool inserted = participants.insert(y).second;

        //skip to next iteration if it was not inserted
        if(!inserted)   continue;
        if( population[y]->getFitness() > maxFitness){
            maxIndex = y;
            maxFitness = population[y]->getFitness();
        }
    }

    return maxIndex;
}

void Algorithm::reproduction(){

    int one, two;

    //selects chromosome from current generation and adds them to the
    //new population (copy assignment) based on their fitness
    for(int i = 0; i < populationSize; i++){

        //pick a random chromosome's index based on fitness
        // one = rouletteSelection();
        one = tournamentSelection();

        //copy it to the new generation
        delete newPopulation[i];
        newPopulation.at(i) = new Schedule( *(population.at(one)) );

    }

    //selects two chromosomes from new population,
    //performs crossover and mutation on them,
    //and adds them back to the original population
    for(int j = bestSize; j < populationSize; j += 2){

        //pick two random chromosomes for crossover
        one     = i4_uniform_ab(0, populationSize - 1, seed);
        two     = i4_uniform_ab(0, populationSize - 1, seed);

        Schedule &schedule1 = *( newPopulation.at(one) );
        Schedule &schedule2 = *( newPopulation.at(two) );

        crossover(schedule1, schedule2, seed);

        //mutate both chromosomes
        schedule1.mutation();
        schedule2.mutation();

        //copy both back to original population
        *( population.at(j) )      = schedule1;
        *( population.at(j+1) )    = schedule2;

    }

    //calculate the fitness for updated population
    calculateFitness();

}

void Algorithm::sortBestAndWorst(){


    //place worst chromosomes at the end
    partial_sort(   population.rbegin(),
                    population.rbegin() + worstSize,
                    population.rend(),
                    compareSchedulesAsc);

    //replace them with new chromosomes
    {
        int i = 0; auto it = population.rbegin();
        for(; i < worstSize && it != population.rend();
            i++, it++){
            delete *( it );
            *( it ) = new Schedule(
                seed, MRATE, CRATE, MSIZE, CSIZE,
                rooms, courses, teachers, sections, lectures
            );
            (*it)->initialize();
        }

    }

    //place best chromosomes at the start
    partial_sort(   population.begin(),
                    population.begin() + bestSize,
                    population.end(),
                    compareSchedulesDesc);

    //assign best fitness of the first chromosome
    bestFitness = population.front()->getFitness();

}