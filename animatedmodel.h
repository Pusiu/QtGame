#ifndef ANIMATEDMODEL_H
#define ANIMATEDMODEL_H

#include "model.h"
#include "joint.h"
#include "animation.h"
#include "animator.h"

class AnimatedModel
{
public:

    Model model;
    Joint* rootJoint;
    int jointCount;

    AnimatedModel(Model m, Joint* rootJoint, int jointCount);
    Joint* GetRootJoint();
    void PlayAnimation(Animation anim);
    void Update();
    QMatrix4x4* GetJointTransforms();
    void AddJointsToArray(Joint* headJoint, QMatrix4x4 jointMatrices[]);
};

#endif // ANIMATEDMODEL_H
