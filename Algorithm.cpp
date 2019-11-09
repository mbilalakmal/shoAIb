#include"Schedule.cpp"
#include"Algorithm.hpp"
#include"Specs.cpp"

#include<ctime>
#include<algorithm>

Algorithm::Algorithm(){

        seed = time(nullptr);

        populationSize = Specs::getInstance().getPopulationSize();
        maxGenerations = Specs::getInstance().getMaxGenerations();

        bestSize    = Specs::getInstance().getBestSize();
        worstSize   = Specs::getInstance().getWorstSize();

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
        population.at(i) = new Schedule(seed);
    }

    currentGeneration = 0;

    calculateFitness();
}

//Main Execution of the algorithm
void Algorithm::run(){

    while(
        population.front()->getFitness() < 1.0 &&
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
        one = rouletteSelection();
        // one = tournamentSelection();

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
        schedule1.mutation(seed);
        schedule2.mutation(seed);

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
            *( it ) = new Schedule(seed);
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