#ifndef BONE_H
#define BONE_H

#include <QString>
#include <QMatrix4x4>

class Bone
{
public:
    Bone();
    QString name;
    QMatrix4x4 offsetMatrix;
    QMatrix4x4 finalTransformation;
};

#endif // BONE_H
