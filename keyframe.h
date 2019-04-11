#ifndef KEYFRAME_H
#define KEYFRAME_H

#include "jointtransform.h"
#include <QMap>
#include <QString>

class KeyFrame
{
public:
    float timeStamp;
    QMap<QString, JointTransform> pose;

    KeyFrame(float timeStamp, QMap<QString, JointTransform> jointKeyFrames);
    float GetTimeStamp();
    QMap<QString, JointTransform> GetJointKeyFrames();
};

#endif // KEYFRAME_H
