#ifndef JOINT_H
#define JOINT_H

#include <QList>
#include <QString>
#include <QMatrix4x4>

class Joint
{
public:
    int index;
    QString name;
    QList<Joint*> children;

    QMatrix4x4 animatedTransform; //model space transform, loaded to shader

    QMatrix4x4 localBindTransform; //original transform in relation to parent
    QMatrix4x4 inverseBindTransform; //same, in relation to model space


    Joint(int index, QString name, QMatrix4x4 bindLocalTransform);

    void AddChild(Joint* child);
    QMatrix4x4 GetAnimatedTransform();
    void SetAnimationTransform(QMatrix4x4 animatedTransform);
    QMatrix4x4 GetInverseBindTransform();

    void CalculateInverseBindTransform(QMatrix4x4 parentBindTransform);
};

#endif // JOINT_H
