#include "cluster.h"

Cluster::Cluster()
{
    values=0;
    number_of_clusters=0;
    score=0;
}

Cluster::Cluster(int number_of_clusters_input, MTRand *random_number_generator)
{
    values=0;
    number_of_clusters=0;
    score=0;
    initClusters(number_of_clusters_input, random_number_generator);
}

Cluster::Cluster(const Cluster &rhs)
{
    number_of_clusters=rhs.number_of_clusters;
    values= new double*[number_of_clusters];
    for(int i=0; i<number_of_clusters; i++)
    {
        values[i]= new double[2];
        values[i][0]=rhs.values[i][0];
        values[i][1]=rhs.values[i][1];
    }
    score=rhs.score;
}

Cluster::~Cluster()
{
    clearClusters();
}

void Cluster::clearClusters()
{
    if(values !=0)
    {
        for(int i=0; i<number_of_clusters; i++)
            delete values[i];
        delete values;
        values=0;
    }
    number_of_clusters=0;
    score=0;
}

void Cluster::initClusters(int number_of_clusters_input, MTRand *random_number_generator)
{
    clearClusters();
    number_of_clusters=number_of_clusters_input;
    values = new double*[number_of_clusters];
    for(int i=0; i<number_of_clusters; i++)
        values[i]= new double[2];
    //do
    //{
        for(int i=0; i<number_of_clusters; i++)
        {
            values[i][0]=random_number_generator->rand(1);
            values[i][1]=random_number_generator->rand(1);
        }
    //}
    //while(clustersOverlap(.1));
}

void Cluster::moveCluster(int cluster_number, double cluster_x, double cluster_y)
{
    values[cluster_number][0]=cluster_x;
    values[cluster_number][1]=cluster_y;
}

bool Cluster::clustersOverlap(const Cluster &rhs,float radius=.3)
{
    if(number_of_clusters!=rhs.number_of_clusters)
        return false;
    bool hasmatch=true;
    for(int i=0; i<number_of_clusters && hasmatch==true; i++)
    {
        hasmatch=false;
        for(int j=0; j<number_of_clusters; j++)
        {
            if(abs(values[i][0]-rhs.values[j][0])<radius)
            {
                if(abs(values[i][0]-rhs.values[j][0])<radius)
                {
                    hasmatch=true;
                }

            }
        }
    }
    if(!hasmatch)
        return false;
    return true;
}

bool Cluster::clustersOverlap(float radius)
{
//
//returns true if clusters ovelap
//
    bool overlap=false;
    for(int i=0; i<number_of_clusters; i++)
    {
        for(int j=i+1; j<number_of_clusters; j++)
        {
            if(abs(values[i][0]-values[j][0])<radius)
            {
                if(abs(values[i][1]-values[j][1])<radius)
                {
                    //double temp_i_0=values[i][0];
                    //double temp_i_1=values[i][1];
                    //double temp_j_0=values[j][0];
                    //double temp_j_1=values[j][1];
                    overlap=true;
                }
            }
        }
    }
    return overlap;
}

Cluster &Cluster::operator =(const Cluster &rhs)
{
    clearClusters();
    number_of_clusters=rhs.number_of_clusters;
    values= new double*[number_of_clusters];
    for(int i=0; i<number_of_clusters; i++)
    {
        values[i]= new double[2];
        values[i][0]=rhs.values[i][0];
        values[i][1]=rhs.values[i][1];
    }
    score=rhs.score;
    return *this;
}

double Cluster::getClusterX(int cluster_number)
{
    return values[cluster_number][0];
}

double Cluster::getClusterY(int cluster_number)
{
    return values[cluster_number][1];
}

void Cluster::generatePoint(int cluster_number, double &x_output, double &y_output, double radius, MTRand *random_number_generator)
{
    x_output=random_number_generator->rand(2*radius)-radius+values[cluster_number][0];
    y_output=random_number_generator->rand(2*radius)-radius+values[cluster_number][1];
}

int Cluster::getClosestCluster(double x_input, double y_input)
{
    int closest_cluster=0;
    for(int i=1; i<number_of_clusters; i++)
    {
        if((abs(values[i][0]-x_input)+abs(values[i][1]-y_input)) < (abs(values[closest_cluster][0]-x_input) +abs(values[closest_cluster][1]-y_input)))
                closest_cluster=i;
    }
    return closest_cluster;
}

std::ostream &operator <<(std::ostream &output, const Cluster & rhs)
{
    for(int i=0; i< rhs.number_of_clusters; i++)
    {
        output <<"Cluster "<<i <<" is ";
        output <<rhs.values[i][0] <<" "<<rhs.values[i][1]<<" ";
    }
    return output;
}

bool Cluster::operator >(const Cluster &rhs) const
{
    return getScore()>rhs.getScore();
}

bool Cluster::operator <(const Cluster &rhs) const
{
    return getScore()<rhs.getScore();
}

bool Cluster::operator ==(const Cluster &rhs) const
{
    return getScore()==rhs.getScore();
}

int Cluster::getScore() const
{
    return score;
}

int Cluster::setScore(int number_of_points, double **input_points)
{
    //
    //sets the score
    //
        score=0;
        for(int i=0; i<number_of_points; i++)//for each point
        {
            double distance_between_point_and_closest;
            double distance_between_point_and_this;
            int closest_cluster_index;
            closest_cluster_index=0;
            //assign the point to it's closest cluster
            for(int j=1; j<number_of_clusters; j++)
            {
                distance_between_point_and_this=pow(input_points[i][0]-values[j][0],2);
                distance_between_point_and_this+=pow(input_points[i][1]-values[j][1],2);
                distance_between_point_and_this=sqrt(distance_between_point_and_this);

                distance_between_point_and_closest=pow(input_points[i][0]-values[closest_cluster_index][0],2);
                distance_between_point_and_closest+=pow(input_points[i][1]-values[closest_cluster_index][1],2);
                distance_between_point_and_closest=sqrt(distance_between_point_and_closest);
                if(distance_between_point_and_this<distance_between_point_and_closest)
                {
                    closest_cluster_index=j;
                }
                //add up all the distances
                score+=distance_between_point_and_closest;//*distance_between_point_and_closest;
            }
        }
        //now calculate the distance from each cluster to the nearest point and add that to the score
        for(int i=0; i<number_of_clusters; i++)
        {
            double distance_between_cluster_and_closest=0;
            double distance_between_cluster_and_this=0;
            int closest_point_index;
            closest_point_index=0;
            for(int j=1; j<number_of_points; j++)
            {
                distance_between_cluster_and_this=pow(input_points[j][0]-values[i][0],2);
                distance_between_cluster_and_this+=pow(input_points[j][1]-values[i][1],2);
                distance_between_cluster_and_this=sqrt(distance_between_cluster_and_this);

                distance_between_cluster_and_closest=pow(input_points[closest_point_index][0]-values[i][0],2);
                distance_between_cluster_and_closest+=pow(input_points[closest_point_index][1]-values[i][1],2);
                distance_between_cluster_and_closest=sqrt(distance_between_cluster_and_closest);

                if(distance_between_cluster_and_closest>distance_between_cluster_and_this)
                    closest_point_index=j;
            }
            //add in the distances
            score+=distance_between_cluster_and_closest;

        }
        //now adjust the score
        //score= 0-score;
        //and return the score
        return score;
}
