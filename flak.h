#ifndef FLAK_H
#define FLAK_H

#include "gameobject.h"
#include "particleeffect.h"

class AnimatedModel;

class Flak : public GameObject
{
public:
    Flak(QString modelPath);


    // GameObject interface
public:
    unsigned int lastFireTime = 0;
    AnimatedModel* model;
    bool isOperated=true;
    bool isDestroyed=false;
    void Fire();

    void Update();
    void Render(QMatrix4x4 *world);
};

#endif // FLAK_H
