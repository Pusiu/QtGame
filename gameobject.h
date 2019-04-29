#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QVector3D>
#include <QQuaternion>
#include <shader.h>

class GameObject
{
public:

    QVector3D position;
    QQuaternion rotation;
    QVector3D scale;

    Shader* shader;


    GameObject();
    virtual ~GameObject();

    virtual void Update() = 0;
    virtual void Render(QMatrix4x4* world);
};

#endif // GAMEOBJECT_H
