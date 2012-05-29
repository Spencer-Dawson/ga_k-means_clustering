#include "ga_k-means_clustering.h"

ga_k_means_clustering::ga_k_means_clustering()
{
    random_number_generator = new MTRand;
    setupClusters();
    points=0;
}

ga_k_means_clustering::~ga_k_means_clustering()
{
    if(random_number_generator!=0)
    {
        delete random_number_generator;
    }
    if(points !=0)
    {
        for(int i=0; i<NUMBEROFCLUSTERS*NUMBEROFPOINTSPERCLUSTER; i++)
            delete points[i];
        delete points;
    }
}

void ga_k_means_clustering::setupClusters()
{
    do
    {
    point_generating_clusters.initClusters(NUMBEROFCLUSTERS, random_number_generator);
    }
    while(point_generating_clusters.clustersOverlap(RADIUS));
}

void ga_k_means_clustering::setupPoints()
{
    //clear out old data if necessary
    if(points !=0)
    {
        for(int i=0; i<NUMBEROFCLUSTERS*NUMBEROFPOINTSPERCLUSTER; i++)
        {
            delete points[i];
        }
        delete points;
    }
    points= new double*[NUMBEROFCLUSTERS*NUMBEROFPOINTSPERCLUSTER];
    for(int i=0; i<NUMBEROFCLUSTERS*NUMBEROFPOINTSPERCLUSTER; i++)
        points[i]= new double[2];
    for(int i=0, j=0; i<NUMBEROFCLUSTERS; i++)
    {
        for(int k=0; k<NUMBEROFPOINTSPERCLUSTER; j++, k++)
        {
            point_generating_clusters.generatePoint(i,points[j][0],points[j][1],RADIUS,random_number_generator);
        }
    }
}

void ga_k_means_clustering::setupGA()
{
    //make the population
    ga.initializeGA(GASELECTIONMETHOD, one_point, GAPOPULATIONSIZE, MUTATIONRATE);
    //updateScores();
}

void ga_k_means_clustering::setupKmeans()
{
    k_means_clusters.resize(KMEANSPOPULATIONSIZE);
    for(int i=0; i<KMEANSPOPULATIONSIZE; i++)
        k_means_clusters[i].initClusters(NUMBEROFCLUSTERS, random_number_generator);
}

void ga_k_means_clustering::updateScores()
{
    //for the GA first
    for(int i=0; i<GAPOPULATIONSIZE; i++)
    {
        ga.setScores(NUMBEROFCLUSTERS*NUMBEROFPOINTSPERCLUSTER, points);
    }
    //now for the KMeans
    if(k_means_clusters.size()!=0)
    {
        for(int i=0; i<KMEANSPOPULATIONSIZE; i++)
            k_means_clusters[i].setScore(NUMBEROFCLUSTERS*NUMBEROFPOINTSPERCLUSTER, points);
        make_heap(k_means_clusters.begin(),k_means_clusters.end());
        sort_heap(k_means_clusters.begin(),k_means_clusters.end());
    }
}

double ga_k_means_clustering::getGABest(double **output)
{
    //
    //returns an array filled with the x/y values of the best GA points
    //
    GACluster temp_cluster(NUMBEROFCLUSTERS,  random_number_generator);
    temp_cluster= ga.getBest();
        for(int i=0; i<NUMBEROFCLUSTERS; i++)
        {
            output[i][0]=temp_cluster.getClusterX(i);
            output[i][1]=temp_cluster.getClusterY(i);
        }
        return temp_cluster.getScore();
}

double ga_k_means_clustering::getKMeansBest(double ** output)
{
    //
    //returns an array filled with the x/y values of the best KMeans points
    //
    KMeans temp_cluster(NUMBEROFCLUSTERS,  random_number_generator);

    temp_cluster= k_means_clusters[0];
        for(int i=0; i<NUMBEROFCLUSTERS; i++)
        {
            output[i][0]=temp_cluster.getClusterX(i);
            output[i][1]=temp_cluster.getClusterY(i);
        }
        return temp_cluster.getScore();
}

double ga_k_means_clustering::getGAAverage()
{
    return ga.averageFitness();
}

double ga_k_means_clustering::getKMeansAverage()
{
    double average=0;
    for(int i=0; i<KMEANSPOPULATIONSIZE; i++)
    {
        average+=k_means_clusters[i].getScore();
    }
    average/=KMEANSPOPULATIONSIZE;
    return average;
}

void ga_k_means_clustering::getPointGeneratingClusters(double** output)
{
//
//returns an array filled with the x/y values of the cluster points
//
    for(int i=0; i<NUMBEROFCLUSTERS; i++)
    {
        output[i][0]=point_generating_clusters.getClusterX(i);
        output[i][1]=point_generating_clusters.getClusterY(i);
    }
}

void ga_k_means_clustering::getPoints(double ** output)
{
    if(points!=0)
    {
        for(int i=0; i<NUMBEROFCLUSTERS*NUMBEROFPOINTSPERCLUSTER; i++)
        {
            output[i][0]=points[i][0];
            output[i][1]=points[i][1];
        }
    }
}

void ga_k_means_clustering::evolveGA()
{
    //updateScores();
    ga.selectCrossoverMutate();
}

void ga_k_means_clustering::trainKMeans()
{
    for(int i=0; i<KMEANSPOPULATIONSIZE; i++)
    {
        //updateScores();
        k_means_clusters[i].adjustClusters(NUMBEROFCLUSTERS*NUMBEROFPOINTSPERCLUSTER, points);
    }
}

double ga_k_means_clustering::getGAWorst()
{
    return ga.worstFitness();
}

double ga_k_means_clustering::getKmeansWorst()
{
    int worst_score_index=0;
    for(int i=1; i<KMEANSPOPULATIONSIZE; i++)
    {
        if(k_means_clusters[i].getScore()>k_means_clusters[worst_score_index].getScore())
            worst_score_index=i;
    }
    return k_means_clusters[worst_score_index].getScore();
}

