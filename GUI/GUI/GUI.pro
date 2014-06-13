#-------------------------------------------------
#
# Project created by QtCreator 2014-06-09T15:25:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        ../../Segmentor/segmentor.cpp

HEADERS  += mainwindow.h\
         ../../Segmentor/segmentor.hpp

FORMS    += mainwindow.ui

LIBS += `pkg-config opencv --libs` -L"/vol/bitbucket/ml3613/opencv/release/lib" -lopencv_nonfree

INCLUDEPATH += "/vol/bitbucket/ml3613/opencv/modules/nonfree/include/"\
               "/vol/bitbucket/ml3613/opencv/modules/core/include"
