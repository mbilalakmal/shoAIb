#ifndef ALGORITHM
#define ALGORITHM

#include"Schedule.hpp"

/*
The info related to uni (rooms, courses, etc) is accessed from
Specs::getInstance().getCourseClasses(), get___byId(T) etc

The info related to running algorithm (populationSize, generations, 
mutationRate, etc) is accessed from Specs::getInstance().getMutationRate() etc
*/

/*
STEPS TO RUN:

*/

//describes the supervisory part of the genetic algorithm
class Algorithm{

    public:

        //seed value for random numbers
        int seed;

        //average and best fitness of current generation
        double bestFitness;
        double avgFitness;

        Algorithm();

        ~Algorithm();

        //initialize the first generation with random schedules
        void initialize();

        //Main Execution of the algorithm
        void run();

        //return the chromosome with highest fitness
        const Schedule* getBestChromosome() const
            {return population.front();}

        //return current gen
        int getCurrentGeneration() const {return currentGeneration;}

    private:

        //current population of chromosomes
        vector<Schedule*> population;

        //next generation of chromosomes
        vector<Schedule*> newPopulation;

        //size of a population
        int populationSize;

        //count of generations
        int currentGeneration;

        //max number of generations
        int maxGenerations;

		//number of best chromosomes to track
		int		bestSize;

		//number of worst chromosomes to track
		int		worstSize;
        
        //calculate relative fitness for all schedules and get avg and best
        void calculateFitness();

        //(schedule index) fitness propotionate selection
        int rouletteSelection();

        // (schedule index) tournament selection (selection pressure)
        int tournamentSelection(int);

        /*
        (1)selects chromosome from current generation and adds
        them to the new population (copy assignment) based on their fitness
        (2)performs crossovers population/2 times
        (3)performs mutations on each crossovered chromosome in last step
        (4)insert them back into current generation (the old generation is lost)
        */
       void reproduction();

       //place best chromosomes at front & worst at the back
       void sortBestAndWorst();

};

#endif