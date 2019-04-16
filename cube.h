#ifndef CUBE_H
#define CUBE_H

#include "gameobject.h"
#include "model.h"

class Cube : public GameObject
{
public:
    Cube(Model* model);
    Model* model;

    void Update() override;
    void Render() override;
    ~Cube() override;
};

#endif // CUBE_H
