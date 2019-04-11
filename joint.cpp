#include "joint.h"


Joint::Joint(int index, QString name, QMatrix4x4 bindLocalTransform)
{
    this->index=index;
    this->name=name;
    this->localBindTransform = bindLocalTransform;
}

void Joint::AddChild(Joint* child)
{
 this->children.append(child);
}

QMatrix4x4 Joint::GetAnimatedTransform()
{
    return animatedTransform;
}

void Joint::SetAnimationTransform(QMatrix4x4 animatedTransform)
{
    this->animatedTransform=animatedTransform;
}

QMatrix4x4 Joint::GetInverseBindTransform()
{
    return inverseBindTransform;
}

void Joint::CalculateInverseBindTransform(QMatrix4x4 parentBindTransform)
{
    QMatrix4x4 bindTransform = parentBindTransform*localBindTransform;
    inverseBindTransform=bindTransform.inverted();

    for (Joint* child : children)
        child->CalculateInverseBindTransform(bindTransform);
}
