#include "cube.h"

Cube::Cube(Model* model)
{
    this->model =model;
}

void Cube::Update()
{

}

void Cube::Render(QMatrix4x4* world)
{
    GameObject::Render(world);
    model->Draw();
}

Cube::~Cube()
{
    delete model;
}

