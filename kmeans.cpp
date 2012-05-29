#include "kmeans.h"

KMeans::KMeans()
{
}

KMeans::KMeans(int input_number_of_clusters, MTRand *random_number_generator) :Cluster( input_number_of_clusters, random_number_generator)
{
}

void KMeans::adjustClusters(int number_of_points, double **input_points)
{
    //
    //moves the cluster points acording to the KMeans algorithm
    //

    double ** error;
    error = new double*[number_of_clusters];
    for(int i=0; i<number_of_clusters; i++)
        error[i]= new double[2];

    for(int i=0; i<number_of_points; i++)//for each point
    {
        double distance_between_point_and_closest;
        double distance_between_point_and_this;
        int closest_cluster_index=0;
        //assign the point to it's closest cluster
        for(int j=1; j<number_of_clusters; j++)
        {
            distance_between_point_and_this=0;
            distance_between_point_and_this+=pow(input_points[i][0]-values[j][0],2);
            distance_between_point_and_this+=pow(input_points[i][1]-values[j][1],2);
            distance_between_point_and_this=sqrt(distance_between_point_and_this);

            distance_between_point_and_closest=0;
            distance_between_point_and_closest+=pow(input_points[i][0]-values[closest_cluster_index][0],2);
            distance_between_point_and_closest+=pow(input_points[i][1]-values[closest_cluster_index][1],2);
            distance_between_point_and_closest=sqrt(distance_between_point_and_closest);
            if(distance_between_point_and_this<distance_between_point_and_closest)
            {
                closest_cluster_index=j;
            }
        }
        //add the error for that point to the error for that cluster
        error[closest_cluster_index][0]+=input_points[i][0]-values[closest_cluster_index][0];
        error[closest_cluster_index][1]+=input_points[i][1]-values[closest_cluster_index][1];
    }

    //this helps prevent clusters from getting stuck
    //for each cluster assign it it's closest point and add that to the error
    for(int i=0; i<number_of_clusters; i++)
    {
        double distance_between_cluster_and_this;
        double distance_between_cluster_and_closest;
        int closest_index=0;
        for(int j=1; j<number_of_points; j++)
        {
            distance_between_cluster_and_closest=pow(input_points[closest_index][0]-values[i][0],2);
            distance_between_cluster_and_closest+=pow(input_points[closest_index][1]-values[i][1],2);
            distance_between_cluster_and_closest=sqrt(distance_between_cluster_and_closest);

            distance_between_cluster_and_this=pow(input_points[j][0]-values[i][0],2);
            distance_between_cluster_and_this+=pow(input_points[j][1]-values[i][1],2);
            distance_between_cluster_and_this=sqrt(distance_between_cluster_and_this);

            if(distance_between_cluster_and_this < distance_between_cluster_and_closest)
                closest_index=j;
        }
        error[i][0]+=input_points[closest_index][0]-values[i][0];
        error[i][1]+=input_points[closest_index][1]-values[i][1];
    }

    //now adjust the centers according to the errors
    for(int i=0; i<number_of_clusters; i++)
    {
        for(int j=0; j<2; j++)
            values[i][j]+=error[i][j]/(number_of_points/number_of_clusters);
    }
    for(int i=0; i<number_of_clusters; i++)
        delete error[i];
    delete error;
}
