#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QKeyEvent>
#include <QMap>
#include "player.h"
#include "mesh.h"
#include "model.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GameWindow : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT
public:
    GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

    QSize sizeHint() const override;

    friend Mesh;
    friend Model;

    Player player;

public slots:
    void setXRotation(float angle);
    void setYRotation(float angle);
    void setZRotation(float angle);
    void cleanup();

signals:
    void xRotationChanged(float angle);
    void yRotationChanged(float angle);
    void zRotationChanged(float angle);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    void setTransforms(void);
    void qNormalizeAngle(float &angle);

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
        LightLocStruct m_lightLoc;

        QMatrix4x4 m_proj;
        QMatrix4x4 m_camera;
        QMatrix4x4 m_world;

        QMap<QString, Model*> m_models;

        bool m_keyState[256];

        float m_camXRot = 15;
        float m_camYRot = 330;
        float m_camZRot = 0;
        float m_camDistance = 3.5f;

        int m_timer;
};

#endif // GAMEWINDOW_H
