#include "texturemanager.h"
QMap<QString, QOpenGLTexture*> TextureManager::textures;

TextureManager::TextureManager()
{

}

void TextureManager::Init()
{
    textures["paratrooper"]=new QOpenGLTexture(QImage("resources/textures/paratrooper.png"));
}

QOpenGLTexture* TextureManager::GetTexture(QString name)
{
    return textures[name];
}
