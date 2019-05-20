#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <QMap>
#include <QString>
#include <QOpenGLTexture>

class TextureManager
{
public:
    TextureManager();

    static void Init();
    static QMap<QString, QOpenGLTexture*> textures;
    static QOpenGLTexture* GetTexture(QString name);
};

#endif // TEXTUREMANAGER_H
