#ifndef GACLUSTER_H
#define GACLUSTER_H
#include "cluster.h"

class GACluster : public Cluster
{
public:
    GACluster();
    GACluster(int, MTRand*);
    GACluster(const Cluster&);
    GACluster onePointCrossover(const GACluster&, MTRand*);
    GACluster twoPointCrossover(const GACluster&, MTRand*);
    GACluster uniformCrossover(const GACluster&, MTRand*);
    void mutate(MTRand*, float);
};

#endif // GACLUSTER_H
