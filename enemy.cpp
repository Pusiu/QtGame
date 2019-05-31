#include "enemy.h"
#include "AnimationSystem/animatedmodel.h"


Enemy::Enemy(QString modelPath)
{
    model=new AnimatedModel(modelPath);
}

Enemy::Enemy(Enemy &ref)
{
    this->model=new AnimatedModel(*ref.model);
    this->shader=ref.shader;
    this->position=ref.position;
    this->scale=ref.scale;
    this->rotation=ref.rotation;
    this->texture=ref.texture;
}

void Enemy::ReceiveDamage()
{
    hp-=40;

    if (hp <= 0)
    {
        float vol=1-((GameWindow::instance->player->position.distanceToPoint(position))/50.0f);
        if (vol < 0)
            vol=0;

        AudioSource::PlaySoundOnce("death", vol*0.4f);
        hp=0;
        isAlive=false;
        int r = rand() % 3;
        switch (r)
        {
        case 0:
            model->SetCurrentAnimation("Die", false);
            break;
        case 1:
            model->SetCurrentAnimation("Die2", false);
            break;
        case 2:
            model->SetCurrentAnimation("Die3", false);
            break;
        }
        currentAnimationState=Other;
        desiredAnimationState=Other;
    }
}

void Enemy::Update()
{
    if (isAlive)
    {
        if (    (model->currentAnimationEnded && currentAnimationState == Other)
                ||
                (currentAnimationState != desiredAnimationState && currentAnimationState != Other)
           )
        {
            currentAnimationState=desiredAnimationState;
            switch (currentAnimationState)
            {
            case Idle:
                model->SetCurrentAnimation("Idle", true);
                break;
            case Running:
                model->SetCurrentAnimation("Run", true);
                break;
            case Aiming:
                model->SetCurrentAnimation("Aim", false);
                break;
            }
        }
    }

    model->Update();
}

void Enemy::Render(QMatrix4x4 *world)
{
    GameObject::Render(world);

    model->Draw(this->shader);
}
