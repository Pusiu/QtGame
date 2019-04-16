#ifndef JOINTTRANSFORM_H
#define JOINTTRANSFORM_H

#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>

class JointTransform
{
public:
    QVector3D position;
    QQuaternion rotation;

    JointTransform(QVector3D position, QQuaternion rotation);
    QMatrix4x4 GetLocalTransform();
    static JointTransform Interpolate(JointTransform frameA, JointTransform frameB, float progression);
    static QVector3D Interpolate(QVector3D start, QVector3D end, float progression);
};

#endif // JOINTTRANSFORM_H
