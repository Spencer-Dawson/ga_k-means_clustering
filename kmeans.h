#ifndef KMEANS_H
#define KMEANS_H
#include "cluster.h"

class KMeans : public Cluster
{
public:
    KMeans();
    KMeans(int, MTRand*);
    void adjustClusters(int, double**);
};

#endif // KMEANS_H
