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
    textures["terrain"]=new QOpenGLTexture(QImage("resources/textures/TerrainMerged.png"));
    textures["trees"]=new QOpenGLTexture(QImage("resources/textures/tree.png"));
    textures["fences"]=new QOpenGLTexture(QImage("resources/textures/FencesMerged.png"));
    textures["hedges"]=new QOpenGLTexture(QImage("resources/textures/hedges.png"));
    textures["houses"]=new QOpenGLTexture(QImage("resources/textures/house_diffuse.tga"));
    textures["skytrain"]=new QOpenGLTexture(QImage("resources/textures/skytrain.png"));
    textures["parachute"]=new QOpenGLTexture(QImage("resources/textures/parachute.png"));
    textures["flak"]=new QOpenGLTexture(QImage("resources/textures/flak.png"));
    textures["explosion"]=new QOpenGLTexture(QImage("resources/textures/explosion.png"));
    textures["german"]=new QOpenGLTexture(QImage("resources/textures/german.png"));
}

QOpenGLTexture* TextureManager::GetTexture(QString name)
{
    QOpenGLTexture* t = textures[name];
    if (t == nullptr)
        qDebug("[TextureManager]Error! Can't find texture (texture not added at initialization?)");

    return t;
}
