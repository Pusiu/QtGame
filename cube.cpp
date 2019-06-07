#include "cube.h"

Cube::Cube(Model* model)
{
    this->model =model;
    this->shader=GameWindow::instance->shaders["simple"];
}

Cube::Cube(Cube &ref) : GameObject (ref)
{
    this->model=new Model(*ref.model);
}

void Cube::Update()
{

}

void Cube::Render(QMatrix4x4* world)
{
    GameObject::Render(world);
    model->Draw();
    shader->program->release();
    if (texture != nullptr)
        texture->release();
}

Cube::~Cube()
{
    delete model;
}

