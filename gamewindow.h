#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QKeyEvent>
#include <QMap>
#include <QElapsedTimer>
#include <QPainter>
#include "texturemanager.h"
#include "audiosource.h"
#include "gameobject.h"
#include "player.h"
#include "flak.h"
#include "enemy.h"
#include "waypoint.h"
#include "boxcollider.h"
#include "hudelement.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

QT_FORWARD_DECLARE_CLASS(Shader)

class GameWindow : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT
public:
    static GameWindow* instance;

    GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

    QSize sizeHint() const override;

Cube* test2;
    QVector<GameObject*> gameObjects;
    QMap<QString,Shader*> shaders;

    Player* player;
    QVector3D playerDirection;
    QVector3D cameraOffset = QVector3D(0,2,0);


    Cube* skybox;
    QVector<GameObject*> skytrains;
    QVector<Enemy*> enemies;
    Flak* flak;
    HUDElement* screen;
    HUDElement* vignette;
    QVector3D cameraDirection;

    bool lockCursor = true;
    unsigned long m_timer = 0;
    QElapsedTimer timerSinceStart;

    void BindCurrentShader(GameObject* go);


    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    void setTransforms(void);
    void qNormalizeAngle(float &angle);

    void GenerateDepthMap();
    void Update();
    void Render();

    QMatrix4x4 m_proj;
    QMatrix4x4 m_camera;
    QMatrix4x4 m_world;
    float m_camDistance = 3.5f;

public slots:
    void setXRotation(float angle);
    void setYRotation(float angle);
    void setZRotation(float angle);
    void cleanup();

signals:
    void xRotationChanged(float angle);
    void yRotationChanged(float angle);
    void zRotationChanged(float angle);




private:
        struct LightLocStruct
        {
            int position;
            int ambient;
            int diffuse;
        };

        QPoint m_lastPos;
        QOpenGLShaderProgram *m_program;
        int m_projMatrixLoc;
        int m_viewMatrixLoc;
        int m_modelMatrixLoc;
        int m_modelColorLoc;
        int m_hasTextureLoc;
        LightLocStruct m_lightLoc;



       // QMap<QString, Model*> m_models;

        bool m_keyState[256];
        bool mouseButtonState[2] = {false,false};

        float m_camXRot = 15;
        float m_camYRot = 330;
        float m_camZRot = 0;

};

#endif // GAMEWINDOW_H
