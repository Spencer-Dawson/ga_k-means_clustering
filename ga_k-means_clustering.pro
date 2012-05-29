#-------------------------------------------------
#
# Project created by QtCreator 2012-04-09T17:57:02
#
#-------------------------------------------------

QT       += core gui

TARGET = ga_k-means_clustering
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cluster.cpp \
    gacluster.cpp \
    kmeans.cpp \
    ga_k-means_clustering.cpp \
    mypoint.cpp

HEADERS  += mainwindow.h \
    ga.h \
    cluster.h \
    gacluster.h \
    MersenneTwister.h \
    kmeans.h \
    ga_k-means_clustering.h \
    mypoint.h

FORMS    += mainwindow.ui
