#include "animatedmodel.h"

AnimatedModel::AnimatedModel(Model m, Joint* rootJoint, int jointCount)
{
    model = m;
    this->rootJoint=rootJoint;
    this->jointCount=jointCount;
    rootJoint->CalculateInverseBindTransform(QMatrix4x4());
}

Joint* AnimatedModel::GetRootJoint()
{
    return rootJoint;
}

void AnimatedModel::PlayAnimation(Animation anim)
{
    animator.PlayAnimation(anim);
}

void AnimatedModel::Update()
{
    animator.Update();
}

QMatrix4x4* AnimatedModel::GetJointTransforms()
{
    QMatrix4x4 *jointMatrices = new QMatrix4x4[jointCount];
    AddJointsToArray(rootJoint, jointMatrices);
    return jointMatrices;
}

void AnimatedModel::AddJointsToArray(Joint *headJoint, QMatrix4x4 jointMatrices[])
{
    jointMatrices[headJoint->index] = headJoint->GetAnimatedTransform();
    for (Joint* childJoint : headJoint->children)
        AddJointsToArray(childJoint, jointMatrices);
}
