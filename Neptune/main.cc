//
// File:   main.cpp
// Author: trevone
//
// Created on 15 March 2013, 12:56
///

#include <iostream>
#include <QApplication>
#include <QMainWindow>
#include <QPixmap>
#include <QSplashScreen>
#include <QString>
#include "Config.h"
#include "MainWindow.h"
#include "Neptune.h"
#include "Splash.h"


int main(int argc, char *argv[])
{
    Neptune app(argc, argv);

    QPixmap pixmap(":/images/neptune.icon.png");
    QIcon icon(pixmap);
    QApplication::setWindowIcon(icon);
    
    Splash* splash = new Splash();
    splash->display();
    delete splash;

    MainWindow window;
    window.show();
    window.start();
    return app.exec();
}
