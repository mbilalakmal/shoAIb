#include"Schedule.cpp"
#include"Algorithm.hpp"
#include"Specs.cpp"

#include<ctime>
#include<algorithm>

Algorithm::Algorithm()
    :seed( time(NULL) ) {

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
    for(auto& i: population){
        i = new Schedule(seed);
    }
    currentGeneration = 0;

    calculateFitness();
}

//Main Execution of the algorithm
void Algorithm::run(){
    while(
        currentGeneration < maxGenerations &&
        bestFitness < 1.0
        //also a time limit to break the loop
    ){

        reproduction();
        currentGeneration++;
    }
    //report best chromosome here?
}

//calc avg and best fitness for this generation
//and set relativeFitness and cumProb
//the individual schedules in this population
//will have already calculated their fitness
void Algorithm::calculateFitness(){
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

    //partially sort best And Worst
    sortBestAndWorst();

}

//fitness propotionate selection returns index of schedule
int Algorithm::selectForReproduction(){

    double x = r8_uniform_ab ( 0.0, 1.0, seed );
    if ( x < population[0]->cumulativeProb )  return 0;

    for (int j = 0; j < populationSize - 1; j++ ){
        if (
            x >= population[j]->cumulativeProb &&
            x < population[j+1]->cumulativeProb
            ){
            return j+1;
            }
    }
    //to avoid compiler warnings
    return 0;
}

void Algorithm::reproduction(){

    int one, two;

    //selects chromosome from current generation and adds them to the
    //new population (copy assignment) based on their fitness
    for(int i = 0; i < populationSize; i++){

        //pick a random chromosome's index based on fitness
        //pick again if it's index of a [worstSize] chromosome
        do{
            one = selectForReproduction();
        }while(one >= (populationSize - worstSize) );

        //copy it to the new generation
        *( newPopulation[i] ) = *( population[one] ) ;

    }

    //selects two chromosomes from new population,
    //performs crossover and mutation on them,
    //and adds them back to the original population
    for(int j = bestSize; j < populationSize; j += 2){

        //pick two random chromosomes for crossover
        one     = i4_uniform_ab(0, populationSize - 1, seed);
        two     = i4_uniform_ab(0, populationSize - 1, seed);

        Schedule &schedule1 = *( newPopulation[one] );
        Schedule &schedule2 = *( newPopulation[two] );

        crossOver(schedule1, schedule2, seed);

        //mutate both chromosomes
        schedule1.mutation(seed);
        schedule2.mutation(seed);

        //copy both back to original population
        *( population[j] )      = schedule1;
        *( population[j+1] )    = schedule2;

    }

    //calculate the fitness for updated population
    calculateFitness();

}

void Algorithm::sortBestAndWorst(){

    //partition vector based on bestSize and worstSize
    nth_element(population.begin(), population.begin() + bestSize, population.end(), compareSchedules );
    nth_element(population.begin(), population.begin() + worstSize, population.end(), compareSchedules );
    
    //partially sort for best and worst chromosomes
    sort(population.begin(), population.begin() + bestSize, compareSchedules);
    sort(population.end() - worstSize, population.end(), compareSchedules);

    //assign best fitness of the first chromosome
    bestFitness = population.front()->getFitness();

}