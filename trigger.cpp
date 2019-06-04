#include "trigger.h"
#include "enemy.h"
#include "gamewindow.h"

int Trigger::GetID()
{
    static int curID=-1;
    curID++;
    return curID;
}

void Trigger::TriggerWaypointGroup()
{
    switch (id)
    {
        case 0: //farm field trigger
        Enemy::GetEnemyByID(0)->AssignWaypoint(Waypoint::GetWaypointByName("ger1"));
        Enemy::GetEnemyByID(1)->AssignWaypoint(Waypoint::GetWaypointByName("ger2"));
        Enemy::GetEnemyByID(2)->AssignWaypoint(Waypoint::GetWaypointByName("ger3"));
        GameWindow::instance->flak->isOperated=false;
        break;
    }
}
