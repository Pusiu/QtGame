#ifndef PLAYER_H
#define PLAYER_H

#include <gameobject.h>


class AnimatedModel;

class Player : public GameObject
{
public:

    AnimatedModel* model;

    enum AnimState {Idle, Running, Aiming, Other};
    AnimState currentAnimationState = Idle;
    AnimState desiredAnimationState = Idle;

    Player(QString modelPath);
    QVector3D direction;
    float speed;

    bool isMoving=false;
    bool canMove=true;
    int currentAmmo = 20;
    int maxAmmo=20;

    void PlayAnimation(QString animName, bool waitForEnd=true);
    void Shoot();
    void Reload();

    unsigned long lastShotTime = 0;

    void Update() override;
    virtual void Render(QMatrix4x4* world) override;
    ~Player() override;
};

#endif // PLAYER_H
