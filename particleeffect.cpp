#include "particleeffect.h"
#include "cube.h"

QMap<Cube*, unsigned long> ParticleEffect::activeParticles;

ParticleEffect::ParticleEffect()
{

}

void ParticleEffect::SpawnEffect(QVector3D position, ParticleEffect::ParticleType type)
{
    Cube* go = new Cube(new Model("resources/meshes/plane.fbx"));
    go->position = position;
    go->shader=GameWindow::instance->shaders["simple"];
    float time=0;
    switch (type)
    {
        case FlakShot:
            go->texture=TextureManager::GetTexture("explosion");
            go->scale=QVector3D(3,3,3);
            time=GameWindow::instance->timerSinceStart.elapsed()+100;
            break;
        case Gunshot:
            go->texture=TextureManager::GetTexture("explosion");
            go->scale=QVector3D(0.2f,0.2f,0.2f);
            go->rotation=QQuaternion::fromDirection(-GameWindow::instance->playerDirection, QVector3D(0,1,0));
            time=GameWindow::instance->timerSinceStart.elapsed()+100;
            break;
    }
    go->isTransparent=true;
    activeParticles.insert(go,time);
    GameWindow::instance->gameObjects.push_back(go);
}

void ParticleEffect::Render(QMatrix4x4* world)
{
    for (int i=0;i<activeParticles.size(); i++)
    {
        activeParticles.keys()[i]->Render(world);
    }
}

void ParticleEffect::Update()
{
    for (int i=0;i<activeParticles.size(); i++)
    {
        Cube* go = activeParticles.keys()[i];
        QVector3D r = -GameWindow::instance->playerDirection.normalized();
        go->rotation=QQuaternion::fromDirection(r, QVector3D(0,1,0));
        if (activeParticles.value(go) <= GameWindow::instance->timerSinceStart.elapsed())
        {
            GameWindow::instance->gameObjects.removeOne(go);
            activeParticles.remove(go);
            delete  go;
        }
    }
}
