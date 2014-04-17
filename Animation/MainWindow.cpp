#include "MainWindow.h"

#define AVANCER 0
#define RECULER 1
#define GAUCHE 2
#define DROITE 3
#define SAUT 4
#define ACCROUPI 5

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    QApplication::setOverrideCursor( QCursor( Qt::BlankCursor ));
    this->setWindowTitle("IN55 Animation Project");

    enPause = false;
    rafraichirData = false;

    anti_repetition = false;

    int nbParamParObjet = 0;
    int nbParamDefinis = 0;
    QString nomObjet;
    bool isVisible = false;
    QVector3D positionObjet;

    bool possedeCollisionBox = false;
    QVector3D diagonaleCollisionBox;
    bool rotation90degCollisionBox = false;
    QVector3D orientationObjet;
    QString fichierMesh;
    QString fichierTexture;
    bool isTextureUVmap = false;
    float repeatTextX = 0.0;
    float repeatTextY = 0.0;

    QVector3D cibleCamera;
    float vitesseJoueur = 0;
    float sensivity = 0;

    QVector3D positionCamera;
    QFile file("listeObjetsMap.txt");
    if (file.open(QIODevice::ReadOnly) == false)
        qDebug() << "----->ERREUR 01 ; Chargement du fichier map : FAILED";

    nombreObjets = 0;

    QTextStream fichier(&file);
    while(!fichier.atEnd())
    {
        QString line = fichier.readLine();
        if (line[0] != '#')
        {
            QStringList mots = line.split(('='));
            if (mots[0]=="tailleTerrainX")
                _tailleTerrainX = mots[1].toInt();
            if (mots[0]=="tailleTerrainY")
                _tailleTerrainY = mots[1].toInt();
            if (mots[0]=="nbParamParObjet")
                nbParamParObjet = mots[1].toInt();;
            if (mots[0]=="!NouvelObjet")
                nbParamDefinis = 0;
            if (mots[0]=="nomObjet")
            {
                nomObjet = mots[1];
                nbParamDefinis++;
            }
            if (mots[0]=="isVisible")
            {
                isVisible = mots[1].toInt();
                nbParamDefinis++;
            }
            if (mots[0]=="posX")
            {
                positionObjet.setX(mots[1].toFloat());
                nbParamDefinis++;
            }
            if (mots[0]=="posY")
            {
                positionObjet.setY(mots[1].toFloat());
                nbParamDefinis++;
            }
            if (mots[0]=="posZ")
            {
                positionObjet.setZ(mots[1].toFloat());
                nbParamDefinis++;
            }
            if (mots[0]=="orientationX")
            {
                orientationObjet.setX(mots[1].toFloat());
                nbParamDefinis++;
            }
            if (mots[0]=="orientationY")
            {
                orientationObjet.setY(mots[1].toFloat());
                nbParamDefinis++;
            }
            if (mots[0]=="orientationZ")
            {
                orientationObjet.setZ(mots[1].toFloat());
                nbParamDefinis++;
            }
            if (mots[0]=="possedeCollisionBox")
            {
                possedeCollisionBox=mots[1].toInt();
                nbParamDefinis++;
            }
            if (mots[0]=="diagCollisionBoxX")
            {
                diagonaleCollisionBox.setX(mots[1].toFloat());
                nbParamDefinis++;
            }
            if (mots[0]=="diagCollisionBoxY")
            {
                diagonaleCollisionBox.setY(mots[1].toFloat());
                nbParamDefinis++;
            }
            if (mots[0]=="diagCollisionBoxZ")
            {
                diagonaleCollisionBox.setZ(mots[1].toFloat());
                nbParamDefinis++;
            }
            if (mots[0]=="rotation90degCollisionBox")
            {
                rotation90degCollisionBox=mots[1].toInt();
                nbParamDefinis++;
            }
            if (mots[0]=="fichierMesh")
            {
                fichierMesh = mots[1];
                nbParamDefinis++;
            }
            if (mots[0]=="fichierTexture")
            {
                fichierTexture = mots[1];
                nbParamDefinis++;
            }
            if (mots[0]=="isTextureUVmap")
            {
                isTextureUVmap = mots[1].toInt();
                nbParamDefinis++;
            }
            if (mots[0]=="repeatTextX")
            {
                repeatTextX = mots[1].toFloat();
                nbParamDefinis++;
            }
            if (mots[0]=="repeatTextY")
            {
                repeatTextY = mots[1].toFloat();
                nbParamDefinis++;
            }
            if (mots[0]=="cibleCameraX")
                cibleCamera.setX(mots[1].toFloat());
            if (mots[0]=="cibleCameraY")
                cibleCamera.setY(mots[1].toFloat());
            if (mots[0]=="cibleCameraZ")
                cibleCamera.setZ(mots[1].toFloat());
            if (mots[0]=="vitesseJoueur")
                vitesseJoueur = mots[1].toFloat();
            if (mots[0]=="sensivity")
                sensivity = mots[1].toFloat();
            if (mots[0]=="!FinObjet")
            {
                if (nbParamDefinis == nbParamParObjet)
                {
                    liste_objets[nombreObjets] = new Objet(isVisible, positionObjet, orientationObjet, possedeCollisionBox, diagonaleCollisionBox, rotation90degCollisionBox, nomObjet, fichierMesh, fichierTexture, isTextureUVmap, repeatTextX,repeatTextY);
                    nombreObjets++;
                }
                else
                    qDebug() << "Erreur 01 : Erreur chargement Objet, nombre parametre incorrect";
            }
            if (mots[0]=="!FinJoueur")
            {
                //de meme on cree le joueur (qui hérite de la classe Objet), donc juste quelques membres et attributs en plus (voir Personnage.cpp et Objets.cpp)
                joueur = new CameraLibre(isVisible, positionObjet, cibleCamera, orientationObjet, possedeCollisionBox, diagonaleCollisionBox, rotation90degCollisionBox, vitesseJoueur, sensivity, fichierMesh, fichierTexture, isTextureUVmap);
            }
        }
    }


    //////////////////////////////
    ////---Camera Principale--////
    //////////////////////////////
    positionCamera = joueur->getPosition();
    positionCamera+=QVector3D(0,0,5);//Pour mettre la camera à 5 au dessus du sol
    cibleCamera = joueur->getCibleCamera();
    QDesktopWidget widget;
    mainScreenSize = widget.availableGeometry(widget.primaryScreen());

    qDebug() << "+ Resolution de l'écran : " << mainScreenSize.width() << "x" << mainScreenSize.height();
    vuePrincipal = new OpenGLWidget (this, joueur, positionCamera, cibleCamera, _tailleTerrainX, _tailleTerrainY, liste_objets, nombreObjets,"Vue Principale");

    setCentralWidget(vuePrincipal);


    timerJeu = new QTimer(this);
    connect(timerJeu, SIGNAL(timeout()), this, SLOT(cycleTimerJeu()));

    timerFPS = new QTimer(this);

    qDebug() <<  "+ Chargement fenetre principale : OK" ;
    lancerJeu();

    this->setMouseTracking(true);
    vuePrincipal->setMouseTracking(true);

}

MainWindow::~MainWindow()
{

}

void MainWindow::lancerJeu()
{
        timerJeu->start(20);
        timerFPS->start(1000);
}


void MainWindow::stopperJeu()
{
        timerJeu->stop();
        timerFPS->stop();
}

void MainWindow::cycleTimerJeu()
{
    _numCycle++;
    joueur->Animate(liste_objets, nombreObjets, _tailleTerrainX, _tailleTerrainY);

    vuePrincipal->updateGL();
}

void MainWindow::mouseMoveEvent ( QMouseEvent *event )
{
    if ( anti_repetition == false )//on verifie que ce n'est pas la fonction qui se rappelle elle meme (avec setPos) et que la souris est sur le bon widget
    {
        int xrel = (mainScreenSize.width()/2 - event->x());
        int yrel = ( mainScreenSize.height()/2 - event->y());

        joueur->mouvementSouris ( xrel, yrel );
        QPoint pos(mainScreenSize.width()/2,mainScreenSize.height()/2);

        QCursor::setPos(mapToGlobal(pos));

        anti_repetition = true;
    }
    else
        anti_repetition = false;
}



void MainWindow::wheelEvent ( QWheelEvent *event )
{
    float vitesseJoueur = joueur->getVitesse();

    if ( event->delta() >= 0 )
    {
        vitesseJoueur += 0.1;
        if (vitesseJoueur > 6)
            vitesseJoueur = 6;

        joueur->setVitesse(vitesseJoueur);
    }
    else
    {
        vitesseJoueur -= 0.1;
        if (vitesseJoueur < 0)
            vitesseJoueur = 0;
        joueur->setVitesse(vitesseJoueur);
    }
    qDebug() << "Speed =" << vitesseJoueur;
}


void MainWindow::keyPressEvent ( QKeyEvent *event )
{
    QVector3D _posi(joueur->getPosition());

    switch ( event->key() )
    {
        case Qt::Key_Escape:
            qApp->exit();
            break;
        case Qt::Key_P:
            if ( !enPause )
            {
                enPause = true;
                stopperJeu();
            }
            else
            {
                enPause = false;
                lancerJeu();
            }
            break;
        case Qt::Key_Z:
            joueur->deplacement ( AVANCER, true );
            break;
        case Qt::Key_S:
            joueur->deplacement ( RECULER, true );
            break;
        case Qt::Key_Q:
            joueur->deplacement ( GAUCHE, true );
            break;
        case Qt::Key_D:
            joueur->deplacement ( DROITE, true );
            break;
        case Qt::Key_Control:
            qDebug() << "A genou";
            joueur->deplacement ( ACCROUPI, true );
            break;
        case Qt::Key_Space:
            //qDebug() << "Space";
            joueur->deplacement ( SAUT, true );
            break;
        case Qt::Key_Shift:
            qDebug() << "Pos = " << _posi.x() << _posi.y() << _posi.z();
            break;
    }
}

void MainWindow::keyReleaseEvent ( QKeyEvent * event )
{
    switch ( event->key() )
    {
        case Qt::Key_Z:
            joueur->deplacement ( AVANCER, false );
            break;
        case Qt::Key_S:
            joueur->deplacement ( RECULER, false );
            break;
        case Qt::Key_Q:
            joueur->deplacement ( GAUCHE, false );
            break;
        case Qt::Key_D:
            joueur->deplacement ( DROITE, false );
            break;
        case Qt::Key_Control:
            qDebug() << "Debout";
            joueur->deplacement ( ACCROUPI, false );
            break;
        case Qt::Key_Space:
            //qDebug() << "Space";
            joueur->deplacement ( SAUT, false );
            break;

    }
}
