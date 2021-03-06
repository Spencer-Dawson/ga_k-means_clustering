#ifndef HW8_H
#define HW8_H
#include <vector>
#include <algorithm>
#include "ga.h"
#include "cluster.h"
#include "gacluster.h"
#include "kmeans.h"

#define NUMBEROFCLUSTERS 5
#define NUMBEROFPOINTSPERCLUSTER 10
#define RADIUS .2
#define GAPOPULATIONSIZE 30
#define MUTATIONRATE .05
#define KMEANSPOPULATIONSIZE 10
#define GASELECTIONMETHOD bobby_bryant
class HW8
{
public:
    HW8();
    ~HW8();
    void setupClusters();
    void setupPoints();
    void setupGA();
    void setupKmeans();
    void updateScores();
    double getGABest(double**);
    double getKMeansBest(double**);
    double getGAAverage();
    double getKMeansAverage();
    double getGAWorst();
    double getKmeansWorst();
    void evolveGA();
    void trainKMeans();
    void getPointGeneratingClusters(double **);
    void getPoints(double **);

private:
    Cluster point_generating_clusters;
    vector<KMeans> k_means_clusters;
    GA<GACluster> ga;
    double** points;
    MTRand* random_number_generator;
};

#endif // HW8_H
