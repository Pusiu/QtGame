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
        AudioSource::PlaySoundOnce("Carbine");
        lastShotTime = GameWindow::instance->timerSinceStart.elapsed();
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
            model->SetCurrentAnimation("Idle", false);
            break;
        case Running:
            model->SetCurrentAnimation("Run", false);
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
