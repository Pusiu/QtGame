#include "ally.h"

void Ally::SpawnAllies()
{
    Ally* a = new Ally();
    a->position=QVector3D(64,0,-2);
    a->rotation=QQuaternion::fromEulerAngles(0,90,0);

    a = new Ally();
    a->position=QVector3D(67,0,-5);
    a->rotation=QQuaternion::fromEulerAngles(0,90,0);

    a = new Ally();
    a->position=QVector3D(60,0,-4);
    a->rotation=QQuaternion::fromEulerAngles(0,90,0);
}

Ally::Ally()
{
    texture=TextureManager::GetTexture("paratrooper");
    scale = QVector3D(0.0005,0.0005,0.0005);
    shader=GameWindow::instance->shaders["skinned"];
    model=new AnimatedModel("resources/meshes/paratrooper.fbx");
    model->SetCurrentAnimation("Idle", true);
    GameWindow::instance->gameObjects.push_back(this);
}

void Ally::Update()
{
    model->Update();
}

void Ally::Render(QMatrix4x4 *world)
{
    GameObject::Render(world);
    model->Draw(this->shader);
}
