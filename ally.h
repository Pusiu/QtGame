#ifndef ALLY_H
#define ALLY_H

#include "gameobject.h"
#include "AnimationSystem/animatedmodel.h"

class Ally : public GameObject
{
public:

    static void SpawnAllies();
    AnimatedModel* model;

    Ally();

    // GameObject interface
public:
    void Update();
    void Render(QMatrix4x4 *world);
};

#endif // ALLY_H
