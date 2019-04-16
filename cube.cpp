#include "cube.h"

Cube::Cube(Model* model)
{
    this->model =model;
}

void Cube::Update()
{

}

void Cube::Render()
{
    model->Draw();
}

Cube::~Cube()
{
    delete model;
}

