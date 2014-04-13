#include "OpenGLWidget.h"

using namespace std;

OpenGLWidget::OpenGLWidget ( QWidget *parent, int largeur, int hauteur, CameraLibre *joueur, Coord3D positionCamera, Coord3D targetCamera,  int tailleSolX, int tailleSolY, Objet *listeObjets[], int nombreObjets, QString nomDeClasse) : QGLWidget ( parent ) //le ": QGLWidget (parent) sert a appeler le constructeur de parent(obligatoire)
{
    _nomDeClasse = nomDeClasse;
    p_joueur = joueur;

    _tailleSolX = tailleSolX;
    _tailleSolY = tailleSolY;

    _nombreObjets = nombreObjets;
    for (int i=0 ; i < _nombreObjets ; i++)
        p_listeObjets[i] = listeObjets[i];

    _positionCamera = positionCamera;
    _cibleCamera = targetCamera;

    setFixedSize ( largeur, hauteur );
    setFormat ( QGLFormat ( QGL::DoubleBuffer | QGL::DepthBuffer ) );
    loadSkybox();

    a=0;
    qDebug() << "+ Creation du GLwidget : " << _nomDeClasse << " : OK";

}

OpenGLWidget::OpenGLWidget ( QWidget *parent, CameraLibre *joueur, Coord3D positionCamera, Coord3D targetCamera,  int tailleSolX, int tailleSolY, Objet *listeObjets[], int nombreObjets, QString nomDeClasse) : QGLWidget ( parent ) //le ": QGLWidget (parent) sert a appeler le constructeur de parent(obligatoire)
{
    _nomDeClasse = nomDeClasse;
    p_joueur = joueur;

    _tailleSolX = tailleSolX;
    _tailleSolY = tailleSolY;

    _nombreObjets = nombreObjets;
    for (int i=0 ; i < _nombreObjets ; i++)
        p_listeObjets[i] = listeObjets[i];

    _positionCamera = positionCamera;
    _cibleCamera = targetCamera;

    setFormat ( QGLFormat ( QGL::DoubleBuffer | QGL::DepthBuffer ) );

    a=0;

    qDebug() << "+ Creation du GLwidget : " << _nomDeClasse << " : OK";
    loadSkybox();

}

void OpenGLWidget::initializeGL()
{

        qglClearColor ( Qt::black );
        float LightAmbient[]= { 0.75f, 0.75f, 0.75f, 1.0f };
        float LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
        float LightPosition[]= { 40.0f, 25.0f, 10.0f, 1.0f };
        float difLight0[4] = {0.75f, 0.75f, 0.75f, 1.0f};
        float specLight0[4] = {0.75f, 0.75f, 0.75f, 1.0f};

        /////////////////////Eclairage/////////////////////////////////////
        float colorWhite[4]  = {1.0f,1.0f,1.0f,1.0f};
        float ambientColor[4] = {0.35f,0.35f,0.35f,1.0f};
        //float colorEmission[4]  = {0.0f,0.0f,0.0f,1.0f};
        glEnable(GL_DEPTH_TEST);	// Active le test de profondeur
        glEnable(GL_LIGHTING);          // Active l'éclairage
        glEnable(GL_LIGHT0);            // Active light0
            glLightfv(GL_LIGHT0,GL_AMBIENT,ambientColor);
            glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION ,0.1f);
        glEnable(GL_LIGHT1); //le spot qui eclaire le sol sous la lampe de plafond
            glLightfv( GL_LIGHT1, GL_DIFFUSE, colorWhite );
            glLightfv( GL_LIGHT1, GL_SPECULAR, colorWhite );
            glLightf( GL_LIGHT1, GL_SPOT_CUTOFF, 90.0f ); //angle du spot
            glLightf( GL_LIGHT1, GL_SPOT_EXPONENT, 3.0f); //attenuation, pour flouter les bord de la tache de lumière
        glEnable(GL_LIGHT2); //le spot qui eclaire l'interieur de la lampe
            glLightfv( GL_LIGHT2, GL_DIFFUSE, colorWhite );
            glLightfv( GL_LIGHT2, GL_SPECULAR, colorWhite );
            glLightf( GL_LIGHT2, GL_SPOT_CUTOFF, 90.0f );
            glLightf( GL_LIGHT2, GL_SPOT_EXPONENT, 3.0f);
        glEnable(GL_LIGHT3);            // Active light3, qui va suivre le projectile lors du tir
            glLightfv( GL_LIGHT3, GL_DIFFUSE, colorWhite );
            glLightfv( GL_LIGHT3, GL_SPECULAR, colorWhite );
            glLightf(GL_LIGHT3,GL_LINEAR_ATTENUATION ,0.1f);
        glDisable(GL_LIGHT3);

        glEnable(GL_LIGHT4);            // Active light3, qui va suivre le projectile lors du tir
            glLightfv( GL_LIGHT4, GL_AMBIENT, LightAmbient );
            glLightfv( GL_LIGHT4, GL_DIFFUSE, difLight0 );
            glLightfv(GL_LIGHT4, GL_SPECULAR, specLight0);
        //glDisable(GL_LIGHT4);
        /*config reflection lumière sur les matériaux*/
        int MatSpec [4] = {1,1,1,1};
        float ambient[4] = {0.55f,0.55f,0.55f,1.0f};

        glMaterialiv(GL_FRONT_AND_BACK,GL_SPECULAR,MatSpec);
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,ambient);
        glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,100);
        /*fin config lum*/
        ///////////////////////////////////////////////////////////////////

        /////////////////////Texture des divers objets du joueur//////////////
        textureSol = loadTexture ( QString ( "pics/floor.png" ), true);
        textureJoueur = loadTexture ( QString ( "pics/whisp.png" ), true);
        textureVueInactive = loadTexture ( QString ( "pics/vueInactive.png" ), true);
        /////////////////////////////////////////////////////////////////////

        for (int i=0 ; i < _nombreObjets ; i++)
            p_listeObjets[i]->genererTextureOpenGL(true);

        p_joueur->genererTextureOpenGL(true);
        loadSkybox();

        qDebug() << "+ Initialisation OpenGL de" << _nomDeClasse << ": OK";;
}

void OpenGLWidget::loadSkybox()
{
    cube_map_texture_ID[0]=loadTexture("sky/front",false);
    cube_map_texture_ID[1]=loadTexture("sky/back",false);
    cube_map_texture_ID[2]=loadTexture("sky/left",false);
    cube_map_texture_ID[3]=loadTexture("sky/right",false);
    cube_map_texture_ID[4]=loadTexture("sky/up",false);
    cube_map_texture_ID[5]=loadTexture("sky/down",false);

}

void OpenGLWidget::drawSkybox()
{
    // R�glage de l'orientation
    glPushMatrix();

    glBindTexture(GL_TEXTURE_2D,cube_map_texture_ID[3]);
    glBegin(GL_QUADS);
        glTexCoord2d(0,0); glVertex3d(0,0,-50);
        glTexCoord2d(0,1); glVertex3d(0,0,72);
        glTexCoord2d(1,1); glVertex3d(0,162,72);
        glTexCoord2d(1,0); glVertex3d(0,162,-50);
    glEnd();
glRotated(90,0,0,1);

glBindTexture(GL_TEXTURE_2D,cube_map_texture_ID[0]);
    glBegin(GL_QUADS);
        glTexCoord2d(0,0); glVertex3d(0,-162,-50);
        glTexCoord2d(0,1); glVertex3d(0,-162,72);
        glTexCoord2d(1,1); glVertex3d(0,0,72);
        glTexCoord2d(1,0); glVertex3d(0,0,-50);
    glEnd();
glRotated(-90,0,0,1);

glBindTexture(GL_TEXTURE_2D,cube_map_texture_ID[2]);
    glBegin(GL_QUADS);
        glTexCoord2d(0,0); glVertex3d(162,162,-50);
        glTexCoord2d(0,1); glVertex3d(162,162,72);
        glTexCoord2d(1,1); glVertex3d(162,0,72);
        glTexCoord2d(1,0); glVertex3d(162,0,-50);
    glEnd();
glRotated(-90,0,0,1);

glBindTexture(GL_TEXTURE_2D,cube_map_texture_ID[1]);
    glBegin(GL_QUADS);
        glTexCoord2d(0,0); glVertex3d(-162,0,-50);
        glTexCoord2d(0,1); glVertex3d(-162,0,72);
        glTexCoord2d(1,1); glVertex3d(-162,162,72);
        glTexCoord2d(1,0); glVertex3d(-162,162,-50);
    glEnd();
glRotated(90,0,0,1);

glPopMatrix();

glBindTexture(GL_TEXTURE_2D,cube_map_texture_ID[5]);
glBegin(GL_QUADS);

    glTexCoord2d(1,0); glVertex3d(0,0,-0.01);
    glTexCoord2d(1,1); glVertex3d(162,0,-0.01);
    glTexCoord2d(0,1); glVertex3d(162,162,-0.01);
    glTexCoord2d(0,0);  glVertex3d(0,162,-0.01);
glEnd();


glBindTexture(GL_TEXTURE_2D,cube_map_texture_ID[4]);
glBegin(GL_QUADS);

    glTexCoord2d(0,0); glVertex3d(0,0,72);
    glTexCoord2d(0,1); glVertex3d(162,0,72);
    glTexCoord2d(1,1); glVertex3d(162,162,72);
    glTexCoord2d(1,0);  glVertex3d(0,162,72);
glEnd();
glPopMatrix();
}

GLuint OpenGLWidget::loadTexture ( QString filename, bool useMipMap)
{
        QImage baseTexture, interTexture;
        GLuint finalTexture;

        if (!baseTexture.load ( filename, "PNG" ))
            qDebug() << "----->ERREUR 02 ; Chargement texture = FAILED";

        interTexture = QGLWidget::convertToGLFormat ( baseTexture ); //transformation et renversement de l'image
        glGenTextures ( 1, &finalTexture ); //generation de la texture openGL, Ã  ce niveau ont pourrait renvoyer finalTexture

        glBindTexture ( GL_TEXTURE_2D, finalTexture );


        if ( useMipMap )
        {
                gluBuild2DMipmaps ( GL_TEXTURE_2D, 3, interTexture.width(), interTexture.height(), GL_RGBA, GL_UNSIGNED_BYTE,
                                    interTexture.bits() );//creation des 3 mipmaps (adapte a  chaque distance)

                glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR ); //ajout du filtre trilineaire pour le "tres beau rendu"
                glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ); //filtre lineaire pour longue distance
        }
        else
        {
                glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ); //ajout seulement d'un filtre lineaire
                glTexImage2D ( GL_TEXTURE_2D, 0, 3, interTexture.width(), interTexture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, interTexture.bits() );
        }

        return finalTexture; //on renvoie la texture

}

/////////////////////////////////////////////////////////////////////////////////////



/*REPERE*/
void OpenGLWidget::dessinerRepere()
{
    glDisable ( GL_TEXTURE_2D );
    glEnable(GL_COLOR_MATERIAL);
    glLineWidth(8);
    glPushMatrix();

    glBegin(GL_LINES);

    glColor3ub(0,0,255); //axe X en Bleu
    glVertex3d(0,0,0);
    glVertex3d(2,0,0);
    glVertex3f(1.5 , 0 , 0.5); //les 2 trait du X
    glVertex3f(2 , 0 , 1.3);
    glVertex3f(1.5 , 0 , 1.3);
    glVertex3f(2 , 0 , 0.5);


    glColor3ub(0,255,0); //axe Y en Vert
    glVertex3d(0,0,0);
    glVertex3d(0,2,0);
    glVertex3f(0 , 1.5 , 1); //les 2 trait du Y
    glVertex3f(0 , 1.2 , 2);
    glVertex3f(0 , 1.6 , 2);
    glVertex3f(0 , 1.4 , 1.5);


    glColor3ub(255,0,0); //axe Z en Rouge;
    glVertex3d(0,0,0);
    glVertex3d(0,0,2);
    glVertex3f(0.2 , 0 , 2); //les 3 trait du Z
    glVertex3f(0.7 , 0 , 2);
    glVertex3f(0.7 , 0 , 2);
    glVertex3f(0.2 , 0 , 1.5);
    glVertex3f(0.2 , 0 , 1.5);
    glVertex3f(0.7 , 0 , 1.5);

    glColor3ub(255,255,255);
    glEnd();
    glPopMatrix();

    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);
}



void OpenGLWidget::ConversionVecteursVersAngles() //transforme les coordonnÃ©es X,Y,Z en _phi et _theta
{
        //Calcul des angles à  partir des coordonnees X,Y,Z :
        //Une coordonnee spherique est de forme (r, _phi, _theta)
        //Pour les coordonnees spherique (r,theta,phi) on a phi langle à partir de laxe vertical z
        //et theta langle à partir de laxe x
        //r = racine( x² + y² + z² )
        //_phi = arcos ( Z / r)
        //_theta = arcos ( X / racine(X²+Y²) )
        //-La librairie <cmath> utilise les radians! C'est pour cela qu'il faut convertir à chaque fois les degrÃ©s...

        Coord3D _forward(_targetJoueur.X - (_positionJoueur.X+1),_targetJoueur.Y - (_positionJoueur.Y+1),_targetJoueur.Z - (_positionJoueur.Z+6));

        float r = sqrt(pow(_forward.X,2) + pow(_forward.Y,2) + pow(_forward.Z,2));
        _phi = ( acos(_forward.Z/r)  *180/M_PI);

        float r_temp = sqrt(pow(_forward.X,2) + pow(_forward.Y,2));
        if (_forward.Y >= 0)
                _theta = ( (acos(_forward.X/r_temp)) *180/M_PI);
        else
                _theta = - (  (acos(_forward.X/r_temp))   *180/M_PI);


}


void OpenGLWidget::resizeGL ( int width, int height )
{

    glViewport ( 0, 0, width, height );
    glMatrixMode ( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective ( 70, ( float ) 640/480,0.5,200 );
    qDebug() << "+ Resize OpenGL de" << _nomDeClasse << ": OK";

}

/////////////////////////////////////////////////////////////////////////////////////

void OpenGLWidget::paintGL()
{


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    ////////////////////////////////////
    /////Placement de la camera//////
    ////////////////////////////////////
    _positionJoueur = (*p_joueur).getPosition();
    _targetJoueur = (*p_joueur).getCibleCamera();

    _positionCamera.X = _positionJoueur.X +1;
    _positionCamera.Y = _positionJoueur.Y +1;
    _positionCamera.Z = _positionJoueur.Z +6;

    _cibleCamera = _targetJoueur;
    gluLookAt(_positionCamera.X,_positionCamera.Y,_positionCamera.Z,_cibleCamera.X,_cibleCamera.Y,_cibleCamera.Z,0,0,1);


    ////////////////////////////////////
    /////Gestion des lumières///////////
    ////////////////////////////////////
    int light0Pos[4] = {0, 10, 1, 1};
    float spot1Pos[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    float spot1Dir[3] = {0.0f, 0.0f, -1.0f};
    float spot2Pos[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    float spot2Dir[3] = {0.0f, 0.0f, 10.0f};

    glDisable ( GL_TEXTURE_2D );
    glEnable(GL_COLOR_MATERIAL);
    glColor3ub(255,255,255);
    glPushMatrix();
        //La petite sphere qui représente la lumière
        glTranslated(24,26,3);
        glPushMatrix();
            glRotated(a,0,0,1);
            glPushMatrix();
                glTranslated(0,10.2,1); //10.2 etant maintenant la distance entre la grosse sphere et la petite
                GLUquadric* params = gluNewQuadric();
                    gluQuadricDrawStyle(params,GLU_POINT);
                    gluSphere(params,0.1,50,50);
                    gluDeleteQuadric(params);
            glPopMatrix();

            //la lumière accroché à la petite sphere
            glLightiv(GL_LIGHT0,GL_POSITION,light0Pos);

        glPopMatrix();
        //et la sphere au centre
        GLUquadric* params2 = gluNewQuadric();
            gluQuadricDrawStyle(params2,GLU_FILL);
            gluSphere(params2,1,50,50);
            gluDeleteQuadric(params2);
    glPopMatrix();
    a+=1; //l'angle de rotation de la lumière
    glPushMatrix();
        glLightfv( GL_LIGHT4, GL_SPOT_DIRECTION, spot1Dir );
        glLightfv(GL_LIGHT4,GL_POSITION,spot1Pos);
    glPopMatrix();
    // Et la LIGHT1 qui est un spot (dirigé vers la chaise)
    glPushMatrix();
        glTranslated(48,30,8);
        glLightfv( GL_LIGHT1, GL_POSITION, spot1Pos );
        glLightfv( GL_LIGHT1, GL_SPOT_DIRECTION, spot1Dir );

        glTranslated(0,0,-2); //et maintenant on eclaire l'interieur de la lampe pour le réalisme
        glLightfv( GL_LIGHT2, GL_POSITION, spot2Pos );
        glLightfv( GL_LIGHT2, GL_SPOT_DIRECTION, spot2Dir );
    glPopMatrix();
    glDisable(GL_COLOR_MATERIAL);

    //////////////////////////////
    //Dessin de lenvironnement; //
    //////////////////////////////
    dessinerRepere();
    glEnable ( GL_TEXTURE_2D );

    drawSkybox();

    ////On dessine le sol
    glBindTexture ( GL_TEXTURE_2D, textureSol );
    glBegin ( GL_TRIANGLES );
    for (int indice_y=0 ; indice_y <= _tailleSolY ; indice_y++)
    {
        for (int indice_x=0 ; indice_x <= _tailleSolX ; indice_x++)
        {
            glNormal3d (0,0,1);
            glTexCoord2d ( 0,0 );  glVertex3d ( indice_x, indice_y, 0 );
            glTexCoord2d ( 1,0 );  glVertex3d ( indice_x+1,indice_y,0 );
            glTexCoord2d ( 1,1 );  glVertex3d ( indice_x+1,indice_y+1,0 );

            glNormal3d (0,0,1);
            glTexCoord2d ( 1,1 );  glVertex3d ( indice_x+1,indice_y+1,0 );
            glTexCoord2d ( 0,1 );  glVertex3d ( indice_x,indice_y+1,0 );
            glTexCoord2d ( 0,0 );  glVertex3d ( indice_x, indice_y, 0 );
        }
    }
    glEnd();


    // On affiche maintenant tous les objets,
    // normalement il suffit d'appeler la fonction afficherObjet() de chaque objet.
    // Mais il faut d'abord dessiner ceux qui sont opaques, puis desactiver le z-buffer en ecriture, et dessiner
    // les face transparentes pour que les pixel transparent ne cache pas les objet derriere, et que ces faces
    // transparente soit caché par les opaque devant (c'est pour cette derniere raison qu'on ne desactive pas le DEPTH_TEST;
    int indexObjet = 0;
    vector< int > tableau(_nombreObjets,0);

    for (int i=0 ; i < _nombreObjets ; i++)
            p_listeObjets[i]->afficherObjet();

    glDepthMask(GL_FALSE); //on desactive le z-buffer EN ECRITURE, sinon les face transparentes serait dessiné au dessus des opaques
    for (int i=0 ; i < indexObjet ; i++)
        p_listeObjets[tableau[i]]->afficherObjet();

    glDepthMask(GL_TRUE);

}

