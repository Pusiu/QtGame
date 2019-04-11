#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "animation.h"
#include "keyframe.h"
#include "joint.h"
#include <QMatrix4x4>
#include <QMap>

class AnimatedModel; //forward declaration

class Animator
{
public:

    AnimatedModel* entity;
    Animation* currentAnimation;
    float animationTime = 0;

    Animator(AnimatedModel* model);
    void PlayAnimation(Animation* anim);
    void Update();
    void IncreaseAnimationTime();
    QMap<QString, QMatrix4x4> CalculateCurrentAnimationPose();
    void ApplyPoseToJoints(QMap<QString, QMatrix4x4> currentPose, Joint* joint, QMatrix4x4 parentTransform);
    QList<KeyFrame> GetPreviousAndNextFrames();
    float CalculateProgression(KeyFrame previousFrame, KeyFrame nextFrame);

    QMap<QString, QMatrix4x4> InterpolatePoses(KeyFrame previousFrame, KeyFrame nextFrame, float progression);
};

#endif // ANIMATOR_H
