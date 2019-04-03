#ifndef PLAYER_H
#define PLAYER_H
#include <QVector3D>

class Player
{
public:
    Player();
    QVector3D position;
    QVector3D direction;
    float speed;
};

#endif // PLAYER_H
