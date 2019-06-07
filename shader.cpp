#include "shader.h"

Shader::Shader(QString name, QString vertexPath, QString fragmentPath)
{
    this->name=name;
    this->vertexPath=vertexPath;
    this->fragmentPath=fragmentPath;
    program = new QOpenGLShaderProgram();
    Reload();
}

void Shader::Reload()
{
    program->bind();
    program->removeAllShaders();
    program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexPath);
    program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentPath);

    program->bindAttributeLocation("vertex", 0);
    program->bindAttributeLocation("normal", 1);
    program->bindAttributeLocation("uvCoord", 2);

    program->link();

    program->release();
}
