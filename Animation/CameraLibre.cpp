#include "CameraLibre.h"

#define AVANCER 0
#define RECULER 1
#define GAUCHE 2
#define DROITE 3
#define SAUT 4
#define ACCROUPI 5

#define ROTA_SUPP 0.1
#define ROTA_SUPP_MAX 8

CameraLibre::CameraLibre(bool isVisible, QVector3D position, QVector3D cibleCamera, QVector3D orientation, bool possedeCollisionBox, QVector3D diagonaleCollisionBox, bool rotation90degCollisionBox, float vitesseJoueur, float sensivity, QString fichierMesh, QString fichierTexture, bool isTextureUVmap)
                         : Objet(isVisible, position, orientation, possedeCollisionBox, diagonaleCollisionBox, rotation90degCollisionBox, "Personnage", fichierMesh, fichierTexture, isTextureUVmap)
{
        _cibleCamera = cibleCamera;

        Mouvement[AVANCER] = false;
        Mouvement[RECULER] = false;
        Mouvement[GAUCHE] = false;
        Mouvement[DROITE] = false;
        Mouvement[SAUT] = false;
        Mouvement[ACCROUPI] = false;

        joueurAccroupi = false;
        saut.step = 0;//valeur de temps dans les equations horaires du mouvement, augmente de 25 a chaques etape pour finir a 1000, la fin du saut
        saut.enCollision = false;

        _vitesse = vitesseJoueur; //pour la vitesse de deplacement
        _sensivity = sensivity;

        _phi = 0;
        _theta = 0;
        ConversionVecteursVersAngles(); //calcul des angles phi et theta qui correspondent au _position et _targetCameraJoueur de dÃ©but de jeu

        _up=QVector3D(0,0,1);

}



void CameraLibre::Animate(Objet *listeObjet[], int nombreObjets, int tailleTerrainX, int tailleTerrainY)
{
        int i=0; //pour les tests de collision
        if (_position.x() < 0) //pour ne pas sortir
                _position.setX(0);
        else	if (_position.x() > tailleTerrainX*4)
                _position.setX(tailleTerrainX*4);

        if (_position.y() < 0)
                _position.setY(0);
        else if (_position.y() > tailleTerrainY*4)
                _position.setY(tailleTerrainY*4);


        if (Mouvement[AVANCER])
        {
            _position += QVector3D(_forward.x()* _vitesse,0,0);
            //debut test collision
            i=0;
            _collision = false;
            while (i<nombreObjets && !_collision)
            {
                if (listeObjet[i]->getPossedeCollisionBox()) //on verifie qu'il a une box
                    _collision = test_Collision(_position, _collisionBox, listeObjet[i]->getPosition(), listeObjet[i]->getCollisionBox());
                i++;
            }//Fin test collision
            if (_collision)
                _position -= QVector3D(_forward.x()* _vitesse,0,0);

            _position += QVector3D(0,_forward.y() * _vitesse,0);
            //debut test collision
            i=0;
            _collision = false;
            while (i<nombreObjets && !_collision)
            {
                if (listeObjet[i]->getPossedeCollisionBox())
                    _collision = test_Collision(_position, _collisionBox, listeObjet[i]->getPosition(), listeObjet[i]->getCollisionBox());
                i++;
            }//Fin test collision

            if (_collision)
                _position -= QVector3D(0,_forward.y() * _vitesse,0);

        }
        if (Mouvement[RECULER])
        {
            _position -= QVector3D(_forward.x() * _vitesse,0,0);
            _position -= QVector3D(0,_forward.y() * _vitesse,0);
        }
        if (Mouvement[GAUCHE])
        {
                _position += _left * _vitesse;
                //debut test collision
                i=0;
                _collision = false;
                while (i<nombreObjets && !_collision)
                {
                    if (listeObjet[i]->getPossedeCollisionBox()) //on verifie qu'il a une box
                        _collision = test_Collision(_position, _collisionBox, listeObjet[i]->getPosition(), listeObjet[i]->getCollisionBox());
                    i++;
                }//Fin test collision

                if (_collision)
                    _position -= _left * _vitesse;
        }
        if (Mouvement[DROITE])
        {
                _position -= _left * _vitesse;
                //debut test collision
                i=0;
                _collision = false;
                while (i<nombreObjets && !_collision)
                {
                    if (listeObjet[i]->getPossedeCollisionBox() == true) //on verifie qu'il a une box
                        _collision = test_Collision(_position, _collisionBox, listeObjet[i]->getPosition(), listeObjet[i]->getCollisionBox());
                    i++;
                }//Fin test collision
                if (_collision)
                    _position += _left * _vitesse;
        }


        if (Mouvement[ACCROUPI])
                _position -= QVector3D(0,0,1);

        if (Mouvement[SAUT])
            _position += QVector3D(0,0,1);
        ////////////////////////////GESTION DU SAUT////////////////////////////
/*
        if ((Mouvement[SAUT]) || (saut.enCollision))
        {
            if ((Mouvement[SAUT]) && (saut.enCollision))
            {
                saut.step = 0;
                saut.enCollision = false;
            }

            if (saut.step == 0)
            {
                saut.altitudeInitial = _position.Z;
                saut.altitudeVoulu = _position.Z+7 ;
                DeterminerConstanteSaut(saut.altitudeInitial, saut.altitudeVoulu);
                saut.altitude = saut.altitudeInitial;
            }
            saut.altitudePrecedente = _position.Z;
            saut.altitude = 0.5*saut.acceleration*(saut.step*saut.step) + saut.vitesseBase*saut.step+saut.altitudeInitial;
            _position.Z = saut.altitude;

            saut.step += 25;

            //debut test collision
            i=0;
            _collision = false;
            while (i<nombreObjets && !_collision)
            {
                if (listeObjet[i]->getPossedeCollisionBox()) //on verifie qu'il a une box
                    _collision = test_Collision(_position, _collisionBox, listeObjet[i]->getPosition(), listeObjet[i]->getCollisionBox());
                i++;
            }
            //Fin test collision

            if ((_collision)&&(saut.step>500)) //quand on a saute sur un objet
            {
                //qDebug() << "step = " << saut.step << " | Altitude  = " << saut.altitude ;
                _position.Z = saut.altitudePrecedente;
                saut.enCollision = true;
                Mouvement[SAUT] = false;
                saut.step -= 25;

            }
            //et maintenant quand on tombe sans sauter de l'objet, on parametre un nouveau saut, mais juste la partie de la descente
            else if ((!_collision)&&(saut.enCollision)&&(_position.Z>0))
            {
                saut.enCollision=false;
                Mouvement[SAUT] = true;
                saut.altitudeInitial = 0;
                saut.altitudeVoulu = _position.Z;
                DeterminerConstanteSaut(saut.altitudeInitial, saut.altitudeVoulu); //calcul vitesseInitiale et acceleration initiale
                saut.altitude = saut.altitudeInitial;
                saut.step=500; //la moitié du saut
            }
            else if (_position.Z<0) //et quand on touche le sol
            {
                _position.Z = 0;
                Mouvement[SAUT] = false;
                saut.enCollision = false;
                saut.step = 0;
            }
        }
*/
        ////////////////////////////////////////////////////////////////////////

        ConversionAnglesVersVecteurs(); //recalcule les coordonnees du vecteur de direction du regard a partir des angles _phi et _theta
        //comme on a bouge, on recalcule la cible fixee par la camera
        // en ajustant pour car la camera n'est pas exactement a la position du jour
        // mais decalé de (1,1,6)
        _cibleCamera = QVector3D(_forward.x() + (_position.x() +1),_forward.y() + (_position.y() +1),_forward.z() + (_position.z() +6));
        _left = QVector3D::crossProduct(_up,_forward); //recalcule le vecteur perdendiculaire au vecteur up et target pour se deplacer vers la gauche ou la droite
        _left.normalize();

}

bool CameraLibre::test_Collision(QVector3D positionObjet1, QVector3D boxObjet1, QVector3D positionObjet2, QVector3D boxObjet2)
{
    if((positionObjet2.x() >= positionObjet1.x() + boxObjet1.x())   // trop à droite
    || (positionObjet2.x() + boxObjet2.x() <= positionObjet1.x())   // trop à gauche
    || (positionObjet2.y() >= positionObjet1.y() + boxObjet1.y())   // trop en bas
    || (positionObjet2.y() + boxObjet2.y() <= positionObjet1.y())   // trop en haut
    || (positionObjet2.z() >= positionObjet1.z() + boxObjet1.z())   // trop derrière
    || (positionObjet2.z() + boxObjet2.z() <= positionObjet1.z()))  // trop devant
        return 0;
    else //si il y a collision
        return 1;
}

void CameraLibre::ConversionAnglesVersVecteurs() //tranforme les coordonnees_phi et _theta en vecteur qui donne la direction de la camera
{
        float r_temp = sin(_phi*M_PI/180);
        _forward = QVector3D(r_temp*cos(_theta*M_PI/180),r_temp*sin(_theta*M_PI/180),cos(_phi*M_PI/180));
}

void CameraLibre::ConversionVecteursVersAngles() //tranforme les coordonnees X,Y,Z en _phi et _theta
{
        //Calcul des angles à  partir des coordonnees X,Y,Z :
        //Une coordonnee spherique est de forme (r, _phi, _theta)
        //Attention avec les coordonnees spherique (r,theta,phi) on a phi langle a partir de laxe z
        //et theta langle a partir de laxe x
        //r = racine( x² + y² + z² )
        //_phi = arcos ( Z / r)
        //_theta = arcos ( X / racine(X²+Y²) )
        //-La librairie <cmath> utilise les radians, il faut donc convertir a chaque fois les degres

    _forward = QVector3D(_cibleCamera.x() - (_position.x()+1),_cibleCamera.y() - (_position.y()+1),_cibleCamera.z() - (_position.z()+5));
    float r = sqrt(pow(_forward.x(),2) + pow(_forward.y(),2) + pow(_forward.z(),2));
    _phi = ( acos(_forward.z()/r)  *180/M_PI);

    float r_temp = sqrt(pow(_forward.x(),2) + pow(_forward.y(),2));
    if (_forward.y() >= 0)
            _theta = ((acos(_forward.x()/r_temp)) *180/M_PI);
    else
            _theta = -((acos(_forward.x()/r_temp))*180/M_PI);


}

void CameraLibre::DeterminerConstanteSaut(float altitudeInitiale, float altitudeVoulu) //calcul l'acceleration et la vitesse (initiale) pour le saut
{
/*On a  z(t) = 0.5*acceleration² + saut.vitesseBase*t + z(0)
On cherche l'acceleration et la vitessebase pour que z(t = 0) = z(0)
                                                     z(t = 1000) = z(0)
                                                     z(t = 500) = altitudeVoulu

On obtient le systeme
---
| 500 000*acceleration + 1000*vitesseBase + altitudeInitiale = altitudeInitiale
| 125 000*acceleration + 500*vitesseBase + altitudeInitiale = altitudeVoulu
---
et donc;
acceleration = (-2*altitudeVoulu + 2*altitudeInitiale) / 250 000
vitesseBase = (4*altitudeVoulu - 4*altitudeInitiale) /1000


*/
    saut.acceleration = (-2*altitudeVoulu + 2*altitudeInitiale) / 250000;
    saut.vitesseBase = (4 * altitudeVoulu - 4*altitudeInitiale) / 1000;
}

void CameraLibre::mouvementSouris (int xrel, int yrel)
{
        _theta += xrel*_sensivity; //on modifie les coordonnees polaire quand on bouge la souris
        _phi -= yrel*_sensivity; //on met un '-' car quand on regarde vers le bas _phi augmente

        if (_phi < 10) //On limite les valeurs de _phi, pour prévenir le torticoli :D
            _phi = 10;
        else if (_phi > 170)
            _phi = 170;


        if (_theta > 180) //Evite d'avoir un angle de rotation lateral (theta donc) qui atteint 2700°
            _theta = -180;
        else if (_theta < -180)
            _theta = 180;

        _orientation.setZ(_theta);
        ConversionAnglesVersVecteurs(); //et on convertit notre variation d'angle de vue en un vecteur (interpretable par la camera)
}


void CameraLibre::deplacement (int numDirection, bool valeur)
{
        if (numDirection <= 5)
            Mouvement[numDirection] = valeur;
        else
            qDebug() << "Erreur 03 ; Mouvement du personnage non-autorisé";
}

void CameraLibre::setVitesse (float vitesse)
{
    _vitesse = vitesse;
}

float CameraLibre::getVitesse()
{
    return _vitesse;
}

QVector3D CameraLibre::getCibleCamera()
{
    return _cibleCamera;
}

