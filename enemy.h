#ifndef ENEMY_H
#define ENEMY_H

#include "gameobject.h"
#include "waypoint.h"

QT_FORWARD_DECLARE_CLASS(AnimatedModel)

class Enemy : public GameObject
{
public:
    static int GetId();

    static Enemy* GetEnemyByID(int id);

    Enemy(QString modelPath);
    Enemy(Enemy& ref);

    Waypoint* currentWaypoint=nullptr;

    enum AnimState {Idle, Running, Aiming, Crouching, AimToCrouch,CrouchToAim,Reload, Other};
    AnimState currentAnimationState = Aiming;
    AnimState desiredAnimationState = Idle;

    long lastShootTime = 0;
    long shootMod=0;

    AnimatedModel* model;
    int id;
    int hp=100;
    float speed=0.05f;
    bool isAlive=true;

    void Shoot();
    void AssignWaypoint(Waypoint* wp);
    void PlayAnimation(QString animName, bool waitForEnd=true);
    void ReceiveDamage();
    void Update();
    void Render(QMatrix4x4 *world);
};

#endif // ENEMY_H

