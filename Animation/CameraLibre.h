#ifndef CAMERALIBRE_H
#define CAMERALIBRE_H

#include "src/objects/Light.h"

class CameraLibre : public Objet
{
        public:
                CameraLibre(bool visible, QVector3D position, QVector3D cibleCamera, QVector3D orientation, bool possedeCollisionBox, QVector3D diagonaleCollisionBox, bool rotation90degBoundBox, float vitesseJoueur = 0, float sensivity = 0, QString fichierMesh = "", QString fichierTexture = "", bool isTextureUVmap = false);

                void Animate (Objet *listeObjet[], int nombreObjets, int tailleTerrainX, int tailleTerrainY); //fonction la plus utilise, elle est appele a  chaques fois que l'on rafraichit l'image (pour deplacer la camera par ex)
                bool test_Collision (QVector3D positionObjet1, QVector3D boxObjet1, QVector3D positionObjet2, QVector3D boxObjet2);

                void mouvementSouris (int new_x, int new_y);
                void deplacement (int numDirection, bool valeur);

                int getAngleRotationArme();
                void setToucheArmeActive(bool statut);
                bool getStatutTir();

                void setVitesse(float speed);
                float getVitesse();
                QVector3D getCibleCamera();
                float _sensivity;
                float _theta;
                float _phi;

        private:
                void ConversionAnglesVersVecteurs();
                void ConversionVecteursVersAngles();
                void DeterminerConstanteSaut(float altitudeInitiale, float altitudeVoulu);

                float _vitesse;
                bool _collision;

                bool Mouvement[6];
                bool joueurAccroupi;

                QVector3D _cibleCamera; //contient les coordonnees du point que le joueur regarde
                QVector3D _forward; //contient les coord du !vecteur! de la direction que le joueur regarde, le calcul entre _targetJoueur et _forward sont effectuee par les fonctions ConversionAnglesVersVecteurs() et ConversionVecteursVersAngles()

                QVector3D _up; //Vecteur vertical du monde, utilisÃ© dans les calculs
                QVector3D _left; //Vecteur qui montre la gauche (orthogonal au sens de deplacement et au vecteur up)


                struct GestionSaut //Toutes les donnees qui influe le saut
                                {float acceleration; float vitesseBase;
                                float altitude ; float altitudeInitial; float altitudeVoulu; int step;
                                float altitudePrecedente; bool enCollision;};
        GestionSaut saut;

};


#endif // CAMERALIBRE_H
