#include "audiosource.h"

QMap<QString, QSoundEffect*> AudioSource::sounds;

AudioSource::AudioSource()
{

}

void AudioSource::Init()
{
    sounds.insert("Carbine",new QSoundEffect);
    sounds["Carbine"]->setSource(QUrl::fromLocalFile("resources/sounds/carbine.wav"));
    sounds.insert("CarbineReload",new QSoundEffect);
    sounds["CarbineReload"]->setSource(QUrl::fromLocalFile("resources/sounds/carbineReload.wav"));
}

void AudioSource::PlaySoundOnce(QString name, float volume)
{
    QSoundEffect* s = sounds[name];
    if (s != nullptr)
    {
        s->setLoopCount(1);
        s->setVolume(volume);
        s->play();
    }
}
