#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H

#include <QSoundEffect>
#include <QString>
#include <QMap>

class AudioSource
{
public:
    AudioSource();

    static QMap<QString, QSoundEffect*> sounds;

    static void Init();
    static void PlaySoundOnce(QString name, float volume=0.5f, bool loop=false);
};

#endif // AUDIOSOURCE_H
