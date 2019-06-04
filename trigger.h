#ifndef TRIGGER_H
#define TRIGGER_H

#include "gameobject.h"
#include <QMap>

class Trigger : public GameObject
{
public:
    bool activated = false;
    bool triggerOnce = true;
    QString name;
    int id=-1;

    static int GetID();

    static QMap<QString, Trigger*> triggers;

    virtual bool IsPlayerInside() = 0;

    void TriggerWaypointGroup();
};

#endif // TRIGGER_H
