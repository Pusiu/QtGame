#ifndef ANIMATEDMODEL_H
#define ANIMATEDMODEL_H


#include "debug.h"
#include "model.h"
#include <utils.h>
#include "AnimationSystem/bone.h"

class AnimatedModel
{
public:

    Model* model;
    QMatrix4x4 	globalInverseTransform;
    QVector<Bone*>		bones;
    QVector<QMatrix4x4> 	boneTransforms;
    aiNode*		root;
    aiAnimation*	animation;

    QOpenGLBuffer indices;

    unsigned long timer;

    AnimatedModel(QString path);
    ~AnimatedModel();
    void LoadAnimatedModel(QString path);

    void Update();
    void Draw(Shader* shader);

    aiNodeAnim* FindNodeAnim(aiAnimation* pAnimation, QString NodeName);
    void SetBoneTransforms(float timeInSeconds);
    Bone* findBone(QString name);
    void ReadNodeHeirarchy(float AnimationTime, aiNode* pNode, QMatrix4x4 ParentTransform);
    int FindScaling(float AnimationTime, aiNodeAnim* pNodeAnim);
    int FindRotation(float AnimationTime, aiNodeAnim* pNodeAnim);
    int FindPosition(float AnimationTime, aiNodeAnim* pNodeAnim);
    void CalcInterpolatedScaling(QVector3D* Out, float AnimationTime, aiNodeAnim* pNodeAnim);
    void CalcInterpolatedRotation(QQuaternion* Out, float AnimationTime, aiNodeAnim* pNodeAnim);
    void CalcInterpolatedPosition(QVector3D* Out, float AnimationTime, aiNodeAnim* pNodeAnim);

};

#endif // ANIMATEDMODEL_H