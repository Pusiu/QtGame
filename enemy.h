#ifndef ENEMY_H
#define ENEMY_H

#include "gameobject.h"

QT_FORWARD_DECLARE_CLASS(AnimatedModel)

class Enemy : public GameObject
{
public:
    Enemy(QString modelPath);
    Enemy(Enemy& ref);

    enum AnimState {Idle, Running, Aiming, Other};
    AnimState currentAnimationState = Aiming;
    AnimState desiredAnimationState = Idle;

    AnimatedModel* model;
    int hp=100;
    bool isAlive=true;

    void ReceiveDamage();
    void Update();
    void Render(QMatrix4x4 *world);
};

#endif // ENEMY_H

