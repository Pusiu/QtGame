#include "cube.h"

Cube::Cube(Model* model)
{
    this->model =model;
}

Cube::Cube(Cube &ref)
{
    this->model=new Model(*ref.model);
    this->position=ref.position;
    this->rotation=ref.rotation;
    this->scale=ref.scale;
    this->shader=ref.shader;
    this->texture=ref.texture;
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

