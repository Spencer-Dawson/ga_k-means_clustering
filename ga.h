#ifndef GA_H
#define GA_H

//#include"binarystring.h"
#include<vector>
#include <algorithm>
#include"MersenneTwister.h"
//#include "ga.cpp"
using namespace std;

enum SelectionType{
    roulette_wheel,
    bobby_bryant,
    eliteism,
    mutation_only
};
enum CrossoverType{
    one_point,
    two_point,
    uniform
};

template<class GAtype> class GA
{
public:
    GA();
    GA(SelectionType selection_input, CrossoverType crossover_input, int population_size_input, float mutation_rate_input);
    void initializeGA(SelectionType selection_input, CrossoverType crossover_input, int population_size_input, float mutation_rate_input);
    void selectCrossoverMutate();
    void printBestString();
    void printWorstString();
    double bestFitness();
    double worstFitness();
    double averageFitness();
    void setScores(int, double **);//
    GAtype getBest();
    //GAtype getWorst();
private:
    vector<GAtype> population;
    SelectionType selection;
    CrossoverType crossover;
    int population_size;
    float mutation_rate;
};

#endif // GA_H

template<typename GAtype>
GA<GAtype>::GA()
{
//
//Initializes GA with default values
//
    initializeGA(roulette_wheel, one_point, 100, .01);
}


template<typename GAtype>
GA<GAtype>::GA(SelectionType selection_input, CrossoverType crossover_input, int population_size_input, float mutation_rate_input)
{
//
//Initializes GA with given values
//
    initializeGA(selection_input, crossover_input, population_size_input, mutation_rate_input);
}


template<class GAtype>
void GA<GAtype>::initializeGA(SelectionType selection_input, CrossoverType crossover_input, int population_size_input, float mutation_rate_input)
{
//
//intitializes GA with given values
//
    selection=selection_input;
    crossover=crossover_input;
    population_size=population_size_input;
    mutation_rate=mutation_rate_input;

    population.resize(population_size);
    MTRand *random;
    random = new MTRand;
    for(int i=0; i<population_size; i++)
        population[i].initClusters(5,random);
    delete random;
}


template<class GAtype>
void GA<GAtype>::selectCrossoverMutate()
{
//
//Here is the meat and potatoes
//
    MTRand *random = new MTRand;
    if(selection==roulette_wheel)
    {
        double total_fitness=0;
        int *score, lowest=0;
        score= new int[population_size];
        for(int i=0; i<population_size; i++)
        {
            score[i]=population[i].getScore();
            if(score[i]<score[lowest])
                lowest=i;
            total_fitness+=score[i];
        }
        total_fitness=0;
        for(int i=0; i<population_size; i++)
        {
            score[i]=population[i].getScore()-population[lowest].getScore();
            score[i]*=score[i];
            total_fitness+=score[i];
        }
        vector<GAtype> roulette_wheel_actual;
        roulette_wheel_actual.resize(total_fitness);
        int roulette_wheel_index=0;
        for(int i=0; i<population_size;i++)
        {
            for(int j=0; j<score[i]; j++)
            {
                roulette_wheel_actual[roulette_wheel_index]=population[i];
                roulette_wheel_index++;
            }
        }
        if(roulette_wheel_index!=total_fitness)
            cout<<"r_w_i should be "<<total_fitness<<" but is "<<roulette_wheel_index<<"."<<endl;
        for(int i=0; i<total_fitness*4; i++)
        {
            int index1, index2;
            do
            {
                index1=random->randInt(total_fitness-1);
                index2=random->randInt(total_fitness-1);
            }
            while(index1==index2);
            GAtype temp=roulette_wheel_actual[index1];
            roulette_wheel_actual[index1]=roulette_wheel_actual[index2];
            roulette_wheel_actual[index2]=temp;
        }
        for(int i=0; i<population_size; i++)
        {
            //selection
            GAtype father(roulette_wheel_actual[random->randInt(total_fitness-1)]);
            GAtype mother(roulette_wheel_actual[random->randInt(total_fitness-1)]);
            //crossover
            population[i]=father;
            if(crossover==one_point)
                population[i].onePointCrossover(mother, random);
            if(crossover==two_point)
                population[i].twoPointCrossover(mother, random);
            if(crossover==uniform)
                population[i].uniformCrossover(mother, random);
            //mutation
            population[i].mutate(random, mutation_rate);
        }
        roulette_wheel_actual.clear();
        delete score;
        /*
        make_heap(population.begin(),population.end());
        sort_heap(population.begin(), population.end());
        //setting up for selection
        vector<GAtype> next_generation;
        next_generation.resize(population_size);
        double total_fitness=0;
        double sum_fitness=0;
        double *score;
        score= new double[population_size];
        for(int i=0; i<population_size; i++)
        {
            score[i]=population[i].getScore()*population[i].getScore();
            total_fitness+=score[i];
        }
        double* probability;
        probability= new double[population_size];
        for(int i=0; i<population_size; i++)
        {
            probability[i] = (score[i] / total_fitness);
            sum_fitness += probability[i];
            probability[i]=sum_fitness-probability[i];
            //cout <<"sum_fitness"<<sum_fitness<<" total_fitness "<<total_fitness<<endl;
        }
        for(int i=0; i<population_size; i++)
        {
            int parent_1, parent_2;
            parent_1=parent_2=population_size-1;
            double random_1 = random->randDblExc();
            double random_2 = random->randDblExc();
            GAtype father=population[population_size-1];
            GAtype mother=population[population_size-1];
            for(int j=0; j<population_size-1; j++)
            {
                if(random_1>probability[j])
                {
                    if(random_1<probability[j+1])
                    {
                        father=population[j];
                        parent_1=j;
                    }
                }
                if(random_2>probability[j])
                {
                    if(random_2<probability[j+1])
                    {
                        mother=population[j];
                        parent_2=j;
                    }
                }
            }
            GAtype child=father;
            if(crossover==one_point)
                child.onePointCrossover(mother, random);
            if(crossover==two_point)
                child.twoPointCrossover(mother, random);
            if(crossover==uniform)
                child.uniformCrossover(mother, random);
            child.mutate(random, mutation_rate);
            next_generation[i]=child;
            cout<<"Breeding "<<parent_1<<" with "<<parent_2<<"."<<endl;
        }
        //now replace the old generation
        population.clear();
        population.insert(population.begin(),next_generation.begin(),next_generation.end());
        next_generation.clear();
        delete score;
        delete probability;
        */
    }
    if(selection==eliteism)
    {
        make_heap(population.begin(),population.end());
        sort_heap(population.begin(), population.end());
        reverse(population.begin(),population.end());
        for(int i=0; i<population_size*.7; i++)
        {
            //selection
            GAtype father=population[.7*population_size+random->randInt(population_size*.3)];
            GAtype mother=population[.7*population_size+random->randInt(population_size*.3)];
            //crossover
            population[i]=father;
            if(crossover==one_point)
                population[i].onePointCrossover(mother, random);
            if(crossover==two_point)
                population[i].twoPointCrossover(mother, random);
            if(crossover==uniform)
                population[i].uniformCrossover(mother, random);
            //mutation
            population[i].mutate(random, mutation_rate);
        }

    }
    if(selection==bobby_bryant)
    {
        make_heap(population.begin(),population.end());
        sort_heap(population.begin(), population.end());
        reverse(population.begin(),population.end());
        for(int i=0; i<population_size; i++)
        {
            //selection
            GAtype father=population[i+random->randInt(population_size-(i+1))];
            GAtype mother=population[i+random->randInt(population_size-(i+1))];
            //crossover
            population[i]=father;
            if(crossover==one_point)
                population[i].onePointCrossover(mother, random);
            if(crossover==two_point)
                population[i].twoPointCrossover(mother, random);
            if(crossover==uniform)
                population[i].uniformCrossover(mother, random);
            //mutation
            population[i].mutate(random, mutation_rate);
        }

    }
    if(selection==mutation_only)
    {
        make_heap(population.begin(),population.end());
        sort_heap(population.begin(), population.end());
        reverse(population.begin(),population.end());
        for(int i=0; i<population_size-1; i++)
        {
            population[i]=population[population_size-1];
            population[i].mutate(random, mutation_rate);
        }

    }
    delete random;
}


template<class GAtype>
void GA<GAtype>::printBestString()
{
//
//prints the GAtype with the highest score
//
    int best_score_index=0;
    for(int i=0; i<population_size; i++)
        if(population[best_score_index]<population[i])
        {
            best_score_index=i;
        }
    cout <<population[best_score_index];
}


template<class GAtype>
void GA<GAtype>::printWorstString()
{
//
//prints the GAtype with the lowest score
//
        int worst_score_index=0;
        for(int i=0; i<population_size; i++)
            if(population[worst_score_index]>population[i])
            {
                worst_score_index=i;
            }
        cout <<population[worst_score_index];
}


template<class GAtype>
double GA<GAtype>::bestFitness()
{
//
//returns the score of the element with the highest fitness in the population
//
    double best_score_index=0;
    for(int i=0; i<population_size; i++)
        if(population[best_score_index]<population[i])
        {
            best_score_index=i;
        }
    return population[best_score_index].getScore();
}


template<class GAtype>
double GA<GAtype>::worstFitness()
{
//
//returns the score of the element with the lowest fitness in the population
//
    double worst_score_index=0;
    for(int i=0; i<population_size; i++)
        if(population[worst_score_index].getScore()<population[i].getScore())
        {
            worst_score_index=i;
        }
    return population[worst_score_index].getScore();
}


template<class GAtype>
double GA<GAtype>::averageFitness()
{
//
//simply returns the average score in the population
//
    double averageScore=0;
    for(int i=0; i<population_size; i++)
        averageScore+=population[i].getScore();
    averageScore/=population.size();
    return averageScore;
}

template<class GAtype>
void GA<GAtype>::setScores(int input_size, double** input)
{
    for(int i=0; i<population_size; i++)
    {
        population[i].setScore(input_size, input);
    }
}

template<class GAtype>
GAtype GA<GAtype>::getBest()
{
    make_heap(population.begin(),population.end());
    sort_heap(population.begin(), population.end());
    //GAtype temp=population.[population_size-1];
    //return temp;
    /*
    int best_GACluster=0;
    for(int i=1; i<population_size; i++)
    {
        if(population[i].getScore()<population[best_GACluster].getScore())
            best_GACluster=i;
    }
    return population[best_GACluster];
    */
    return population[0];
}
