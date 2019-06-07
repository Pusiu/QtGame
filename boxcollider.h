#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

#include <QVector3D>
#include <QQuaternion>
#include <utils.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

class BoxCollider
{
public:
    BoxCollider();

    static QList<BoxCollider*> allColliders;
    static void LoadColliders(QString path);

    QString name;
    QVector3D position;
    QVector3D scale;
    QQuaternion rotation;

    BoxCollider(QVector3D position, QQuaternion rotation, QVector3D scale);

    bool IsPointInside(QVector3D point);
};

#endif // BOXCOLLIDER_H
