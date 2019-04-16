#include "animatedmodel.h"
#include <QQueue>

AnimatedModel::AnimatedModel(Model m, Joint* rootJoint, int jointCount)
{
    model = m;
    animator=new Animator(this);
    SetupBones();
    //this->rootJoint=rootJoint;
   // this->jointCount=jointCount;
   // rootJoint->CalculateInverseBindTransform(QMatrix4x4());
}

void AnimatedModel::SetupBones()
{
    //finding skeleton
    aiNode* skeletonRoot = model.rootBone;
    QQueue<aiNode*> q;
    QQueue<Joint*> parents;
    q.push_back(skeletonRoot);

    int k=0;
    while (!q.empty())
    {
        aiNode* a = q.front();
        q.pop_front();
        aiBone* b;
        for (int i=0; i < model.allBones.size();i++)
        {
            if (model.allBones[i]->mName==a->mName)
            {
                b=model.allBones[i];
                break;
            }
        }
        QMatrix4x4 m = QMatrix4x4(b->mOffsetMatrix.a1, b->mOffsetMatrix.a2, b->mOffsetMatrix.a3, b->mOffsetMatrix.a4,
                                  b->mOffsetMatrix.b1, b->mOffsetMatrix.b2, b->mOffsetMatrix.b3, b->mOffsetMatrix.b4,
                                  b->mOffsetMatrix.c1, b->mOffsetMatrix.c2, b->mOffsetMatrix.c3, b->mOffsetMatrix.c4,
                                  b->mOffsetMatrix.d1, b->mOffsetMatrix.d2, b->mOffsetMatrix.d3, b->mOffsetMatrix.d4
                                  );

        Joint* j = new Joint(k,b->mName.C_Str(), m);

        Joint* parent = parents.front();
        parents.pop_front();
        if (parent != nullptr)
            parent->AddChild(j);

        if (rootJoint == nullptr)
            rootJoint=j;


        for (int i=0; i < a->mNumChildren;i++)
        {
            parents.push_back(j);
            q.push_back(a->mChildren[i]);
        }
    }






}

Joint* AnimatedModel::GetRootJoint()
{
    return rootJoint;
}

void AnimatedModel::PlayAnimation(Animation anim)
{
    animator->PlayAnimation(&anim);
}

void AnimatedModel::Update()
{
    animator->Update();
}

QMatrix4x4* AnimatedModel::GetJointTransforms()
{
    QMatrix4x4 *jointMatrices = new QMatrix4x4[jointCount];
    AddJointsToArray(rootJoint, jointMatrices);
    return jointMatrices;
}

void AnimatedModel::AddJointsToArray(Joint *headJoint, QMatrix4x4 jointMatrices[])
{
    jointMatrices[headJoint->index] = headJoint->GetAnimatedTransform();
    for (Joint* childJoint : headJoint->children)
        AddJointsToArray(childJoint, jointMatrices);
}
