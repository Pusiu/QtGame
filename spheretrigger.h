#ifndef SPHERETRIGGER_H
#define SPHERETRIGGER_H

#include "trigger.h"
#include "gamewindow.h"

class SphereTrigger : public Trigger
{
public:
    float size;
    SphereTrigger(QString name, QVector3D position, float size, bool triggerOnce=true);

    void Update() override;

    bool IsPlayerInside() override;
};

#endif // SPHERETRIGGER_H
