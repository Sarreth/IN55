#ifndef COORD3D_H
#define COORD3D_H

#include <cmath>
#include <vector>
#include <iostream>

#include <QDesktopWidget>
#include <QString>
#include <QStringList>
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
#include <QFile>
#include <QTimer>
#include <QImage>
#include <QDebug>
#include <QSound>
#include <QTextStream>
#include <QMessageBox>
#include <QKeyEvent>


#include <QtOpenGL/QGLWidget>
#include <GL/gl.h>
#include <GL/glu.h>

class Coord3D
{
        public:
                float X;
                float Y;
                float Z;

                Coord3D();
                Coord3D ( float x,float y,float z );
                Coord3D ( const Coord3D &v );
                Coord3D ( const Coord3D &from,const Coord3D &to );

                Coord3D & operator= ( const Coord3D & v );

                Coord3D & operator+= ( const Coord3D & v );

                Coord3D operator+ ( const Coord3D & v ) const;
                Coord3D operator+ ( const Coord3D *v );
                Coord3D & operator+ (const float a);

                Coord3D & operator-= ( const Coord3D & v );
                Coord3D operator- ( const Coord3D & v ) const;
                Coord3D & operator- (const float a);

                Coord3D & operator*= ( const float a );
                Coord3D operator* ( const float a ) const;
                friend Coord3D operator* ( const float a,const Coord3D & v );

                Coord3D & operator/= ( const float a );
                Coord3D operator/ ( const float a ) const;

                Coord3D crossProduct ( const Coord3D & v ) const;
                float distanceDroiteAuPoint(const Coord3D & pointDeLaDroite, const Coord3D & point)const;
                float length() const;
                Coord3D & normalize();
};


#endif // COORD3D_H
