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
    virtual void Render(QMatrix4x4* world) override;
    ~Player() override;
};

#endif // PLAYER_H
