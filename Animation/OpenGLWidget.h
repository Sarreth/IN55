#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <GL/gl.h>
#include <GL/glu.h>

#include <cmath>
#include <vector>

#include <QtOpenGL/QGLWidget>
#include <QImage>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include <QKeyEvent>

#include "Coord3D.h"
#include "CameraLibre.h"

class OpenGLWidget : public QGLWidget
{
        //Q_OBJECT

        public:
                OpenGLWidget ( QWidget *parent, int largeur, int hauteur, CameraLibre *joueur, Coord3D positionCamera, Coord3D targetCamera, int taillSolX, int tailleSolY, Objet *listeObjet[], int nombreObjets, int typeCamera, QString nomDeClasse = "" );
                OpenGLWidget ( QWidget *parent, CameraLibre *joueur, Coord3D positionCamera, Coord3D targetCamera,  int tailleSolX, int tailleSolY, Objet *listeObjets[], int nombreObjets, int typeCamera, QString nomDeClasse);

                void setVueIsActive(bool active); //si on desactive la minicarte avec le menu
        private:
                void initializeGL(); //initialisation des paramtres opengl, du fog, des textures,...
                void resizeGL ( int width, int height ); //est appele quand on cree le widget
                void paintGL(); //appelle apres lors des updateGL (quand on veut rafraichir)
                GLuint loadTexture ( QString filename, bool useMipMap); //chargement des textures
                static void dessinerRepere();

                void ConversionVecteursVersAngles();

                bool _vueActive;

                int _typeDeCamera;
                Coord3D _positionCamera;
                Coord3D _cibleCamera;
                float _theta;
                float _phi;

                CameraLibre *p_joueur;
                Coord3D _positionJoueur; //variables temporaires de calcul, utilisÃ© seulement pour calculer la cible visÃ© par la camera (fonction ConversionVecteursVersAngles()...)
                Coord3D _targetJoueur;

                int _tailleSolX;
                int _tailleSolY;

                Objet *p_listeObjets[50]; //pointeur vers chaques objets
                int _nombreObjets; //information transmise par mainwindow
                Coord3D _positionObjet; //information que l'on va recuperer a laide du pointeur sur chaque objet
                Coord3D _orientationObjet;
                std::vector< std::vector< std::vector< float > > > *p_tableauVectrices;

                int _nombreDeTextures;
                GLuint texturesGun1, texturesGun2,textureSol, textureJoueur,textureVueInactive;
                QString _nomDeClasse;


                int a; //angle de rotation de la sphere de lumière
};


#endif // OPENGLWIDGET_H
