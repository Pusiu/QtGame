#include "enemy.h"
#include "AnimationSystem/animatedmodel.h"


int Enemy::GetId()
{
    static int lastId=-1;
    lastId++;
    return lastId;
}

Enemy *Enemy::GetEnemyByID(int id)
{
    for (int i=0; i <GameWindow::instance->enemies.size();i++)
    {
        if (GameWindow::instance->enemies[i]->id==id)
        {
            return GameWindow::instance->enemies[i];
        }
    }
    return nullptr;
}

Enemy::Enemy(QString modelPath)
{
    model=new AnimatedModel(modelPath);
    this->id=GetId();
}

Enemy::Enemy(Enemy &ref)
{
    this->model=new AnimatedModel(*ref.model);
    this->shader=ref.shader;
    this->position=ref.position;
    this->scale=ref.scale;
    this->rotation=ref.rotation;
    this->texture=ref.texture;
    this->id=GetId();
}

void Enemy::Shoot()
{
    shootMod=(rand() % 5) * 500;
    lastShootTime=GameWindow::instance->timerSinceStart.elapsed();
    AudioSource::PlaySoundOnce("mauser", 0.1f);
    QVector3D right = rotation*QVector3D(0,0,1);
    QVector3D dir = QVector3D::crossProduct(right, QVector3D(0,1,0));
    dir.setY(0);
    QVector3D effectPos=position+(right*0.2f)+QVector3D(0,1.95f,0)-(dir*1.5f);

    ParticleEffect::SpawnEffect(effectPos,ParticleEffect::Gunshot);

    GameWindow::instance->player->ReceiveDamage(20.0f);
}

void Enemy::AssignWaypoint(Waypoint *wp)
{
    if (isAlive)
    {
        currentWaypoint=wp;
        if (currentWaypoint != nullptr)
        {
            currentWaypoint->Execute(this);
        }
        else
        {
            desiredAnimationState=Idle;
        }
    }
}

void Enemy::PlayAnimation(QString animName, bool waitForEnd)
{
    model->SetCurrentAnimation(animName, !waitForEnd);
    currentAnimationState=Other;
    desiredAnimationState=Other;
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
        QVector3D dir = GameWindow::instance->player->position-position;
        if (currentAnimationState == Aiming)
        {
            rotation=QQuaternion::fromEulerAngles(0,-atan2(dir.z(), dir.x()) * 180.0f / M_PI,0);
        }

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
                //rotation=QQuaternion::fromEulerAngles(0,-atan2(dir.z(), dir.x()) * 180.0f / M_PI,0);
                break;
            case Crouching:
                rotation=QQuaternion::fromEulerAngles(0,-atan2(dir.z(), dir.x()) * 180.0f / M_PI,0);
                model->SetCurrentAnimation("Crouch", false);
                break;
            case AimToCrouch:
                model->SetCurrentAnimation("AimToCrouch", false);
                break;
            case CrouchToAim:
                model->SetCurrentAnimation("CrouchToAim", false);
                break;
            case Reload:
                model->SetCurrentAnimation("Reload", false);
                break;
            }
        }

        if (currentWaypoint != nullptr)
        {
            long currentTime = GameWindow::instance->timerSinceStart.elapsed();
            long diff = currentTime-lastShootTime;
            switch (currentWaypoint->type)
            {
                case Waypoint::GoTo:
                    if (position.distanceToPoint(currentWaypoint->position) < 1)
                    {
                        AssignWaypoint(currentWaypoint->nextWaypoint);
                    }
                    else
                    {
                        QVector3D dir = currentWaypoint->position - position;
                        dir.normalize();
                        position += dir*speed;
                    }
                    break;
               case Waypoint::Crouch:
                    if (diff > 8000+shootMod)
                        Shoot();
                    else if (diff > 6500)
                        desiredAnimationState=Aiming;
                    else if (diff > 6000)
                        desiredAnimationState=CrouchToAim;
                    else if (diff > 2000)
                        desiredAnimationState=Crouching;
                    else if (diff > 1500)
                        desiredAnimationState=AimToCrouch;
                    else if (diff > 0)
                        desiredAnimationState=Reload;

                    break;
               case Waypoint::Shoot:
                    if (diff > 3000+shootMod)
                        Shoot();
                    else if (diff > 1500)
                        desiredAnimationState=Aiming;
                    else if (diff > 0)
                        desiredAnimationState=Reload;
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
