#include "flak.h"
#include "AnimationSystem/animatedmodel.h"

Flak::Flak(QString modelPath)
{
    model=new AnimatedModel(modelPath);
}

void Flak::Fire()
{
    lastFireTime=GameWindow::instance->timerSinceStart.elapsed();
    model->SetCurrentAnimation("Shoot", false);
    float distance = position.distanceToPoint(GameWindow::instance->player->position);
    float volume = 1-(distance/60);
    if (volume < 0)
        volume=0;

    AudioSource::PlaySoundOnce("flak", 0.1f+(volume*0.5f));
    ParticleEffect::SpawnEffect(position+QVector3D(5,5.5,1), ParticleEffect::FlakShot);
}

void Flak::Update()
{
    if (model->currentAnimationEnded)
    {
        model->SetCurrentAnimation("Idle");
    }
    if (GameWindow::instance->timerSinceStart.elapsed() - lastFireTime > 9000 && isOperated && !isDestroyed)
    {
        Fire();
    }

    model->Update();
}

void Flak::Render(QMatrix4x4 *world)
{
    GameObject::Render(world);
    model->Draw(this->shader);
}
