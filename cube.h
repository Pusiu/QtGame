#ifndef CUBE_H
#define CUBE_H

#include "gameobject.h"
#include "model.h"

class Cube : public GameObject
{
public:
    Cube(Model* model);
    Cube(Cube& ref);
    Model* model;

    void Update() override;
    void Render(QMatrix4x4* world) override;
    ~Cube() override;
};

#endif // CUBE_H
