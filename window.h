#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class QSlider;
class QPushButton;

class GameWindow;
class MainWindow;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(MainWindow *mw);

private:
    GameWindow *glWidget;
    MainWindow *mainWindow;
};

#endif
