#include <QApplication>
#include <QDesktopWidget>
#include <QSurfaceFormat>
#include <iostream>

#include "gamewindow.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // creates object for MainWindow class
    qputenv("QT_OPENGL", "desktop");
    MainWindow mainWindow;
    mainWindow.resize(mainWindow.sizeHint()); // size of the main window (defined in GLWidget)
    mainWindow.show(); // shows main window
    //mainWindow.showMaximized(); // main window will be maximized

    return a.exec();
}
