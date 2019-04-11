#include "animation.h"


Animation::Animation(float lengthInSeconds, QList<KeyFrame> frames)
{
    this->length=lengthInSeconds;
    this->keyFrames=frames;
}

float Animation::GetLength()
{
    return length;
}

QList<KeyFrame> Animation::GetKeyFrames()
{
    return keyFrames;
}
