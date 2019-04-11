#ifndef ANIMATION_H
#define ANIMATION_H

#include <keyframe.h>
#include <QList>

class Animation
{
public:
    float length;
    QList<KeyFrame> keyFrames;


    Animation(float lengthInSeconds, QList<KeyFrame> frames);
    float GetLength();
    QList<KeyFrame> GetKeyFrames();
};

#endif // ANIMATION_H
