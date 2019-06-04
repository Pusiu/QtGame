#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <QString>
#include <QVector3D>

QT_FORWARD_DECLARE_CLASS(Enemy)
class Waypoint
{
public:

    static QList<Waypoint*> allWaypoints;

    static void SetupWaypoints();
    static int GetID();
    static Waypoint* GetWaypointByName(QString name);

    enum WaypointType {GoTo, Shoot, Crouch};
    WaypointType type;

    int id;
    QString name = "";
    QVector3D position;
    Waypoint* nextWaypoint = nullptr;

    Waypoint(QVector3D position, WaypointType type=GoTo);
    void Execute(Enemy* owner);
    Waypoint* AddWaypoint(Waypoint* wp);
};

#endif // WAYPOINT_H
