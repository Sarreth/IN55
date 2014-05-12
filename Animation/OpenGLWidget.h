#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include "Sun.h"


class OpenGLWidget : public QGLWidget
{
        //Q_OBJECT
        public:
                OpenGLWidget ( QWidget *parent, int largeur, int hauteur, CameraLibre *joueur, QVector3D positionCamera, QVector3D targetCamera, int taillSolX, int tailleSolY, Objet *listeObjet[], int nombreObjets, QString nomDeClasse = "" );
                OpenGLWidget ( QWidget *parent, CameraLibre *joueur, QVector3D positionCamera, QVector3D targetCamera,  int tailleSolX, int tailleSolY, Objet *listeObjets[], int nombreObjets, QString nomDeClasse);
                void initializeGL(); //initialisation des paramtres opengl, du fog, des textures,...
                void resizeGL ( int width, int height ); //est appele quand on cree le widget
                void paintGL(); //appelle apres lors des updateGL (quand on veut rafraichir)
                GLuint loadTexture ( QString filename, bool useMipMap); //chargement des textures
                static void dessinerRepere();

                void ConversionVecteursVersAngles();

                void loadSkybox();
                void drawSkybox();
                void drawSun();
                GLuint cube_map_texture_ID[6];
                QImage texture_image[6];

                bool _vueActive;

                QVector3D _positionCamera;
                QVector3D _cibleCamera;
                float _theta;
                float _phi;

                CameraLibre *p_joueur;
                QVector3D _positionJoueur; //variables temporaires de calcul, utilisÃ© seulement pour calculer la cible visÃ© par la camera (fonction ConversionVecteursVersAngles()...)
                QVector3D _targetJoueur;

                int _tailleSolX;
                int _tailleSolY;

                Objet *p_listeObjets[50]; //pointeur vers chaques objets
                int _nombreObjets; //information transmise par mainwindow
                QVector3D _positionObjet; //information que l'on va recuperer a laide du pointeur sur chaque objet
                QVector3D _orientationObjet;

                MD5Model g_model;
                int _nombreDeTextures;
                GLuint textureSol, textureJoueur;
                QString _nomDeClasse;

                Sun* sun;
                Material* __material;
                Shader* __shader;

                int a; //angle de rotation de la sphere de lumière
};


#endif // OPENGLWIDGET_H
