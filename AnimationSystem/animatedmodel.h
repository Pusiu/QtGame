#ifndef ANIMATEDMODEL_H
#define ANIMATEDMODEL_H


#include "debug.h"
#include "model.h"
#include "AnimationSystem/animator.h"

class AnimatedModel
{
public:

    Model model;
    Animator* animator;
    Joint* rootJoint;
    int jointCount;
    QList<Joint*> skeleton;

    AnimatedModel(Model m, Joint* rootJoint, int jointCount);

    void SetupBones();


    Joint* GetRootJoint();
    void PlayAnimation(Animation anim);
    void Update();
    QMatrix4x4* GetJointTransforms();
    void AddJointsToArray(Joint* headJoint, QMatrix4x4 jointMatrices[]);
};

#endif // ANIMATEDMODEL_H
