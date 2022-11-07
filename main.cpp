#include "soundmanager.h"
#include "mainwindow.h"
#include <QApplication>
#include <iostream>



int main(int argc, char *argv[])
{
    //Forces the usage of desktop OpenGL - Qt uses OpenGL ES as default
    //Attribute must be set before Q(Gui)Application is constructed:
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);

    //Makes a Qt application
    QApplication a(argc, argv);

    SoundManager::getInstance()->init();

    //Makes the Qt MainWindow and shows it.
    MainWindow w;
    w.show();
    //~
    w.resize(1300, 800);
    //~//


    int x = a.exec();

    SoundManager::getInstance()->cleanUp();

    return x;
}
