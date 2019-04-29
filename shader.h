#ifndef SHADER_H
#define SHADER_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

class Shader
{
public:

    QOpenGLShaderProgram* program;
    QString name;

    QString vertexPath;
    QString fragmentPath;

    Shader(QString name, QString vertexPath, QString fragmentPath);
    void Reload();
};

#endif // SHADER_H
