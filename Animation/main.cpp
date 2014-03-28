#include <QApplication>
#include <QtOpenGL/QGLWidget>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.showFullScreen();

    return a.exec();
}
