#ifndef PARTICLEEFFECT_H
#define PARTICLEEFFECT_H

#include <QVector3D>
#include <QMap>

QT_FORWARD_DECLARE_CLASS(Cube)

class ParticleEffect
{
public:
    ParticleEffect();

    enum ParticleType {Gunshot, FlakShot, Explosion};
    static QMap<Cube*, unsigned long> activeParticles;

    static void SpawnEffect(QVector3D position, ParticleType type);
    static void Render(QMatrix4x4* world);
    static void Update();
};

#endif // PARTICLEEFFECT_H
