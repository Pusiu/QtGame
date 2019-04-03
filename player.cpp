#include "player.h"

Player::Player()
{
    position = QVector3D(0, 0, 0);
    direction = QVector3D(0, 0, -1);
    speed = 0.01f;
}
