#include "jointtransform.h"

JointTransform::JointTransform(QVector3D position, QQuaternion rotation)
{
    this->position=position;
    this->rotation=rotation;
}

QMatrix4x4 JointTransform::GetLocalTransform()
{
    QMatrix4x4 matrix = QMatrix4x4();
    matrix.translate(position);
    return matrix * QMatrix4x4(rotation.toRotationMatrix());;
}

JointTransform JointTransform::Interpolate(JointTransform frameA, JointTransform frameB, float progression)
{
    QVector3D pos = Interpolate(frameA.position, frameB.position, progression);
    QQuaternion rot = QQuaternion::nlerp(frameA.rotation, frameB.rotation, progression);
    return JointTransform(pos,rot);
}

QVector3D JointTransform::Interpolate(QVector3D start, QVector3D end, float progression)
{
    float x = start.x() + (end.x() - start.x()) * progression;
    float y = start.y() + (end.y() - start.y()) * progression;
    float z = start.z() + (end.z() - start.z()) * progression;
    return QVector3D(x, y, z);
}
