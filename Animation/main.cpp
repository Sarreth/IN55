#include "MainWindow.h"
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPixmap pixmap("pics/splash.jpg");
    QSplashScreen splash(pixmap);

    splash.show();
    splash.showMessage("<font color=white size=30 stretch=100>Loading, please wait...");
    qApp->processEvents();

    MainWindow w;
    w.showFullScreen();
    splash.finish(&w);

    return a.exec();
}
