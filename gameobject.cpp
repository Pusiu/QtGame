#include "gameobject.h"
#include <gamewindow.h>

GameObject::GameObject()
{
    position = QVector3D(0,0,0);
    rotation = QQuaternion::fromEulerAngles(QVector3D(0,0,0));
    scale = QVector3D(1,1,1);
}

GameObject::GameObject(GameObject &ref)
{
    this->position=ref.position;
    this->rotation=ref.rotation;
    this->scale=ref.scale;
    this->shader=ref.shader;
    this->texture=ref.texture;
    this->isTransparent=ref.isTransparent;
}

GameObject::~GameObject()
{

}

void GameObject::Update()
{
;
}

void GameObject::Render(QMatrix4x4* world)
{
    world->translate(position);
    world->scale(scale);
    world->rotate(rotation.normalized());

    if (shader != nullptr)
    {
        GameWindow::instance->BindCurrentShader(this);
        GameWindow::instance->setTransforms();
    }
}
