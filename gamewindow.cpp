#define _USE_MATH_DEFINES

#include "gamewindow.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <math.h>
#include <iostream>
#include <qstack.h>
#include "cube.h"
#include "shader.h"

using namespace std;

GameWindow* GameWindow::instance;

GameWindow::GameWindow(QWidget *parent) : QOpenGLWidget(parent), m_program(nullptr), cameraDirection(0,0,-1)
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    /*QCursor c = cursor();
    c.setShape(Qt::CursorShape::BlankCursor);
    setCursor(c);*/
    instance=this;
    timerSinceStart.start();
}

GameWindow::~GameWindow()
{
    for(auto it = gameObjects.begin() ; it != gameObjects.end(); it++)
        delete (*it);

    cleanup();
}

QSize GameWindow::sizeHint() const
{
    return QSize(1000, 800);
}

void GameWindow::cleanup()
{
    if (m_program == nullptr)
        return;
    makeCurrent();


    delete m_program;
    m_program = nullptr;
    doneCurrent();
}

void GameWindow::qNormalizeAngle(float &angle)
{
    while (angle < 0)
        angle += 360;
    while (angle > 360)
        angle -= 360;
}

void GameWindow::setXRotation(float angle)
{
    qNormalizeAngle(angle);
    if (abs(angle - m_camXRot) > 0.001f) {
        m_camXRot = angle;
        emit xRotationChanged(angle);
        update();
    }
}

void GameWindow::setYRotation(float angle)
{
    qNormalizeAngle(angle);
    if (abs(angle - m_camYRot) > 0.001f) {
        m_camYRot = angle;
        emit yRotationChanged(angle);
        update();
    }
}

void GameWindow::setZRotation(float angle)
{
    qNormalizeAngle(angle);
    if (abs(angle - m_camZRot) > 0.001f) {
        m_camZRot = angle;
        emit zRotationChanged(angle);
        update();
    }
}


void GameWindow::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.0,0.0,0.0,1.0);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);


    // Create Shader (Do not release until VAO is created)

    shaders.push_back(new Shader("simple", "resources/shaders/simple.vert", "resources/shaders/simple.frag"));
    shaders.push_back(new Shader("skinned", "resources/shaders/skinned2.vert", "resources/shaders/simple.frag"));

    m_program=shaders[0]->program;
    /*m_program = new QOpenGLShaderProgram();
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "resources/shaders/simple.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "resources/shaders/simple.frag");

    m_program->bindAttributeLocation("vertex", 0);
    m_program->bindAttributeLocation("normal", 1);
    m_program->link();*/




   /* m_models.insert("Cube", new Model());
    m_models["Cube"]->meshes.push_back(new Mesh());
    m_models["Cube"]->meshes[0]->generateCube(1,1,1);
    m_models.insert("Test", new Model());
    m_models["Test"]->meshes.push_back(new Mesh());
    m_models["Test"]->meshes[0]->generateMeshFromObjFile("resources/meshes/bunny.obj");*/


    Model* testmodel=new Model("resources/meshes/test.fbx");
    for (int i=0; i < 36; i++)
    {
        Cube* c = new Cube(testmodel);
        c->shader = shaders[0];
        //c->model->meshes.push_front(new Mesh());
        //c->model->meshes[0]->generateCube(1,1,1);

        c->position = QVector3D(sin(i)*5,0,cos(i)*5);
        gameObjects.push_front(c);
    }

   /* Cube* trooper = new Cube(new Model("resources/meshes/paratrooper.fbx"));
    trooper->scale = QVector3D(0.05,0.05,0.05);
    trooper->shader = shaders[0];
    gameObjects.push_front(trooper);*/

    player = new Player("resources/meshes/paratrooper.fbx");
    player->scale = QVector3D(0.0005,0.0005,0.0005);
    player->shader = shaders[1];
    gameObjects.push_front(player);





    //gameObjects.insert("Paratrooper", new Model("resources/meshes/paratrooper.fbx"));
    //m_models["Paratrooper"]->LoadModel("resources/meshes/test.fbx");


    // Release (unbind) all
    //m_program->release();
}

void GameWindow::BindCurrentShader(Shader* shader)
{
    m_program->release();

    m_program=shader->program;
    m_program->bind();
    m_projMatrixLoc = m_program->uniformLocation("projMatrix");
    m_viewMatrixLoc = m_program->uniformLocation("viewMatrix");
    m_modelMatrixLoc = m_program->uniformLocation("modelMatrix");
    m_modelColorLoc = m_program->uniformLocation("modelColor");
    m_lightLoc.position = m_program->uniformLocation("light.position");
    m_lightLoc.ambient = m_program->uniformLocation("light.ambient");
    m_lightLoc.diffuse = m_program->uniformLocation("light.diffuse");

    m_program->setUniformValue(m_lightLoc.position, QVector3D(0.0f, 0.0f, 15.0f));
    m_program->setUniformValue(m_lightLoc.ambient, QVector3D(0.1f, 0.1f, 0.1f));
    m_program->setUniformValue(m_lightLoc.diffuse, QVector3D(0.9f, 0.9f, 0.9f));
}



void GameWindow::paintGL()
{
    update();
    for (int i=0; i < gameObjects.size(); i++)
    {
        gameObjects[i]->Update();
    }




    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    QCursor::setPos(mapToGlobal(QPoint(width()/2, height()/2)));

    QStack<QMatrix4x4> worldMatrixStack;

    //m_program->bind();



    m_camera.setToIdentity();
    m_world.setToIdentity();


    QVector3D cameraOffset(QVector3D(0,2,0));
    m_camera.lookAt(player->position - m_camDistance * cameraDirection + cameraOffset,
                    player->position + cameraOffset,
                    QVector3D(0, 1, 0)
                    );

    //m_camera.lookAt(QVector3D(0,0,0), QVector3D(-5,0,0), QVector3D(0,1,0));


    float phi = atan2(cameraDirection.z(), cameraDirection.x());


    if(m_keyState[Qt::Key_Z]) m_camDistance += 0.05f;
    if(m_keyState[Qt::Key_X]) m_camDistance -= 0.05f;

    if (m_keyState[Qt::Key_W])
    {
        player->position.setX(player->position.x() + cameraDirection.x() * player->speed);
        player->position.setZ(player->position.z() + cameraDirection.z() * player->speed);
        //player->rotation = QQuaternion::fromEulerAngles(0,-phi * 180.0f / M_PI,0);
        phi = atan2(cameraDirection.z(), cameraDirection.x());
    }
    if (m_keyState[Qt::Key_S])
    {
        player->position.setX(player->position.x() - cameraDirection.x() * player->speed);
        player->position.setZ(player->position.z() - cameraDirection.z() * player->speed);
        player->rotation = QQuaternion::fromEulerAngles(0,-phi * 180.0f / M_PI,0);
        //player->rotation=player->rotation.inverted();
        phi = atan2(-cameraDirection.z(), -cameraDirection.x());
    }
    if (m_keyState[Qt::Key_A])
    {
        player->position.setX(player->position.x() + cameraDirection.z() * player->speed);
        player->position.setZ(player->position.z() - cameraDirection.x() * player->speed);
        //player->rotation = QQuaternion::fromEulerAngles(0,(-phi-80) * 180.0f / M_PI,0);
        phi = atan2(cameraDirection.z(), cameraDirection.x()) - 80.0f;
    }
    if (m_keyState[Qt::Key_D])
    {
        player->position.setX(player->position.x() - cameraDirection.z() * player->speed);
        player->position.setZ(player->position.z() + cameraDirection.x() * player->speed);
        //player->rotation = QQuaternion::fromEulerAngles(0,(-phi+80) * 180.0f / M_PI,0);
        phi = atan2(cameraDirection.z(), cameraDirection.x()) + 80.0f;
    }
    if (m_keyState[Qt::Key_R])
    {
        for (auto i : shaders)
            i->Reload();
    }

    player->rotation = player->rotation.fromEulerAngles(0,-phi * 180.0f / M_PI,0);

    for (int i=0; i < gameObjects.size(); i++)
    {
        worldMatrixStack.push(m_world);
        gameObjects[i]->Render(&m_world);
        m_program->setUniformValue(m_modelColorLoc,QVector3D(1.0f, 1.0, 1.0));

        m_world = worldMatrixStack.pop();
    }

    m_program->release();

    m_timer++;





}

void GameWindow::setTransforms(void)
{
    m_program->setUniformValue(m_projMatrixLoc, m_proj);
    m_program->setUniformValue(m_viewMatrixLoc, m_camera);
    m_program->setUniformValue(m_modelMatrixLoc, m_world);
}

void GameWindow::resizeGL(int w, int h)
{
    m_proj.setToIdentity();
    m_proj.perspective(60.0f, GLfloat(w) / h, 0.01f, 100.0f);
}

void GameWindow::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
}

void GameWindow::mouseMoveEvent(QMouseEvent *event)
{
    /*int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();*/

    int dx = event->x() - width()/2;
    int dy = event->y() - height()/2;


    if (event->buttons() & Qt::LeftButton) {
        setXRotation(m_camXRot + 0.5f * dy);
        setYRotation(m_camYRot + 0.5f * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(m_camXRot + 0.5f * dy);
        setZRotation(m_camZRot + 0.5f * dx);
    }

    float r = 1;
    float phi = atan2(cameraDirection.z(), cameraDirection.x());
    float theta = acos(cameraDirection.y() / r);

    phi = phi + dx * 0.01;
    theta = theta + dy * 0.01;

    if(theta < 0.01) theta = 0.01;
    if(theta > 3.14) theta = 3.14;

    cameraDirection.setX(r*sin(theta)*cos(phi));
    cameraDirection.setY(r*cos(theta));
    cameraDirection.setZ(r*sin(theta)*sin(phi));

    m_lastPos = event->pos();
}

void GameWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
        exit(0);
    else
        QWidget::keyPressEvent(e);

    if(e->key() >= 0 && e->key() <= 255)
        m_keyState[e->key()] = true;
}

void GameWindow::keyReleaseEvent(QKeyEvent *e)
{
    if(e->key() >= 0 && e->key() <= 255)
        m_keyState[e->key()] = false;
}
