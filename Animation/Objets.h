#ifndef OBJETS_H
#define OBJETS_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include <vector>
#include <QtOpenGL/QGLWidget>
#include <QImage>

#include <QDebug>
#include <QFile>
#include <QString>

#include <QSound>
#include <QStringList>
#include "Coord3D.h"

class Objet
{

    public:
        Objet(bool visible, Coord3D position, Coord3D orientation, bool possedeCollisionBox, Coord3D diagonaleBox, bool rotation90degBoundBox = false, QString nomObjet = "", QString fichierMesh = "", QString fichierTexture = "", bool textureUV = false, int repeatx = 1, int repeaty = 1);

        void genererTextureOpenGL(bool useMipMap); //car les fonctions de chargement de texture openGL ne peuvent Ãªtre appelÃ©
                                                   //qu'apres que le widget QGLWidget correspondant ait initialisÃ© les variables opengGL
                                                   // (ce qui est fait apres la creation de l'objet et qui explique que cette fonction ne soit pas
                                                   // a l'interieur de la fonction loadTexture(); )
        void afficherObjet();
        void dessinerBoundBox();
        int getNombreDeFaces();
        void setPossedeCollisionBox(bool possedeBox);
        bool getPossedeCollisionBox();


        void setIsVisible(bool visible);
        bool getIsVisible();
        void setPosition(Coord3D position);
        Coord3D getPosition();
        Coord3D getCollisionBox();
        QString getNom();


    protected: //protected pour etre accessible à la classe personnage, heritant d'objet
        void loadMesh();
        void loadTexture();

        QString _nomObjet;
        Coord3D _position;
        Coord3D _orientation;
        bool _possedeCollisionBox;
        Coord3D _collisionBox;
        bool _rotation90degCollisionBox; //si true, la boundbox dois etre tourné de 90 degré
        bool _isVisible; //faut-il l'afficher?


        //Variable pour le chargement du mesh et de la texture
        QString _fichierMesh;
        std::vector< std::vector< std::vector< float > > > _tableauMesh; //equivaut a float _tableauMesh[][][]  ; //le premier indice est le numero de la face, le deuxieme l'une des vectrices de la face (un des points), et le dernier la coordonnee (X,Y et Z);
        std::vector< std::vector< std::vector< float > > > _tableauTexture; //il va contenir les coordonnees des point texture du fichier obj
        std::vector< int > _nombreVectricesFace; //va contenir le nombre de vectrices de la face
        std::vector< std::vector< float > > _tableauxNormales; //va contenir les coord des normales de chaques faces
        int _nombreDeFaces;
        int _nombreDeVectrices;
        int _nombreDeCoordTexture;
        int _nombreDeCoordNormales;
        bool _contientNormalVect;
        bool _isTextureUVMap; // = true si le fichier obj contient toutes les informations sur la texture à appliquer
        int _repeatx; //si la texture n'est pas une UV-map, cb de fois on la repete
        int _repeaty;
        bool _chargementMeshOk;

        QString _fichierTexture;
        QImage _imageBase;
        QImage _imageTextureQT;
        GLuint _textureFinaleObjet;
        bool _chargementTextureOk;

};


#endif // OBJETS_H
