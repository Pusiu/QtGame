#include "player.h"
#include "AnimationSystem/animatedmodel.h"

Player::Player(QString modelPath) : GameObject ()
{
    model = new AnimatedModel(modelPath);



    position = QVector3D(0, 0, 0);
    direction = QVector3D(0, 0, -1);
    speed = 0.1f;
}

void Player::Update()
{
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
