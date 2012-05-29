#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <ga_k-means_clustering.h>
#include <mypoint.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void clearData();
    void clearGA();
    void clearKMeans();
    void updateClusterPoints();
    void updateTestPoints();
    void updateGAClusterPoints();
    void updateKMeansClusterPoints();
    
private slots:
    void on_ga_gen_button_clicked();

    void on_t_u_ga_button_clicked();

    void on_kmeans_gen_button_clicked();

    void on_t_u_kmeans_button_clicked();

    void on_ga_100x_clicked(bool checked);

    void on_kmeans_100x_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;

    bool ga_100x_is_true;
    bool kmeans_100x_is_true;
    ga_k_means_clustering expiriment;

    MyPoint** test_points;
    MyPoint** cluster_points;
    MyPoint** ga_cluster_points;
    MyPoint** kmeans_cluster_points;

};

#endif // MAINWINDOW_H
