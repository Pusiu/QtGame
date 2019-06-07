#ifndef HUDElement_H
#define HUDElement_H

#include "gameobject.h"

QT_FORWARD_DECLARE_CLASS(Model)
class HUDElement : public GameObject
{
public:
    HUDElement(float startAlpha = 0.0f);

    struct Rectangle
    {
     float xPos;
     float yPos;
     float width;
     float height;
    };

    float desiredAlpha = 0.0f;
    float transitionTime = 2.0f; //in seconds
    long startTime;
    Model* m;

    void StartTransition(float timeOffset=0);

    void Update() override;

    void Render(QMatrix4x4 *world) override;
    float alpha = 1.0f;

};

#endif // HUDElement_H
