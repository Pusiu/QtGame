#include "waypoint.h"
#include "enemy.h"

QList<Waypoint*> Waypoint::allWaypoints;

void Waypoint::SetupWaypoints()
{
    Waypoint* g1 = new Waypoint(QVector3D(-23,0,17), WaypointType::GoTo);
    g1->name="ger1";
    g1=g1->AddWaypoint(new Waypoint(QVector3D(-23,0,17), WaypointType::Crouch));

    Waypoint* g2 = new Waypoint(QVector3D(-26.5f,0,16), WaypointType::GoTo);
    g2->name="ger2";
    g2=g2->AddWaypoint(new Waypoint(QVector3D(-26.5f,0,16), WaypointType::Crouch));

    Waypoint* g3 = new Waypoint(QVector3D(-12,0,12), WaypointType::GoTo);
    g3->name="ger3";
    g3=g3->AddWaypoint(new Waypoint(QVector3D(-12,0,12), WaypointType::Shoot));
}

int Waypoint::GetID()
{
    static int curID=-1;
    curID++;
    return curID;
}

Waypoint *Waypoint::GetWaypointByName(QString name)
{
    for (int i=0; i < allWaypoints.size(); i++)
    {
        if (allWaypoints[i]->name.contains(name, Qt::CaseInsensitive))
        {
            return allWaypoints[i];
        }
    }
    return nullptr;
}

Waypoint::Waypoint(QVector3D position, Waypoint::WaypointType type)
{
    this->position=position;
    this->type=type;
    this->id=GetID();
    allWaypoints.push_back(this);
}

void Waypoint::Execute(Enemy *owner)
{
    QVector3D dir=position-owner->position;
    switch (type)
    {
    case Waypoint::GoTo:
        owner->desiredAnimationState=Enemy::Running;
        owner->rotation=QQuaternion::fromEulerAngles(0,-atan2(dir.z(), dir.x()) * 180.0f / M_PI,0);
        break;
    case Waypoint::Shoot:
        break;
    case Waypoint::Crouch:
        owner->desiredAnimationState=Enemy::Crouching;
        break;

    }
}

Waypoint *Waypoint::AddWaypoint(Waypoint *wp)
{
    this->nextWaypoint=wp;
    return nextWaypoint;
}
