#include "Objets.h"

#define TRIANGLE 0
#define QUADRILAT 1
#define POLYGONE 2

using namespace std;

/*
Voici un petit dessin des origines des reperes:


       ------- //ici _theta = 90°
     -    y    -
    -     |     -
180°-   O--x     - //_theta = 0°
    -           -
     -         -
       ------- //_theta = -90°

Attention avec les coordonnees spheriques (r,theta,phi) on a phi langle a partir de laxe z et theta langle a partir de laxe x

*/


Objet::Objet(bool visible, Coord3D position, Coord3D orientation, bool possedeCollisionBox, Coord3D collisionBox, bool rotation90degCollisionBox, QString nomObjet, QString fichierMesh, QString fichierTexture, bool textureUV, int repeatx, int repeaty)
{
    float tampon=0;
    _isVisible = visible;
    _position = position;
    _orientation = orientation;
    _possedeCollisionBox = possedeCollisionBox;
    _collisionBox = collisionBox;

    ////////////////////////////////////////////////////////
    //Rotation de la bound box avec la rotation de l'objet//
    ////////////////////////////////////////////////////////
    if ((_rotation90degCollisionBox = rotation90degCollisionBox))
    {
        tampon = _collisionBox.X;
        _collisionBox.X = _collisionBox.Y;
        _collisionBox.Y = tampon;
    }
    ////////////////////////////////////////////////////////

    _nomObjet = nomObjet;
    _fichierMesh = fichierMesh;
    _nombreDeFaces = 0;
    _nombreDeVectrices = 0;
    _nombreDeCoordTexture = 0;
    _nombreDeCoordNormales = 0;
    _contientNormalVect = false;
    _isTextureUVMap = textureUV; //sert a  preciser si la texture doit etre affichee comme une texture UV map ou non
    _repeatx = repeatx;
    _repeaty = repeaty;
    _fichierTexture = fichierTexture;
    _chargementMeshOk = false;
    _chargementTextureOk = false;

    loadMesh();
    loadTexture();
}



void Objet::loadMesh()
{
    QFile file(_fichierMesh);

    if (!file.open(QIODevice::ReadOnly)) //si il y a une erreur a l'ouverture
        _chargementMeshOk = false;
    else //si il a reussi a le lire
    {
        _nombreDeFaces = 0;
        _nombreDeVectrices = 0;
        _nombreDeCoordNormales = 0;

        QTextStream fichier(&file);

        //--On lit une premiere fois pour compter le nombre de vectrices et de faces,
        //et connaitre le type de face pour ces dernieres (quads, triangles,...) ;
        while(!fichier.atEnd())
        {
            QString line = fichier.readLine();
            QStringList listeCellules = line.split((' '));
            if (listeCellules[0] == "v")
                _nombreDeVectrices++;
            if (listeCellules[0] == "vt")
                _nombreDeCoordTexture++;
            if (listeCellules[0] == "vn")
            {
                _contientNormalVect = true;
                _nombreDeCoordNormales++;
            }
            if (listeCellules[0] == "f")
            {
                _nombreDeFaces++;
                _nombreVectricesFace.push_back(listeCellules.count()-1); //on ajoute une case au tableau, elle contient le nb de vectrice de la face
            }
         }

        //--On verifie que les donnees recupere sont valides puis on relit le fichier pour en extraire les coordonnÃ©es voulues
        if ((_nombreDeFaces == 0) || (_nombreDeVectrices == 0))
        {
            qDebug() << "----->ERREUR 04 ; Format incorrect du fichier mesh (pas assez de vectrices ou de faces) " << _fichierMesh << " : FAILED";
            _chargementMeshOk = false;
        }
        else if ((_nombreDeCoordTexture == 0) && (_isTextureUVMap==true))
        {
            qDebug() << "----->ERREUR 05 ; Mauvaise description du fichier : pas de coord texture pour l'UV map";
            _chargementMeshOk = false;
        }
        else
        {

            // on donne à notre tableau (qui contient les donnees du mesh) la taille qu'il faut
            // c'est a dire en general le nombre de face comptées, 4 vectrices max, avec 3 coordonnées pour chacune
            _tableauMesh.resize(_nombreDeFaces, vector<vector<float> >(4, vector<float>(3, 0)));
            //de meme sauf que les textures n'ont que deux coord
            _tableauTexture.resize(_nombreDeFaces, vector<vector<float> >(4, vector<float>(2, 0)));
            //et les coordonnees des vecteur normaux à chaque face
            _tableauxNormales.resize(_nombreDeFaces, vector<float>(3, 0));
            ///////////////////////////////////////////////////////////////////////////////
            // On connait maintenant les caracteristiques de l'objet et
            // les trois tableaux qui vont contenir dans l'ordre du dessin les coord
            // sont à la bonnes tailles, on relit maintenant le fichier pour les remplir
            ////////////////////////////////////////////////////////////////////////////////
            fichier.seek(0); //on repart au debut du fichier
            //initiliation des variables;
            int numeroVectrices= 0; //contient le numero de la vectrice
            int numeroCoordTexture = 0;
            int numeroNormales = 0;
            int numeroFaces = 0;
            vector< vector< float > > _listeVectrices(_nombreDeVectrices,vector<float> (3,0)); //on alloue un autre tableau (intermediaire) qui va contenir les coordonnees de chaque vectrice dans l'ordre;
            vector< vector< float > > _listeCoordTexture(_nombreDeCoordTexture,vector<float> (2,0)); //on a autant de coordonnee texture que de vectrice... (on est oblige de declarer le tableau meme si il ne sera pas utilise si pas d'UV map)
            vector< vector< float > > _listeNormales(_nombreDeCoordNormales,vector<float> (3,0)); //va contenir le numéro de la normale pour chaque face

            while(!fichier.atEnd())
            {

                QString line = fichier.readLine(); //on lit la ligne on obtient par exemple la ligne "v 25.00 0.00 15.00" pour
                                                   //les vectrices ou "f 1/1 4/2 3/3 2/4" pour les faces
                QStringList listeMots = line.split((' ')); //on separe les mots et on obtient  "f"   "1/1"   "4/2"  ...
                                                           //dans "1/1" le premier est le numero de la vectrice (dans le fichier obj) et le deuxieme le numero de la coordonnÃ©e texture


                //On commence par toutes les lignes qui commencent par v, les vectrices donc...
                if (listeMots[0] == "v") //on lit tout d'abord chaque vectrice et on les stockent dans l'ordre dans le tableau _listeVectrices
                {
                    _listeVectrices[numeroVectrices][0] = (listeMots[1]).toFloat();
                    _listeVectrices[numeroVectrices][1] = (listeMots[2]).toFloat();
                    _listeVectrices[numeroVectrices][2] = (listeMots[3]).toFloat();
                    numeroVectrices++;
                }

                //Puis par toutes les lignes qui commencent par vt, les coordonnees des points textures donc...
                if (listeMots[0] == "vt")
                {
                    _listeCoordTexture[numeroCoordTexture][0] = (listeMots[1]).toFloat();
                    _listeCoordTexture[numeroCoordTexture][1] = (listeMots[2]).toFloat();
                    //qDebug() << "Num text = " << numeroCoordTexture;
                    numeroCoordTexture++;
                }

                //puis les coord des normales aux faces
                if (listeMots[0] == "vn")
                {
                    _listeNormales[numeroNormales][0] = (listeMots[1]).toFloat();
                    _listeNormales[numeroNormales][1] = (listeMots[2]).toFloat();
                    _listeNormales[numeroNormales][2] = (listeMots[3]).toFloat();
                    numeroNormales++;
                }

                //Puis on continue dans le fichier et on on tombe sur les lignes commencant par f, les faces. On va donc mettre
                // ici les vectrices correspondantes a chaque face dans l'ordre...
                if (listeMots[0] == "f")
                {
                    numeroVectrices = 0;
                    numeroCoordTexture = 0;
                    for (int i=1 ; i <= _nombreVectricesFace[numeroFaces] ; i++)
                    {
                        QStringList listeVariables = listeMots[i].split(('/')); // on coupe le '12/54' en deux, et on va utiliser
                                                                            // le premier nombre, c'est le numero de la vectrice,...
                                                                            // le deuxieme c'est le numero de la texture

                        _tableauMesh[numeroFaces][numeroVectrices][0] = _listeVectrices[(listeVariables[0]).toInt()-1][0]; //-1 car donc un fichier .obj, les vectrices sont numerotees a partir de 1
                        _tableauMesh[numeroFaces][numeroVectrices][1] = _listeVectrices[(listeVariables[0]).toInt()-1][1];
                        _tableauMesh[numeroFaces][numeroVectrices][2] = _listeVectrices[(listeVariables[0]).toInt()-1][2];
                        if (_isTextureUVMap)
                        {
                            _tableauTexture[numeroFaces][numeroCoordTexture][0] = _listeCoordTexture[(listeVariables[1]).toInt()-1][0];
                            _tableauTexture[numeroFaces][numeroCoordTexture][1] = _listeCoordTexture[(listeVariables[1]).toInt()-1][1];
                        }
                        numeroVectrices++;
                        numeroCoordTexture++;
                    }
                    if (_contientNormalVect)
                    {
                        QStringList listeVariables = listeMots[1].split(('/')); //on recupère dans "f 1//2 3//4" le "2"
                        _tableauxNormales[numeroFaces][0] = _listeNormales[(listeVariables[2]).toInt()-1][0];
                        _tableauxNormales[numeroFaces][1] = _listeNormales[(listeVariables[2]).toInt()-1][1];
                        _tableauxNormales[numeroFaces][2] = _listeNormales[(listeVariables[2]).toInt()-1][2];
                    }
                    numeroFaces++;
                    numeroVectrices = 0;
                 }

            }


        qDebug() << "+ Chargement fichier" << _fichierMesh << " : OK";
        _chargementMeshOk = true;
        }//fin test mesh incorrect

    }
}



void Objet::loadTexture()
{
    if(_imageBase.load ( _fichierTexture, "PNG" ))//chargement fichier image
    {
        _chargementTextureOk=true;
        _imageTextureQT = QGLWidget::convertToGLFormat ( _imageBase ); //transformation et renversement de l'image
        qDebug() << "+ Chargement texture objet " << _nomObjet << " : OK";
    }else//si erreur
    {
        qDebug() << "----->ERREUR 02 ; Chargement texture " << _nomObjet << " = FAILED";
        _chargementTextureOk=false;
    }

}



void Objet::afficherObjet()
{
    if ((_chargementMeshOk) && (_chargementTextureOk) && (_isVisible))
    {
        glPushMatrix();
        glTranslated ( _position.X,_position.Y,_position.Z );

        if (_orientation.X != 0)
            glRotated ( _orientation.X,1,0,0 );
        if (_orientation.Y != 0)
            glRotated ( _orientation.Y,0,1,0 );
        if (_orientation.Z != 0)
            glRotated ( _orientation.Z,0,0,1 );

        glBindTexture ( GL_TEXTURE_2D, _textureFinaleObjet );
        glBegin ( GL_TRIANGLES );

        for (int f=0 ; f < _nombreDeFaces ; f++)
        {
                if (_nombreVectricesFace[f]==3) //si c'est des triangles
                {
                    glNormal3f(_tableauxNormales[f][0],_tableauxNormales[f][1],_tableauxNormales[f][2]);
                    if (_isTextureUVMap == true)
                    {
                        glTexCoord2f ( _tableauTexture[f][0][0],_tableauTexture[f][0][1] );   glVertex3f ( (_tableauMesh)[f][0][0],(_tableauMesh)[f][0][1],(_tableauMesh)[f][0][2]);
                        glTexCoord2f ( _tableauTexture[f][1][0],_tableauTexture[f][1][1] );   glVertex3f ( (_tableauMesh)[f][1][0],(_tableauMesh)[f][1][1],(_tableauMesh)[f][1][2]);
                        glTexCoord2f ( _tableauTexture[f][2][0],_tableauTexture[f][2][1] );   glVertex3f ( (_tableauMesh)[f][2][0],(_tableauMesh)[f][2][1],(_tableauMesh)[f][2][2]);
                    }
                    else //si cest des triangles sans uv map
                    {
                        glTexCoord2d ( 0       , 0       );  glVertex3f ( (_tableauMesh)[f][0][0],(_tableauMesh)[f][0][1],(_tableauMesh)[f][0][2]);
                        glTexCoord2d ( _repeatx, 0       );  glVertex3f ( (_tableauMesh)[f][1][0],(_tableauMesh)[f][1][1],(_tableauMesh)[f][1][2]);
                        glTexCoord2d ( _repeatx, _repeaty);  glVertex3f ( (_tableauMesh)[f][2][0],(_tableauMesh)[f][2][1],(_tableauMesh)[f][2][2]);
                    }

                }
                else if (_nombreVectricesFace[f]==4) //si il y a 4 vectrices dans la face, on  trace le rectangle avec deux triangles, pour optimiser le dessin et limiter les appel à glBegin()
                {
                    if (_isTextureUVMap)
                    {
                        glNormal3f(_tableauxNormales[f][0],_tableauxNormales[f][1],_tableauxNormales[f][2]);
                        glTexCoord2f ( _tableauTexture[f][0][0],_tableauTexture[f][0][1] );   glVertex3f ( (_tableauMesh)[f][0][0],(_tableauMesh)[f][0][1],(_tableauMesh)[f][0][2]);
                        glTexCoord2f ( _tableauTexture[f][1][0],_tableauTexture[f][1][1] );   glVertex3f ( (_tableauMesh)[f][1][0],(_tableauMesh)[f][1][1],(_tableauMesh)[f][1][2]);
                        glTexCoord2f ( _tableauTexture[f][2][0],_tableauTexture[f][2][1] );   glVertex3f ( (_tableauMesh)[f][2][0],(_tableauMesh)[f][2][1],(_tableauMesh)[f][2][2]);

                        glNormal3f(_tableauxNormales[f][0],_tableauxNormales[f][1],_tableauxNormales[f][2]);
                        glTexCoord2f ( _tableauTexture[f][0][0],_tableauTexture[f][0][1] );   glVertex3f ( (_tableauMesh)[f][0][0],(_tableauMesh)[f][0][1],(_tableauMesh)[f][0][2]);
                        glTexCoord2f ( _tableauTexture[f][2][0],_tableauTexture[f][2][1] );   glVertex3f ( (_tableauMesh)[f][2][0],(_tableauMesh)[f][2][1],(_tableauMesh)[f][2][2]);
                        glTexCoord2f ( _tableauTexture[f][3][0],_tableauTexture[f][3][1] );   glVertex3f ( (_tableauMesh)[f][3][0],(_tableauMesh)[f][3][1],(_tableauMesh)[f][3][2]);

                    }
                    else //si c'est rectangle sans UV map
                    {
                        glNormal3f(_tableauxNormales[f][0],_tableauxNormales[f][1],_tableauxNormales[f][2]);
                        glTexCoord2d ( 0,0              );  glVertex3f ( (_tableauMesh)[f][0][0],(_tableauMesh)[f][0][1],(_tableauMesh)[f][0][2]);
                        glTexCoord2d ( _repeatx,0       );  glVertex3f ( (_tableauMesh)[f][1][0],(_tableauMesh)[f][1][1],(_tableauMesh)[f][1][2]);
                        glTexCoord2d ( _repeatx,_repeaty);  glVertex3f ( (_tableauMesh)[f][2][0],(_tableauMesh)[f][2][1],(_tableauMesh)[f][2][2]);

                        glNormal3f(_tableauxNormales[f][0],_tableauxNormales[f][1],_tableauxNormales[f][2]);
                        glTexCoord2d ( 0,0              );  glVertex3f ( (_tableauMesh)[f][0][0],(_tableauMesh)[f][0][1],(_tableauMesh)[f][0][2]);
                        glTexCoord2d ( _repeatx,_repeaty);  glVertex3f ( (_tableauMesh)[f][2][0],(_tableauMesh)[f][2][1],(_tableauMesh)[f][2][2]);
                        glTexCoord2d ( 0,_repeaty       );  glVertex3f ( (_tableauMesh)[f][3][0],(_tableauMesh)[f][3][1],(_tableauMesh)[f][3][2]);

                    }

                }
        }
        glEnd();
        glPopMatrix();

    }
    else if (!(_chargementMeshOk) || !(_chargementTextureOk))
    {
        qDebug() << "Erreur du mesh ou de texture, l'objet " << _nomObjet << " ne sera pas affichee";
    }
}


void Objet::dessinerBoundBox()
{
    if (_possedeCollisionBox)
    {
        glDisable(GL_TEXTURE_2D);

        glLineWidth(6);
        glPushMatrix();

        glTranslated ( _position.X,_position.Y,_position.Z );

        glBegin(GL_LINES);

        glColor3ub(0,0,255); //axe X en Bleu

        glVertex3f(0,0,0);
        glVertex3f(_collisionBox.X,  0 , 0);
        glVertex3f(_collisionBox.X , 0 , 0);
        glVertex3f(_collisionBox.X , 0 , _collisionBox.Z);
        glVertex3f(_collisionBox.X , 0 , _collisionBox.Z);
        glVertex3f(0 , 0 , _collisionBox.Z);
        glVertex3f(0 , 0 , _collisionBox.Z);
        glVertex3f(0 , 0 , 0);

        glVertex3f(_collisionBox.X,0,0);
        glVertex3f(_collisionBox.X,_collisionBox.Y,0);
        glVertex3f(0 , 0 , 0);
        glVertex3f(0 , _collisionBox.Y , 0);
        glVertex3f(0 , 0 , _collisionBox.Z);
        glVertex3f(0 , _collisionBox.Y , _collisionBox.Z);
        glVertex3f(_collisionBox.X , 0 , _collisionBox.Z);
        glVertex3f(_collisionBox.X , _collisionBox.Y , _collisionBox.Z);

        glVertex3f(0,_collisionBox.Y,_collisionBox.Z);
        glVertex3f(_collisionBox.X,_collisionBox.Y,_collisionBox.Z);
        glVertex3f(_collisionBox.X,_collisionBox.Y,_collisionBox.Z);
        glVertex3f(_collisionBox.X,_collisionBox.Y,0);
        glVertex3f(_collisionBox.X,_collisionBox.Y,0);
        glVertex3f(0 ,_collisionBox.Y,0);
        glVertex3f(0 ,_collisionBox.Y,0);
        glVertex3f(0 ,_collisionBox.Y,_collisionBox.Z);


        glColor3ub(255,255,255);
        glEnd();
        glPopMatrix();

        glEnable(GL_TEXTURE_2D);
    }
}



void Objet::genererTextureOpenGL(bool useMipMap)
{
    if (_chargementTextureOk)
    {
    GLuint finalTexture;

    glGenTextures ( 1, &finalTexture ); //generation de la texture openGL, a ce niveau ont pourrait renvoyer finalTexture

    glBindTexture ( GL_TEXTURE_2D, finalTexture );

    if ( useMipMap )
    {
        gluBuild2DMipmaps ( GL_TEXTURE_2D, 3, _imageTextureQT.width(), _imageTextureQT.height(), GL_RGBA, GL_UNSIGNED_BYTE,
                                _imageTextureQT.bits() );//creation des 3 mipmaps (adapte a chaque distance)
        glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
                              GL_LINEAR_MIPMAP_LINEAR ); //ajout du filtre trilineaire pour le "tres beau rendu"
    }
    else
    {
        glTexImage2D ( GL_TEXTURE_2D, 0, 4, _imageTextureQT.width(), _imageTextureQT.height(), 0, GL_RGBA,GL_UNSIGNED_BYTE,
                                _imageTextureQT.bits() );
        glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ); //ajout de seulement un filtre lineaire
    }

    glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR ); //filtre lineaire pour longue distance
    _textureFinaleObjet = finalTexture;
    }

}

void Objet::setPosition(Coord3D position)
{
    _position = position;
}

Coord3D Objet::getPosition()
{
    return _position;
}

Coord3D Objet::getCollisionBox()
{
    return _collisionBox;
}

bool Objet::getPossedeCollisionBox()
{
        return _possedeCollisionBox;
}
