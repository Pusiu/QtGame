#include "animator.h"
#include "animatedmodel.h" //to get members of AnimatedModel
#include "gamewindow.h"

Animator::Animator(AnimatedModel *model)
{
    this->entity=model;
}

void Animator::PlayAnimation(Animation* anim)
{
    this->animationTime=0;
    this->currentAnimation=anim;
}

void Animator::Update()
{
    if (currentAnimation == nullptr)
        return;

    IncreaseAnimationTime();
    QMap<QString, QMatrix4x4> currentPose = CalculateCurrentAnimationPose();
    ApplyPoseToJoints(currentPose, entity->GetRootJoint(), QMatrix4x4());
}

void Animator::IncreaseAnimationTime()
{
    animationTime+=GameWindow::instance->m_timer;
    if (animationTime > currentAnimation->GetLength()) {
        this->animationTime = fmod(this->animationTime,currentAnimation->GetLength()); //float modulo
    }
}

QMap<QString, QMatrix4x4> Animator::CalculateCurrentAnimationPose()
{
    QList<KeyFrame> frames = GetPreviousAndNextFrames();
    float progression = CalculateProgression(frames[0], frames[1]);
    return InterpolatePoses(frames[0],frames[1],progression);
}

void Animator::ApplyPoseToJoints(QMap<QString, QMatrix4x4> currentPose, Joint *joint, QMatrix4x4 parentTransform)
{
    QMatrix4x4 currentLocalTransform = currentPose.find(joint->name).value();
    QMatrix4x4 currentTransform = parentTransform*currentLocalTransform;
    for (Joint* childJoint : joint->children) {
        ApplyPoseToJoints(currentPose, childJoint, currentTransform);
    }
    currentTransform = currentTransform * joint->GetInverseBindTransform();
    joint->SetAnimationTransform(currentTransform);
}

QList<KeyFrame> Animator::GetPreviousAndNextFrames()
{
    QList<KeyFrame> allFrames = currentAnimation->GetKeyFrames();
    KeyFrame previousFrame = allFrames[0];
    KeyFrame nextFrame = allFrames[0];
    for (int i = 1; i < allFrames.length(); i++) {
        nextFrame = allFrames[i];
        if (nextFrame.GetTimeStamp() > animationTime) {
            break;
        }
        previousFrame = allFrames[i];
    }
    return QList<KeyFrame> { previousFrame, nextFrame };
}

float Animator::CalculateProgression(KeyFrame previousFrame, KeyFrame nextFrame)
{
    float totalTime = nextFrame.GetTimeStamp() - previousFrame.GetTimeStamp();
    float currentTime = animationTime - previousFrame.GetTimeStamp();
    return currentTime / totalTime;
}

QMap<QString, QMatrix4x4> Animator::InterpolatePoses(KeyFrame previousFrame, KeyFrame nextFrame, float progression)
{
    QHash<QString, QMatrix4x4> currentPose = QHash<QString, QMatrix4x4>();
    for (QString jointName : previousFrame.GetJointKeyFrames().keys()) {
        JointTransform previousTransform = previousFrame.GetJointKeyFrames().find(jointName).value();
        JointTransform nextTransform = nextFrame.GetJointKeyFrames().find(jointName).value();
        JointTransform currentTransform = JointTransform::Interpolate(previousTransform, nextTransform, progression);
        currentPose.insert(jointName, currentTransform.GetLocalTransform());
    }

    QMap<QString, QMatrix4x4> r = QMap<QString, QMatrix4x4>();
    QHashIterator<QString, QMatrix4x4> it(currentPose);
    while (it.hasNext())
    {
        it.next();
        r.insert(it.key(), it.value());
    }

    return r;
}


