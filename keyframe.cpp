#include "keyframe.h"


KeyFrame::KeyFrame(float timeStamp, QMap<QString, JointTransform> jointKeyFrames)
{
    this->timeStamp=timeStamp;
    this->pose = jointKeyFrames;
}

float KeyFrame::GetTimeStamp()
{
    return timeStamp;
}

QMap<QString, JointTransform> KeyFrame::GetJointKeyFrames()
{
    return pose;
}
