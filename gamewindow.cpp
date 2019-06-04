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
#include "particleeffect.h"
#include "spheretrigger.h"
#include <stdio.h>

using namespace std;

GameWindow* GameWindow::instance;

GameWindow::GameWindow(QWidget *parent) : QOpenGLWidget(parent), m_program(nullptr), cameraDirection(0,0,-1)
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    for (int i=0; i < 255; i++)
        m_keyState[i] = false;


    /*QCursor c = cursor();
    c.setShape(Qt::CursorShape::BlankCursor);
    setCursor(c);*/
    instance=this;
    setAutoFillBackground(false);
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
    return QSize(1600, 900);
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

    shaders.insert("simple", new Shader("simple", "resources/shaders/simple.vert", "resources/shaders/simple.frag"));
    shaders.insert("skinned", new Shader("skinned", "resources/shaders/skinned2.vert", "resources/shaders/simple.frag"));
    shaders.insert("skybox", new Shader("skybox", "resources/shaders/simple.vert", "resources/shaders/skybox.frag"));
    TextureManager::Init();
    Waypoint::SetupWaypoints();


    m_program=shaders["simple"]->program;

    Cube* terrain=new Cube(new Model("resources/meshes/terrain.fbx"));
    terrain->texture=TextureManager::GetTexture("terrain");
    terrain->shader=shaders["simple"];
    terrain->rotation = QQuaternion::fromEulerAngles(QVector3D(-90,0,0));
    terrain->scale = QVector3D(5,5,5);
    gameObjects.push_back(terrain);

    Cube* fences=new Cube(new Model("resources/meshes/fences.fbx"));
    fences->texture=TextureManager::GetTexture("fences");
    fences->shader=shaders["simple"];
    fences->rotation = QQuaternion::fromEulerAngles(QVector3D(-90,0,0));
    fences->scale = QVector3D(5,5,5);
    gameObjects.push_back(fences);

    Cube* hedges=new Cube(new Model("resources/meshes/hedges.fbx"));
    hedges->texture=TextureManager::GetTexture("hedges");
    hedges->shader=shaders["simple"];
    hedges->rotation = QQuaternion::fromEulerAngles(QVector3D(-90,0,0));
    hedges->scale = QVector3D(5,5,5);
    gameObjects.push_back(hedges);

    Cube* trees=new Cube(new Model("resources/meshes/trees.fbx"));
    trees->texture=TextureManager::GetTexture("trees");
    trees->shader=shaders["simple"];
    trees->rotation = QQuaternion::fromEulerAngles(QVector3D(-90,0,0));
    trees->scale = QVector3D(5,5,5);
    gameObjects.push_back(trees);

    Cube* houses=new Cube(new Model("resources/meshes/houses.fbx"));
    houses->texture=TextureManager::GetTexture("houses");
    houses->shader=shaders["simple"];
    houses->rotation = QQuaternion::fromEulerAngles(QVector3D(-90,0,0));
    houses->scale = QVector3D(5,5,5);
    gameObjects.push_back(houses);

    Cube* parachute=new Cube(new Model("resources/meshes/parachute.fbx"));
    parachute->texture=TextureManager::GetTexture("parachute");
    parachute->shader=shaders["simple"];
    parachute->rotation = QQuaternion::fromEulerAngles(QVector3D(-90,0,0));
    parachute->scale = QVector3D(5,5,5);
    gameObjects.push_back(parachute);

    Cube* crates=new Cube(new Model("resources/meshes/crates.fbx"));
    crates->texture=TextureManager::GetTexture("crate");
    crates->shader=shaders["simple"];
    crates->rotation = QQuaternion::fromEulerAngles(QVector3D(-90,0,0));
    crates->scale = QVector3D(5,5,5);
    gameObjects.push_back(crates);

    Cube* shells=new Cube(new Model("resources/meshes/shells.fbx"));
    shells->texture=TextureManager::GetTexture("shell");
    shells->shader=shaders["simple"];
    shells->rotation = QQuaternion::fromEulerAngles(QVector3D(-90,0,0));
    shells->scale = QVector3D(5,5,5);
    gameObjects.push_back(shells);

    player = new Player("resources/meshes/paratrooper.fbx");
    player->texture=TextureManager::GetTexture("paratrooper");
    player->scale = QVector3D(0.0005,0.0005,0.0005);
    player->position = QVector3D(35, 0, 67);
    player->shader = shaders["skinned"];
    gameObjects.push_front(player);

    skybox = new Cube(new Model("resources/meshes/skybox.fbx"));
    skybox->scale = QVector3D(1000,1000,1000);
    skybox->rotation = QQuaternion::fromEulerAngles(QVector3D(-90,0,0));
    skybox->shader = shaders["skybox"];
    skybox->texture=TextureManager::GetTexture("skybox");

    //main plane
        Cube* skytrain = new Cube(new Model("resources/meshes/plane.fbx"));
        skytrain->scale = QVector3D(10,10,10);
        skytrain->position = QVector3D(0,90,-600);
        skytrain->rotation = QQuaternion::fromEulerAngles(QVector3D(90,-90,0));
        skytrain->shader = shaders["simple"];
        skytrain->texture=TextureManager::GetTexture("skytrain");
        gameObjects.push_front(skytrain);
        skytrains.push_back(skytrain);

        skytrain=new Cube(*skytrain);
        skytrain->position+=QVector3D(50,0,50);
        gameObjects.push_front(skytrain);
        skytrains.push_back(skytrain);

        skytrain=new Cube(*skytrain);
        skytrain->position+=QVector3D(-100,0,30);
        gameObjects.push_front(skytrain);
        skytrains.push_back(skytrain);


    //left side planes
    for (int i = 0; i < 5; ++i) {
        skytrain=new Cube(*skytrain);
        skytrain->position = QVector3D(200+(rand() % 200),90+(rand() % 30),1000+(rand() % 400));
        gameObjects.push_front(skytrain);
        skytrains.push_back(skytrain);
    }

    for (int i = 0; i < 5; ++i) {
        skytrain=new Cube(*skytrain);
        skytrain->position = QVector3D(-200-(rand() % 200),90+(rand() % 30),1000+(rand() % 400));
        gameObjects.push_front(skytrain);
        skytrains.push_back(skytrain);
    }


    flak = new Flak("resources/meshes/flak.fbx");
    flak->position = QVector3D(-16,0,13);
    flak->scale = QVector3D(0.02,0.02,0.02);
    flak->rotation = QQuaternion::fromEulerAngles(QVector3D(0,0,0));
    flak->shader = shaders["skinned"];
    flak->texture=TextureManager::GetTexture("flak");
    gameObjects.push_front(flak);

    Enemy* enemy = new Enemy("resources/meshes/german.fbx");
    enemy->position=flak->position+QVector3D(2,0,5);
    enemy->rotation=QQuaternion::fromEulerAngles(QVector3D(0,90,0));
    enemy->shader=shaders["skinned"];
    enemy->texture=TextureManager::GetTexture("german");
    enemy->scale = QVector3D(0.0005,0.0005,0.0005);
    enemies.push_back(enemy);
    gameObjects.push_back(enemy);

    enemy = new Enemy(*enemy);
    enemy->position=flak->position+QVector3D(-0.75f,0,1.4f);
    enemy->rotation=QQuaternion::fromEulerAngles(QVector3D(0,90,0));
    enemy->PlayAnimation("OperateFlak",false);
    enemies.push_back(enemy);
    gameObjects.push_back(enemy);

    enemy = new Enemy(*enemy);
    enemy->position=flak->position+QVector3D(-4,0,-5);
    enemy->rotation=QQuaternion::fromEulerAngles(QVector3D(0,-90,0));
    enemies.push_back(enemy);
    gameObjects.push_back(enemy);

    SphereTrigger* farmTrigger = new SphereTrigger("farm", QVector3D(-10,0,46), 15);
    gameObjects.push_back(farmTrigger);

    AudioSource::Init();


    // Release (unbind) all
    //m_program->release();
}

void GameWindow::BindCurrentShader(GameObject* go)
{
    m_program->release();

    m_program=go->shader->program;
    m_program->bind();
    m_projMatrixLoc = m_program->uniformLocation("projMatrix");
    m_viewMatrixLoc = m_program->uniformLocation("viewMatrix");
    m_modelMatrixLoc = m_program->uniformLocation("modelMatrix");
    m_modelColorLoc = m_program->uniformLocation("modelColor");
    m_lightLoc.position = m_program->uniformLocation("light.position");
    m_lightLoc.ambient = m_program->uniformLocation("light.ambient");
    m_lightLoc.diffuse = m_program->uniformLocation("light.diffuse");
    m_hasTextureLoc = m_program->uniformLocation("hasTexture");

    m_program->setUniformValue(m_lightLoc.position, QVector3D(0.0f, 0.0f, 15.0f));
    m_program->setUniformValue(m_lightLoc.ambient, QVector3D(0.4f, 0.4f, 0.4f));
    m_program->setUniformValue(m_lightLoc.diffuse, QVector3D(0.2f, 0.2f, 0.2f));
    m_program->setUniformValue(m_modelColorLoc,QVector3D(1.0f, 1.0, 1.0));

    if (go->texture != nullptr)
    {
        m_program->setUniformValue(m_hasTextureLoc, 1);
        go->texture->bind();
    }
    else
        m_program->setUniformValue(m_hasTextureLoc, 0);
}

void GameWindow::Render()
{
    QStack<QMatrix4x4> worldMatrixStack;

    //m_program->bind();

    m_world.setToIdentity();

    //m_camera.lookAt(QVector3D(0,0,0), QVector3D(-5,0,0), QVector3D(0,1,0));
    //float phi = atan2(cameraDirection.z(), cameraDirection.x());
    worldMatrixStack.push(m_world);
    skybox->Render(&m_world);
    m_world=worldMatrixStack.pop();

    for (int i=0; i < gameObjects.size(); i++)
    {
        worldMatrixStack.push(m_world);
        gameObjects[i]->Render(&m_world);

        m_world = worldMatrixStack.pop();
        m_program->release();
    }
}

void GameWindow::Update()
{
    ParticleEffect::Update();
    update();
    for (int i=0; i < skytrains.count();i++)
    {
        skytrains[i]->position -= QVector3D(0.0f,0,0.5f);
        if (skytrains[i]->position.z() < -800)
        {
            skytrains[i]->position=QVector3D(skytrains[i]->position.x(),skytrains[i]->position.y(),300);
            if (i==0)
                AudioSource::PlaySoundOnce("skytrain", 0.1f);
        }
    }


    for (int i=0; i < gameObjects.size(); i++)
    {
        gameObjects[i]->Update();
    }

    if (lockCursor)
        QCursor::setPos(mapToGlobal(QPoint(width()/2, height()/2)));

    if(m_keyState[Qt::Key_Z]) m_camDistance += 0.05f;
    if(m_keyState[Qt::Key_X]) m_camDistance -= 0.05f;

    player->desiredAnimationState = Player::Idle;

    if (player->canMove)
    {
        if (m_keyState[Qt::Key_W] || m_keyState[Qt::Key_S] || m_keyState[Qt::Key_A] || m_keyState[Qt::Key_D])
            playerDirection=QVector3D(0,0,0);

        float speedMultiplier=1.0f;
        if ((m_keyState[Qt::Key_W] && (m_keyState[Qt::Key_A] || m_keyState[Qt::Key_D])) || (m_keyState[Qt::Key_S] && (m_keyState[Qt::Key_A] || m_keyState[Qt::Key_D])))
                speedMultiplier=0.75f;

        if (m_keyState[Qt::Key_W])
        {
            player->position.setX(player->position.x() + cameraDirection.x() * player->speed * speedMultiplier);
            player->position.setZ(player->position.z() + cameraDirection.z() * player->speed * speedMultiplier);
            playerDirection+=QVector3D(cameraDirection.x(),0,cameraDirection.z());
            player->desiredAnimationState = Player::Running;
        }
        if (m_keyState[Qt::Key_S])
        {
            player->position.setX(player->position.x() - cameraDirection.x() * player->speed * speedMultiplier);
            player->position.setZ(player->position.z() - cameraDirection.z() * player->speed * speedMultiplier);
            playerDirection+=QVector3D(-cameraDirection.x(),0,-cameraDirection.z());
            player->desiredAnimationState = Player::Running;
        }

        if (m_keyState[Qt::Key_A])
        {
            player->position.setX(player->position.x() + cameraDirection.z() * player->speed * speedMultiplier);
            player->position.setZ(player->position.z() - cameraDirection.x() * player->speed * speedMultiplier);
            playerDirection+=QVector3D(cameraDirection.z(),0,-cameraDirection.x());
            player->desiredAnimationState = Player::Running;
        }
        if (m_keyState[Qt::Key_D])
        {
            player->position.setX(player->position.x() - cameraDirection.z() * player->speed * speedMultiplier);
            player->position.setZ(player->position.z() + cameraDirection.x() * player->speed * speedMultiplier);
            playerDirection+=QVector3D(-cameraDirection.z(),0,cameraDirection.x());
            player->desiredAnimationState = Player::Running;
        }
    }

    if (m_keyState[Qt::Key_K])
    {
        char c[1024];
        sprintf(c, "%f %f %f\n", player->position.x(), player->position.y(), player->position.z());
        qDebug(c);
    }

    if (m_keyState[Qt::Key_1])
    {
        for (auto i : shaders)
            i->Reload();
    }

    if (mouseButtonState[1])
    {
        player->desiredAnimationState = Player::Aiming;
        //playerDirection=QVector3D(cameraDirection.x(),0,cameraDirection.z());
        playerDirection=QVector3D(cameraDirection.x(),0,cameraDirection.z());
        QVector3D right = QVector3D::crossProduct(playerDirection.normalized(), QVector3D(0,1,0));
        cameraOffset = QVector3D(0, 2, 0) + (right * 0.7f);
        m_camDistance=2.0f;
        player->canMove=false;
    }
    else {
        cameraOffset = QVector3D(0, 2, 0);
        m_camDistance=3.5f;
        player->canMove=true;
    }

    if (player->desiredAnimationState == Player::Aiming)
    {
        if (mouseButtonState[0])
            player->Shoot();
    }
    if (m_keyState[Qt::Key_R])
    {
        player->Reload();
        player->currentAmmo=player->maxAmmo;
    }

    player->rotation = player->rotation.fromEulerAngles(0,-atan2(playerDirection.z(), playerDirection.x()) * 180.0f / M_PI,0);

    skybox->position = player->position + QVector3D(0,15,0);

    m_timer++;

}


void GameWindow::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    m_camera.setToIdentity();
    m_camera.lookAt(player->position - m_camDistance * cameraDirection + cameraOffset,
                    player->position + cameraOffset,
                    QVector3D(0, 1, 0)
                    );
    Render();
    Update();

    m_program->release();
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    QPainter painter(this);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);


    painter.setPen(Qt::white);

    //painter.setFont(QFont("Arial", 25));
   /* QRect r(width()*0.8f, height() * 0.7f, width() * 0.2f, height()*0.1f);
    painter.drawText(r, Qt::AlignCenter, QString::number(player->currentAmmo));


    if (player->currentAmmo == 0)
    {
        r=QRect(width()*0.25f, height() * 0.05f, width() * 0.5f, height()*0.1f);
        painter.drawText(r, Qt::AlignCenter, "Przeladuj! (R)");
    }*/
    painter.end();
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
    m_proj.perspective(60.0f, GLfloat(w) / h, 0.01f, 2000000000.0f);
}

void GameWindow::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
    mouseButtonState[0]=event->buttons() & Qt::MouseButton::LeftButton;
    mouseButtonState[1]=event->buttons() & Qt::MouseButton::RightButton;
}

void GameWindow::mouseReleaseEvent(QMouseEvent *event)
{
    mouseButtonState[0]=event->buttons() & Qt::MouseButton::LeftButton;
    mouseButtonState[1]=event->buttons() & Qt::MouseButton::RightButton;
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
        lockCursor=!lockCursor; //exit(0);
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
