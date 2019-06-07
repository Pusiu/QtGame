#include "hudelement.h"
#include "gamewindow.h"
#include "model.h"
#include <cmath>

HUDElement::HUDElement(float startAlpha)
{
    m = new Model("resources/meshes/plane.fbx");
    shader=GameWindow::instance->shaders["hud"];
    alpha=startAlpha;
    isTransparent=true;
}

float startAlpha;
void HUDElement::StartTransition(float timeOffset)
{
    startTime=GameWindow::instance->timerSinceStart.elapsed() + (timeOffset*1000);
    if (alpha == 1)
    {
        startAlpha=1;
        desiredAlpha=0;
    }
    else
    {
        startAlpha=0;
        desiredAlpha=1;
    }
}

void HUDElement::Update()
{
    /*this->position=GameWindow::instance->player->position - GameWindow::instance->m_camDistance * GameWindow::instance->cameraDirection + GameWindow::instance->cameraOffset;
    position+=GameWindow::instance->cameraDirection*1.6f;
    QQuaternion q = QQuaternion::fromDirection(-GameWindow::instance->cameraDirection.normalized(), QVector3D(0,1,0));
    QVector3D euler = q.toEulerAngles();
    rotation = QQuaternion::fromEulerAngles(euler.x(),euler.y(), -90);*/

    long t = GameWindow::instance->timerSinceStart.elapsed();
    if (startTime <= t && alpha != desiredAlpha)
    {
        alpha = startAlpha + ((t-startTime)/(transitionTime*1000))*(desiredAlpha-startAlpha);
    }

}

void HUDElement::Render(QMatrix4x4 *world)
{
    GameObject::Render(world);
    this->shader->program->setUniformValue("alpha", alpha);
    this->shader->program->setUniformValue("rect.xPos", 30);
    this->shader->program->setUniformValue("rect.yPos", 30);
    this->shader->program->setUniformValue("rect.width", 200);
    this->shader->program->setUniformValue("rect.height", 100);
    this->shader->program->setUniformValue("resolution", QVector2D(GameWindow::instance->width(), GameWindow::instance->height()));
    m->Draw();
}
