#ifndef UTILS_H
#define UTILS_H

#include <QVector3D>
#include <QMatrix4x4>
#include <QQuaternion>
#include <assimp/vector3.h>
#include <assimp/quaternion.h>
#include <assimp/matrix4x4.h>

class Utils
{
public:

    static QVector3D VectorFromAssimp(aiVector3D a)
    {
        QVector3D v;
        v.setX(a.x);
        v.setY(a.y);
        v.setZ(a.z);
        return v;
    }

    static QQuaternion QuaternionFromAssimp(aiQuaternion a)
    {
        QQuaternion q;
        q.setX(a.x);
        q.setY(a.y);
        q.setZ(a.z);
        q.setScalar(a.w);
        return q;
    }

    static QMatrix4x4 MatrixFromAssimp(aiMatrix4x4 m)
    {
        /*QMatrix4x4 q(m.a1, m.b1, m.c1, m.d1,
                     m.a2, m.b2, m.c2, m.d2,
                     m.a3, m.b3, m.c3, m.d3,
                     m.a4, m.b4, m.c4, m.d4
                    );*/
        QMatrix4x4 q(m.a1, m.a2, m.a3, m.a4,
                     m.b1, m.b2, m.b3, m.b4,
                     m.c1, m.c2, m.c3, m.c4,
                     m.d1, m.d2, m.d3, m.d4
                    );
        return q;
    }

};

#endif // UTILS_H
