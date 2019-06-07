#include "spheretrigger.h"

QMap<QString, Trigger*> Trigger::triggers;

SphereTrigger::SphereTrigger(QString name, QVector3D position, float size, bool triggerOnce)
{
    this->name=name;
    this->position=position;
    this->size=size;
    this->triggerOnce=triggerOnce;
    this->id=GetID();
    triggers.insert(name, this);
}

void SphereTrigger::Update()
{
    if (IsPlayerInside())
    {
        if ((!activated && triggerOnce) || !triggerOnce)
        {
            Activate();
        }
    }
    else
    {
        if (!triggerOnce)
            this->activated=false;
    }
}

bool SphereTrigger::IsPlayerInside()
{
    return position.distanceToPoint(GameWindow::instance->player->position) < size;
}

void SphereTrigger::Activate()
{
    this->activated=true;
    qDebug("Trigger activated");
    TriggerWaypointGroup();

    if (triggerOnce)
    {

        triggers.remove(name);
        delete this;
    }
}
