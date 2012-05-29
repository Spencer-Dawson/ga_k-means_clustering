#include "gacluster.h"

GACluster::GACluster() :Cluster()
{
    //Cluster:Cluster();
}

GACluster::GACluster(int input_number_of_clusters, MTRand * random_number_generator) :Cluster( input_number_of_clusters, random_number_generator)
{
}

GACluster GACluster::onePointCrossover(const GACluster &rhs, MTRand *random_number_generator)
{
    GACluster  output;
    output.initClusters(number_of_clusters, random_number_generator);
    int crossover_point_cluster_index = random_number_generator->randInt(number_of_clusters-1);//index 0,number_of_clusters-1
    int crossover_point_x_y_index = random_number_generator->randInt(1);
    //time to start copying
    for(int i=crossover_point_cluster_index; i<number_of_clusters; i++)
        for(int j=crossover_point_x_y_index; j<2; j++)
            output.values[i][j]=rhs.values[i][j];
    return output;
}

GACluster GACluster::twoPointCrossover(const GACluster &, MTRand *)
{
//I might fix this later
    return *this;
}

GACluster GACluster::uniformCrossover(const GACluster &, MTRand *)
{
//I might fix this later
    return *this;
}
void GACluster::mutate(MTRand * random_number_generator, float mutation_rate)
{
    double mutate_if_less_then_mutation_rate;
    for(int i=0; i<number_of_clusters; i++)
    {
        mutate_if_less_then_mutation_rate = random_number_generator->rand();
        if(mutate_if_less_then_mutation_rate<=mutation_rate)
            values[i][0]+=random_number_generator->randNorm(0,.2);
        mutate_if_less_then_mutation_rate = random_number_generator->rand();
        if(mutate_if_less_then_mutation_rate<=mutation_rate)
            values[i][1]+=random_number_generator->randNorm(0,.2);
    }
}

