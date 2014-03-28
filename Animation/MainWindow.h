#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPoint>
#include <QCheckBox>
#include <QMenuBar>
#include <QProgressBar>
#include <QWidget>
#include <QSpacerItem>
#include <QString>
#include <QFile>
#include <QTimer>
#include <iostream>
#include <QDebug>
#include <QStringList>

#include <GL/gl.h>
#include <GL/glu.h>

#include "Coord3D.h"
#include "OpenGLWidget.h"
#include "CameraLibre.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
private slots:
    void cycleTimerJeu();

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void lancerJeu();
    void stopperJeu();

    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent ( QKeyEvent * event );

    bool enPause;
    int _numCycle;
    bool rafraichirData;

    QRect mainScreenSize;

    OpenGLWidget *vuePrincipal;
    CameraLibre *joueur;

    int _tailleTerrainX;
    int _tailleTerrainY;
    int nombreObjets;
    Objet *liste_objets[50];

    QTimer *timerJeu;
    QTimer *timerFPS;

    bool anti_repetition;
    QPoint oldPosSouris;
};

#endif // MAINWINDOW_H
