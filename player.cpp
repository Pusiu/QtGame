#include "player.h"
#include "AnimationSystem/animatedmodel.h"

Player::Player(QString modelPath) : GameObject ()
{
    model = new AnimatedModel(Model(modelPath), nullptr, -1);

    position = QVector3D(0, 0, 0);
    direction = QVector3D(0, 0, -1);
    speed = 0.01f;
}

void Player::Update()
{

}

void Player::Render()
{
    model->model.Draw();
}

Player::~Player()
{

}
