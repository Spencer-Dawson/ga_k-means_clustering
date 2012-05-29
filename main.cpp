#include <QtGui/QApplication>
#include "mainwindow.h"
#include "MersenneTwister.h"
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    MTRand random_number_generator;
    vector<double> random_number_list;
    random_number_list.resize(1000);
    for(int i=0; i<1000; i++)
        random_number_list[i]=random_number_generator.randNorm(0,1);
    make_heap(random_number_list.begin(), random_number_list.end());
    sort_heap(random_number_list.begin(), random_number_list.end());
    for(int i=0; i<1000; i++)
        cout << random_number_list[i]<<" ";
    int within_one_standard_deviation, within_two_standard_deviation, within_three_standard_deviation;
    within_one_standard_deviation=within_two_standard_deviation=within_three_standard_deviation=0;
    for(int i=0; i<1000; i++)
    {
        if(-3<random_number_list[i] && random_number_list[i]<3)//within 3 standard deviations
        {
            within_three_standard_deviation++;
            if(-2<random_number_list[i] && random_number_list[i]<2)//within 2 standard deviations
            {
                within_two_standard_deviation++;
                if(-1<random_number_list[i] && random_number_list[i]<1)//within 1 standard deviation
                {
                    within_one_standard_deviation++;
                }

            }
        }
    }
    cout <<endl;
    cout <<static_cast<float>(within_one_standard_deviation)/10<<"% Are within one standard deviation."<<endl;
    cout <<static_cast<float>(within_two_standard_deviation)/10<<"% Are within two standard deviations."<<endl;
    cout <<static_cast<float>(within_three_standard_deviation)/10<<"% Are within three standard deviations."<<endl;
    return a.exec();
}
