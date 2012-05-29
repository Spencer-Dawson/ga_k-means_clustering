#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    ga_100x_is_true=false;
    kmeans_100x_is_true=false;

    test_points=0;
    cluster_points=0;
    ga_cluster_points=0;
    kmeans_cluster_points=0;

    expiriment.setupClusters();
    updateClusterPoints();
    expiriment.setupPoints();
    updateTestPoints();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ga_gen_button_clicked()
{
    expiriment.setupGA();
    expiriment.updateScores();
    updateGAClusterPoints();
}

void MainWindow::on_t_u_ga_button_clicked()
{
    int runXtimes;
    if(ga_cluster_points!=0)
    {
        if(!ga_100x_is_true)
        {
            //run test once
            runXtimes=1;
        }
        else
        {
            //run test 100x
            runXtimes=100;
        }
        for(int i=0; i<runXtimes; i++)
        {
            expiriment.updateScores();
            expiriment.evolveGA();
        }
        expiriment.updateScores();
        updateGAClusterPoints();
        updateTestPoints();
    }
}

void MainWindow::on_kmeans_gen_button_clicked()
{
    expiriment.setupKmeans();
    expiriment.updateScores();
    updateKMeansClusterPoints();
}

void MainWindow::on_t_u_kmeans_button_clicked()
{
    int runXtimes;
    if(kmeans_cluster_points!=0)
    {
        if(!kmeans_100x_is_true)
        {
            //run test once
            runXtimes=1;
        }
        else
        {
            //run test 100x
            runXtimes=100;
        }
        //expiriment.updateScores();
        for(int i=0; i<runXtimes; i++)
        {
            expiriment.updateScores();
            expiriment.trainKMeans();
        }
        expiriment.updateScores();
        updateKMeansClusterPoints();
        updateTestPoints();
    }
}

void MainWindow::on_ga_100x_clicked(bool checked)
{
    ga_100x_is_true=checked;
}

void MainWindow::on_kmeans_100x_clicked(bool checked)
{
    kmeans_100x_is_true=checked;
}

void MainWindow::updateClusterPoints()
{
//
//updates the position of and displays the point generating clusters
//
    //first clear out old data (if necessary)
    if(cluster_points!=0)
    {
        for(int i=0; i< NUMBEROFCLUSTERS ; i++)
        {
            scene->removeItem(cluster_points[i]);
        }
        delete cluster_points;

    }
    //now make room for the new data
    cluster_points = new MyPoint*[NUMBEROFCLUSTERS];
    double ** temp_clusters;
    temp_clusters = new double*[NUMBEROFCLUSTERS];
    for(int i=0; i<NUMBEROFCLUSTERS; i++)
    {
        temp_clusters[i]= new double[2];
        cluster_points[i] = new MyPoint;
    }
    //now load the data into the clusters
    expiriment.getPointGeneratingClusters(temp_clusters);
    for(int i=0; i<NUMBEROFCLUSTERS; i++)
    {
        //double temp_x, temp_y;//debugging stuff
        //temp_x=temp_clusters[i][0];//debugging stuff
        //temp_y=temp_clusters[i][1];//debugging stuff
        cluster_points[i]->setPoint(temp_clusters[i][0]*300,temp_clusters[i][1]*300,Qt::red);
        scene->addItem(cluster_points[i]);
    }
    //cleanup leftover memory
    for(int i=0; i<NUMBEROFCLUSTERS; i++)
    {
        delete temp_clusters[i];
    }
    delete temp_clusters;
}

void MainWindow::updateTestPoints()
{
    //
    //updates the position of and displays the test points
    //
        //first clear out old data (if necessary)
        if(test_points!=0)
        {
            for(int i=0; i< NUMBEROFCLUSTERS*NUMBEROFPOINTSPERCLUSTER ; i++)
            {
                scene->removeItem(test_points[i]);
            }
            delete test_points;
        }
        //now make room for the new data
        test_points = new MyPoint*[NUMBEROFCLUSTERS*NUMBEROFPOINTSPERCLUSTER];
        double ** temp_points;
        temp_points = new double*[NUMBEROFCLUSTERS*NUMBEROFPOINTSPERCLUSTER];
        for(int i=0; i<NUMBEROFCLUSTERS*NUMBEROFPOINTSPERCLUSTER; i++)
        {
            temp_points[i]= new double[2];
            test_points[i] = new MyPoint;
        }
        //now load the data into the clusters
        expiriment.getPoints(temp_points);
        for(int i=0; i<NUMBEROFCLUSTERS*NUMBEROFPOINTSPERCLUSTER; i++)
        {
            double temp_x, temp_y;//debugging stuff
            temp_x=temp_points[i][0];//debugging stuff
            temp_y=temp_points[i][1];//debugging stuff
            test_points[i]->setPoint(temp_points[i][0]*300,temp_points[i][1]*300,Qt::blue);
            scene->addItem(test_points[i]);
        }
        //cleanup leftover memory
        for(int i=0; i<NUMBEROFCLUSTERS*NUMBEROFPOINTSPERCLUSTER; i++)
        {
            delete temp_points[i];
        }
        delete temp_points;
}

void MainWindow::updateGAClusterPoints()
{
    //
    //updates the position of and displays the GA clusters
    //
        //first clear out old data (if necessary)
        if(ga_cluster_points!=0)
        {
            for(int i=0; i< NUMBEROFCLUSTERS ; i++)
            {
                scene->removeItem(ga_cluster_points[i]);
                delete ga_cluster_points[i];
            }
            delete ga_cluster_points;
        }

        //now make room for the new data
        ga_cluster_points = new MyPoint*[NUMBEROFCLUSTERS];
        double ** temp_clusters;
        temp_clusters = new double*[NUMBEROFCLUSTERS];
        for(int i=0; i<NUMBEROFCLUSTERS; i++)
        {
            temp_clusters[i]= new double[2];
            ga_cluster_points[i] = new MyPoint;
        }
        //*
        //now load the data into the clusters
        double best = expiriment.getGABest(temp_clusters);
        //*
        for(int i=0; i<NUMBEROFCLUSTERS; i++)
        {
            //double temp_x, temp_y;//debugging stuff
            //temp_x=temp_clusters[i][0];//debugging stuff
            //temp_y=temp_clusters[i][1];//debugging stuff
            ga_cluster_points[i]->setPoint(temp_clusters[i][0]*300,temp_clusters[i][1]*300,Qt::green);
            scene->addItem(ga_cluster_points[i]);
        }
        //now append the scores to the text box//
        double worst=expiriment.getGAWorst();
        double average=expiriment.getGAAverage();
        QString buffer;
        buffer.append("ga best: ");
        buffer.append(QString("%1").arg(best));
        buffer.append(". ga average: ");
        buffer.append(QString("%1").arg(average));
        buffer.append(". ga worst: ");
        buffer.append(QString("%1").arg(worst));
        ui->textBrowser->append(buffer);
        //cleanup leftover memory

        for(int i=0; i<NUMBEROFCLUSTERS; i++)
        {
            delete temp_clusters[i];
        }
        delete temp_clusters;
        //*/
}

void MainWindow::updateKMeansClusterPoints()
{
    //
    //updates the position of and displays the k-means clusters
    //
        //first clear out old data (if necessary)
        if(kmeans_cluster_points!=0)
        {
            for(int i=0; i< NUMBEROFCLUSTERS ; i++)
            {
                scene->removeItem(kmeans_cluster_points[i]);
                delete kmeans_cluster_points[i];
            }
            delete kmeans_cluster_points;
        }

        //now make room for the new data
        kmeans_cluster_points = new MyPoint*[NUMBEROFCLUSTERS];
        double ** temp_clusters;
        temp_clusters = new double*[NUMBEROFCLUSTERS];
        for(int i=0; i<NUMBEROFCLUSTERS; i++)
        {
            temp_clusters[i]= new double[2];
            kmeans_cluster_points[i] = new MyPoint;
        }
        //*
        //now load the data into the clusters
        double best = expiriment.getKMeansBest(temp_clusters);
        //*
        for(int i=0; i<NUMBEROFCLUSTERS; i++)
        {
            //double temp_x, temp_y;//debugging stuff
            //temp_x=temp_clusters[i][0];//debugging stuff
            //temp_y=temp_clusters[i][1];//debugging stuff
            kmeans_cluster_points[i]->setPoint(temp_clusters[i][0]*300,temp_clusters[i][1]*300,Qt::green);
            scene->addItem(kmeans_cluster_points[i]);
        }
        //output the fitness info
        double average = expiriment.getKMeansAverage();
        double worst = expiriment.getKmeansWorst();
        QString buffer;
        buffer.append("k-means best: ");
        buffer.append(QString("%1").arg(best));
        buffer.append(". k-means average: ");
        buffer.append(QString("%1").arg(average));
        buffer.append(". k-means worst: ");
        buffer.append(QString("%1").arg(worst));
        ui->textBrowser->append(buffer);
        //cleanup leftover memory

        for(int i=0; i<NUMBEROFCLUSTERS; i++)
        {
            delete temp_clusters[i];
        }
        delete temp_clusters;
        //*/
}
