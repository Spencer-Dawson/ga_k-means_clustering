#ifndef CLUSTER_H
#define CLUSTER_H
#include "MersenneTwister.h"

using namespace std;

class Cluster
{
public:
    Cluster();
    Cluster(int, MTRand*);
    Cluster(const Cluster&);
    ~Cluster();
    void clearClusters();
    void initClusters(int, MTRand*);
    void moveCluster(int, double, double);
    bool clustersOverlap(const Cluster&, float);
    bool clustersOverlap(float);
    Cluster& operator=(const Cluster&);
    double getClusterX(int);
    double getClusterY(int);
    void generatePoint(int, double&, double&, double, MTRand*);
    int getClosestCluster(double, double);
    friend std::ostream& operator<<(std::ostream &, const Cluster&);
    bool operator>(const Cluster&) const;
    bool operator<(const Cluster&) const;
    bool operator==(const Cluster&) const;
    int getScore() const;
    int setScore(int, double**);
protected:
    double ** values;
    int number_of_clusters;
    double score;
};

#endif // CLUSTER_H
