#include "texturemanager.h"
#include "debug.h"
QMap<QString, QOpenGLTexture*> TextureManager::textures;

TextureManager::TextureManager()
{

}

void TextureManager::Init()
{
    textures["paratrooper"]=new QOpenGLTexture(QImage("resources/textures/paratrooper.png"));
    textures["skybox"]=new QOpenGLTexture(QImage("resources/textures/skybox_texture.jpg"));
    textures["terrain"]=new QOpenGLTexture(QImage("resources/textures/TerrainTexture.png"));
}

QOpenGLTexture* TextureManager::GetTexture(QString name)
{
    QOpenGLTexture* t = textures[name];
    if (t == nullptr)
        Debug::Log("[TextureManager]Error! Can't find texture (texture not added at initialization?)");

    return t;
}
