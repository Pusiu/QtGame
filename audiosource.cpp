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
    sounds.insert("skytrain",new QSoundEffect);
    sounds["skytrain"]->setSource(QUrl::fromLocalFile("resources/sounds/skytrain.wav"));
    sounds.insert("flak",new QSoundEffect);
    sounds["flak"]->setSource(QUrl::fromLocalFile("resources/sounds/flak.wav"));
    sounds.insert("death",new QSoundEffect);
    sounds["death"]->setSource(QUrl::fromLocalFile("resources/sounds/death.wav"));
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
    else {
        qDebug("Can't play audio file");
    }
}
