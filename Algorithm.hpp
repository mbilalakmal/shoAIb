#ifndef ALGORITHM
#define ALGORITHM

#include"Schedule.hpp"

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

        Algorithm(
            const unordered_map<int, Room*>&,
            const unordered_map<string, Course*>&,
            const unordered_map<string, Teacher*>&,
            const unordered_map<string, StudentSection*>&,
            const vector<Lecture*>&
        );

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
		int bestSize;

		//number of worst chromosomes to track
		int worstSize;
        
        //Aggregated objects
            const unordered_map<int, Room*>
        rooms;
            const unordered_map<string, Course*>
        courses;
            const unordered_map<string, Teacher*>
        teachers;
            const unordered_map<string, StudentSection*>
        sections;
            const vector<Lecture*>
        lectures;

        //calculate relative fitness for all schedules and get avg and best
        void calculateFitness();

        //(schedule index) fitness propotionate selection
        int rouletteSelection();

        // (schedule index) tournament selection (selection pressure)
        int tournamentSelection(int);

        /*
        (1)selects best chromosomes from current population using selectionFunction
        (2)performs [populationSize/2] crossovers
        (3)performs mutations on each crossovered chromosome in step (2)
        (4)insert them back into current generation (the old generation is overwritten)
        */
       void reproduction();

       //place best chromosomes at front & worst at the back
       void sortBestAndWorst();

};

#endif