#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QVector3D>
#include <QQuaternion>

class GameObject
{
public:

    QVector3D position;
    QQuaternion rotation;
    QVector3D scale;


    GameObject();
    virtual ~GameObject();

    virtual void Update() = 0;
    virtual void Render() = 0;
};

#endif // GAMEOBJECT_H
