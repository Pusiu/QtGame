#ifndef PLAYER_H
#define PLAYER_H

#include <gameobject.h>

class AnimatedModel;

class Player : public GameObject
{
public:

    AnimatedModel* model;

    Player(QString modelPath);
    QVector3D direction;
    float speed;

    void Update() override;
    void Render() override;
    ~Player() override;
};

#endif // PLAYER_H
