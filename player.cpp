#include "player.h"
#include "audiosource.h"
#include "AnimationSystem/animatedmodel.h"

Player::Player(QString modelPath) : GameObject ()
{
    model = new AnimatedModel(modelPath);

    position = QVector3D(0, 0, 0);
    direction = QVector3D(0, 0, -1);
    speed = 0.1f;
}

void Player::PlayAnimation(QString animName, bool waitForEnd)
{
    model->SetCurrentAnimation(animName, !waitForEnd);
    currentAnimationState=Other;
}

void Player::Shoot()
{
    if (GameWindow::instance->timerSinceStart.elapsed() - lastShotTime >= 300)
    {
        if (currentAmmo == 0)
            return;

        currentAmmo--;
        AudioSource::PlaySoundOnce("Carbine");
        QVector3D right = rotation*QVector3D(0,0,1);
        QVector3D dir = QVector3D::crossProduct(right, QVector3D(0,1,0));
        dir.setY(0);
        QVector3D effectPos=position+(right*0.5f)+QVector3D(0,1.95f,0)-(dir*1.5f);

        ParticleEffect::SpawnEffect(effectPos,ParticleEffect::Gunshot);

        lastShotTime = GameWindow::instance->timerSinceStart.elapsed();
        for (int i=0; i < GameWindow::instance->enemies.size(); i++)
        {
            Enemy* en = GameWindow::instance->enemies[i];
            if (!en->isAlive)
                continue;

            QVector3D dir = (en->position-position).normalized();
            float dot = QVector3D::dotProduct(GameWindow::instance->playerDirection.normalized(), dir);
            //qDebug(QString::number(dot).toStdString().data());
            if (dot >= 0.98f)
            {
                en->ReceiveDamage();
                break;
            }
        }
    }
}

void Player::Reload()
{
    AudioSource::PlaySoundOnce("CarbineReload");
    PlayAnimation("Reload", true);
}

void Player::Update()
{
    if ((model->currentAnimationEnded && currentAnimationState == Other)
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

    model->Update();
}

void Player::Render(QMatrix4x4* world)
{
    GameObject::Render(world);
    model->Draw(this->shader);
}

Player::~Player()
{

}
