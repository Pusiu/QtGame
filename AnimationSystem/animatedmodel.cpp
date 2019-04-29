#include "animatedmodel.h"
#include <math.h>

AnimatedModel::AnimatedModel(QString path) : indices(QOpenGLBuffer::IndexBuffer)
{
    model = new Model();
    timer = GameWindow::instance->m_timer;
    LoadAnimatedModel(path);
}

AnimatedModel::~AnimatedModel()
{
    delete model;
}
Assimp::Importer importer;

void AnimatedModel::LoadAnimatedModel(QString path)
{

    const aiScene* scene =  importer.ReadFile(path.toStdString().data(),
                aiProcess_Triangulate |
                aiProcess_GenSmoothNormals |
                aiProcess_FlipUVs |
                aiProcess_CalcTangentSpace |
                aiProcess_LimitBoneWeights |
                aiProcess_JoinIdenticalVertices
            );

            if(scene == nullptr | scene->mNumAnimations == 0)
            {
                std:: cout << "the imported file does not contain any animations." << std::endl;
                return;
            }

            aiMesh* mesh = scene->mMeshes[0];

            unsigned int sizeOfVertex = 19;
            unsigned int sizeOfVertexUnrigged = 11;
            /**
             * position data 3f
             * normal   data 3f
             * tangent  data 3f
             * texcoord data 2f
             *
             * bone		info 4f
             * bone		info 4f
             */

            unsigned int arrayLength = mesh->mNumVertices * sizeOfVertex;
            float* array = new float[arrayLength];
            int index 		 = 0;


            for(unsigned int v = 0; v < mesh->mNumVertices; v++)
            {
                aiVector3D position = mesh->mVertices[v];
                aiVector3D normal   = mesh->mNormals[(v)];
                aiVector3D tangent  = mesh->mTangents[(v)];
                aiVector3D texCoord = mesh->mTextureCoords[0][(v)];
                /**
                 * The above assumes that the program has texture coordinates, if it doesn't the program will throw a null pointer exception.
                 */

                array[index++] = position.x;
                array[index++] = position.y;
                array[index++] = position.z;

                array[index++] = normal.x;
                array[index++] = normal.y;
                array[index++] = normal.z;

                array[index++] = texCoord.x;
                array[index++] = texCoord.y;

                array[index++] = tangent.x;
                array[index++] = tangent.y;
                array[index++] = tangent.z;

                array[index++] = 0;
                array[index++] = 0;
                array[index++] = 0;
                array[index++] = 0;

                array[index++] = 0;
                array[index++] = 0;
                array[index++] = 0;
                array[index++] = 0;

            }



            index = 0;
            /**
             * ^
             * this has to be here
             */

            QVector<unsigned int> indices;
           /* unsigned int* indices = new unsigned int[mesh->mNumFaces * mesh->mFaces[0].mNumIndices];

            for(int f = 0; f < mesh->mNumFaces; f++)
            {
                aiFace face = mesh->mFaces[f];
                for(int ind = 0; ind < face.mNumIndices; ind++)
                {
                    indices[(f*mesh->mFaces[0].mNumIndices)+ind]=face.mIndices[ind];
                }
            }*/

            for(unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                // retrieve all indices of the face and store them in the indices vector
                for(unsigned int j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }

            QHash<QString, int> boneMap;
            QHash<int, int> bone_index_map0;
            QHash<int, int> bone_index_map1;

            for(int b = 0; b < mesh->mNumBones; b++)
            {
                aiBone* bone = mesh->mBones[b];
                boneMap.insert(bone->mName.C_Str(), b);

                for(int w = 0; w < bone->mNumWeights; w++)
                {
                    aiVertexWeight weight = bone->mWeights[w];
                    int vertexIndex = weight.mVertexId;
                    int findex		= vertexIndex * sizeOfVertex;

                    if(!bone_index_map0.contains(vertexIndex))
                    {
                        array[(findex + sizeOfVertexUnrigged) + 0] = b;
                        array[(findex + sizeOfVertexUnrigged) + 2] = weight.mWeight;
                        bone_index_map0.insert(vertexIndex, 0);
                    } else if(bone_index_map0[vertexIndex] == 0)
                    {
                        array[(findex + sizeOfVertexUnrigged) + 1] = b;
                        array[(findex + sizeOfVertexUnrigged) + 3] = weight.mWeight;
                        bone_index_map0.insert(vertexIndex, 1);
                    } else if(!bone_index_map1.contains(vertexIndex))
                    {
                        array[(findex + sizeOfVertexUnrigged) + 4] = b;
                        array[(findex + sizeOfVertexUnrigged) + 6] = weight.mWeight;
                        bone_index_map1.insert(vertexIndex, 0);
                    } else if(bone_index_map1[(vertexIndex)] == 0)
                    {
                        array[(findex + sizeOfVertexUnrigged) + 5] = b;
                        array[(findex + sizeOfVertexUnrigged) + 7] = weight.mWeight;
                        bone_index_map1.insert(vertexIndex, 1);
                    } else {
                        std::cout << "max 4 bones per vertex." << std::endl;
                        return;
                    }
                }
            }

            aiMatrix4x4 inverseRootTransform = scene->mRootNode->mTransformation;
            QMatrix4x4	inverseRootTransformation = Utils::MatrixFromAssimp(inverseRootTransform);

            Bone** bones = new Bone*[boneMap.size()];

            for(int b = 0; b < mesh->mNumBones; b++)
            {
                aiBone* bone = mesh->mBones[b];
                bones[b] = new Bone();

                bones[b]->name		  = bone->mName.C_Str();
                bones[b]->offsetMatrix = Utils::MatrixFromAssimp(bone->mOffsetMatrix);
                this->bones.push_back(bones[b]);
            }


            this->model->meshes.push_front(new Mesh());
            this->model->meshes[0]->assimpMesh=mesh;
            this->model->meshes[0]->m_data.clear();
            this->model->meshes[0]->m_count=arrayLength/sizeOfVertex;
            this->model->meshes[0]->stride = sizeOfVertex * sizeof(GLfloat); //stride is specified in bytes, 19 elements * size of float



            this->model->meshes[0]->m_vao.create();
            this->model->meshes[0]->m_vao_binder = new QOpenGLVertexArrayObject::Binder(&this->model->meshes[0]->m_vao);

            this->model->meshes[0]->m_vbo.create();
            this->model->meshes[0]->m_vbo.bind();
            this->model->meshes[0]->m_vbo.allocate(array, arrayLength * sizeof(GLfloat));

            this->model->meshes[0]->m_vbo.release();
            this->indices.create();
            this->indices.bind();
            //this->indices.allocate(indices,mesh->mNumFaces * mesh->mFaces[0].mNumIndices);
            this->indices.allocate(indices.constData(),mesh->mNumFaces * mesh->mFaces[0].mNumIndices * sizeof(GLuint));

            this->model->meshes[0]->m_vao_binder->release();


            animation = new aiAnimation(*(scene->mAnimations[0]));
            this->boneTransforms.resize(boneMap.size());

            this->root			 = scene->mRootNode;
            this->globalInverseTransform = inverseRootTransformation;

}

void AnimatedModel::Draw(Shader* shader) {
    /*stringstream ss;
    for(int i = 0; i < boneTransforms->size(); i++)
    {
        ss << "gBones[" << i << "]";
        shader->program->setUniformValue(ss.str().c_str(), *((*boneTransforms)[i]));
        ss.clear();
    }*/

    //shader->program->enableAttributeArray()

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    model->meshes[0]->m_vao_binder->rebind();

    model->meshes[0]->m_vbo.bind();
    int boneArrayLoc = shader->program->uniformLocation("gBones");
    int textureLoc = shader->program->attributeLocation("texCoord");
    int tangentLoc = shader->program->attributeLocation("tangent");
    int boneDataALoc = shader->program->attributeLocation("boneDataA");
    int boneDataBLoc = shader->program->attributeLocation("boneDataB");

    shader->program->enableAttributeArray(boneArrayLoc);
    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);
    f->glEnableVertexAttribArray(textureLoc);
    f->glEnableVertexAttribArray(tangentLoc);
    f->glEnableVertexAttribArray(boneDataALoc);
    f->glEnableVertexAttribArray(boneDataBLoc);

    /*for (int i=0; i < boneTransforms.size();i++)
        boneTransforms[i] = QMatrix4x4();*/

    shader->program->setUniformValueArray(boneArrayLoc, boneTransforms.data(), boneTransforms.size());

    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, model->meshes[0]->stride, nullptr);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, model->meshes[0]->stride, reinterpret_cast<void *>(3 * sizeof(GLfloat)));//offset=3 of position
    f->glVertexAttribPointer(textureLoc, 2, GL_FLOAT, GL_FALSE, model->meshes[0]->stride, reinterpret_cast<void *>(6 * sizeof(GLfloat)));//offset=3 of position + 3 of normal
    f->glVertexAttribPointer(tangentLoc, 3, GL_FLOAT, GL_FALSE, model->meshes[0]->stride, reinterpret_cast<void *>(8 * sizeof(GLfloat)));//offset=3 of position + 3 of normal + 2 of texture
    f->glVertexAttribPointer(boneDataALoc, 4, GL_FLOAT, GL_FALSE, model->meshes[0]->stride, reinterpret_cast<void *>(11 * sizeof(GLfloat))); //offset=3 of position + 3 of normal + 2 of texture + 3 of tangent
    f->glVertexAttribPointer(boneDataBLoc, 4, GL_FLOAT, GL_FALSE, model->meshes[0]->stride, reinterpret_cast<void *>(15 * sizeof(GLfloat))); //offset=3 of position + 3 of normal + 2 of texture + 3 of tangent + 4 of boneDataA

    //model->Draw();

    //GameWindow::instance->glDrawArrays(model->meshes[0]->m_primitive, 0, model->meshes[0]->vertexCount());
    //GameWindow::instance->glDrawArrays(model->meshes[0]->m_primitive, 0, model->meshes[0]->m_vbo.size()/14);

    //model->meshes[0]->m_vbo.release();
    indices.bind();

    int bsize=indices.size();
    GameWindow::instance->glDrawElements(GL_TRIANGLES, bsize, GL_UNSIGNED_INT, 0);

    shader->program->disableAttributeArray(boneArrayLoc);
    f->glDisableVertexAttribArray(0);
    f->glDisableVertexAttribArray(0);
    f->glDisableVertexAttribArray(textureLoc);
    f->glDisableVertexAttribArray(tangentLoc);
    f->glDisableVertexAttribArray(boneDataALoc);
    f->glDisableVertexAttribArray(boneDataBLoc);
    indices.release();

    model->meshes[0]->m_vao_binder->release();
}

void AnimatedModel::Update() {

    SetBoneTransforms((float)(((double)GameWindow::instance->timerSinceStart.elapsed() - (double)timer) / 1000.0));
}

aiNodeAnim* AnimatedModel::FindNodeAnim(aiAnimation* pAnimation, QString NodeName)
{
    for (int i = 0 ; i < pAnimation->mNumChannels; i++) {
        aiNodeAnim* pNodeAnim = new aiNodeAnim(*(pAnimation->mChannels[i]));

        const char* c1 = pNodeAnim->mNodeName.C_Str();
        string c3 = NodeName.toStdString();

        if (c3.compare(c1) == 0)    // strcmp(c1,c2) == 0)
            return pNodeAnim;
    }

    return nullptr;
}

void AnimatedModel::CalcInterpolatedPosition(QVector3D* Out, float AnimationTime, aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumPositionKeys == 1) {
        *Out = Utils::VectorFromAssimp(pNodeAnim->mPositionKeys[0].mValue);
        return;
    }

    int PositionIndex = FindPosition(AnimationTime, pNodeAnim);
    int NextPositionIndex = (PositionIndex + 1);
    assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
    float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    QVector3D Start = Utils::VectorFromAssimp(pNodeAnim->mPositionKeys[PositionIndex].mValue);
    QVector3D End = Utils::VectorFromAssimp(pNodeAnim->mPositionKeys[NextPositionIndex].mValue);
    QVector3D Delta = End - Start;
    *Out = Start+(Delta * Factor);// + Factor * Delta;
}

void AnimatedModel::CalcInterpolatedRotation(QQuaternion* Out, float AnimationTime, aiNodeAnim* pNodeAnim)
{
    // we need at least two values to interpolate...
    if (pNodeAnim->mNumRotationKeys == 1) {
        *Out = Utils::QuaternionFromAssimp(pNodeAnim->mRotationKeys[0].mValue);
        return;
    }

    int RotationIndex = FindRotation(AnimationTime, pNodeAnim);
    int NextRotationIndex = (RotationIndex + 1);
    assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
    float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    QQuaternion StartRotationQ = Utils::QuaternionFromAssimp(pNodeAnim->mRotationKeys[RotationIndex].mValue);
    QQuaternion EndRotationQ   = Utils::QuaternionFromAssimp(pNodeAnim->mRotationKeys[NextRotationIndex].mValue);
    *Out = QQuaternion::slerp(StartRotationQ,EndRotationQ, Factor);// = AIQuaternion.Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
//        Out = Out.Normalize();
}


void AnimatedModel::CalcInterpolatedScaling(QVector3D* Out, float AnimationTime, aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumScalingKeys == 1) {
        *Out = Utils::VectorFromAssimp(pNodeAnim->mScalingKeys[0].mValue);
        return;
    }

    int ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
    int NextScalingIndex = (ScalingIndex + 1);
    assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
    float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    QVector3D Start = Utils::VectorFromAssimp(pNodeAnim->mScalingKeys[ScalingIndex].mValue);
    QVector3D End   = Utils::VectorFromAssimp(pNodeAnim->mScalingKeys[NextScalingIndex].mValue);
    QVector3D Delta = End- Start;
    *Out = Start + (Delta*Factor);
}

int AnimatedModel::FindPosition(float AnimationTime, aiNodeAnim* pNodeAnim)
{
    for (int i = 0 ; i < pNodeAnim->mNumPositionKeys - 1 ; i++) {
        if (AnimationTime < (float)pNodeAnim->mPositionKeys[(i + 1)].mTime) {
            return i;
        }
    }

    return 0;
}


int AnimatedModel::FindRotation(float AnimationTime, aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumRotationKeys > 0);

    for (int i = 0 ; i < pNodeAnim->mNumRotationKeys - 1 ; i++) {
        if (AnimationTime < (float)pNodeAnim->mRotationKeys[(i + 1)].mTime) {
            return i;
        }
    }

    return 0;
}


int AnimatedModel::FindScaling(float AnimationTime, aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumScalingKeys > 0);

    for (int i = 0 ; i < pNodeAnim->mNumScalingKeys - 1 ; i++) {
        if (AnimationTime < (float)pNodeAnim->mScalingKeys[(i + 1)].mTime) {
            return i;
        }
   }

    return 0;
}

void AnimatedModel::ReadNodeHeirarchy(float AnimationTime, aiNode* pNode, QMatrix4x4 ParentTransform)
{
    QString NodeName(pNode->mName.data);

//        engine.animation pAnimation = null;//m_pScene.mAnimations[0];

    QMatrix4x4 NodeTransformation = Utils::MatrixFromAssimp(pNode->mTransformation);//(pNode.mTransformation);

    aiNodeAnim* pNodeAnim = FindNodeAnim(animation, NodeName);

    if (pNodeAnim != nullptr)
    {
        // Interpolate scaling and generate scaling transformation matrix
        QVector3D Scaling = QVector3D(0, 0, 0);
        CalcInterpolatedScaling(&Scaling, AnimationTime, pNodeAnim);
        QMatrix4x4 ScalingM;
        ScalingM.setToIdentity();
        ScalingM.scale(Scaling.x(), Scaling.y(), Scaling.z());

        // Interpolate rotation and generate rotation transformation matrix
        QQuaternion RotationQ = QQuaternion(0, 0, 0, 0);
        CalcInterpolatedRotation(&RotationQ, AnimationTime, pNodeAnim);

        QVector3D forward(2.0f * (RotationQ.x() * RotationQ.z() - RotationQ.scalar() * RotationQ.y()),
                          2.0f * (RotationQ.y() * RotationQ.z() + RotationQ.scalar() * RotationQ.x()),
                          1.0f - 2.0f * (RotationQ.x() * RotationQ.x() + RotationQ.y() * RotationQ.y()));

        QVector3D up(2.0f * (RotationQ.x() * RotationQ.y() + RotationQ.scalar() * RotationQ.z()),
                     1.0f - 2.0f * (RotationQ.x() * RotationQ.x() + RotationQ.z() * RotationQ.z()),
                     2.0f * (RotationQ.y() * RotationQ.z() - RotationQ.scalar() * RotationQ.x()));

        QVector3D right(1.0f - 2.0f * (RotationQ.y() * RotationQ.y() + RotationQ.z() * RotationQ.z()),
                        2.0f * (RotationQ.x() * RotationQ.y() - RotationQ.scalar() * RotationQ.z()),
                        2.0f * (RotationQ.x() * RotationQ.z() + RotationQ.scalar() * RotationQ.y()));

        QMatrix4x4 RotationM;
        RotationM(0,0)=right.x();   RotationM(0,1)=right.y();   RotationM(0,2)=right.z();   RotationM(0,3)=0;
        RotationM(1,0)=up.x();      RotationM(1,1)=up.y();      RotationM(1,2)=up.z();   RotationM(1,3)=0;
        RotationM(2,0)=forward.x(); RotationM(2,1)=forward.y(); RotationM(2,2)=forward.z();   RotationM(2,3)=0;
        RotationM(3,0)=0;           RotationM(3,1)=0;           RotationM(3,2)=0;           RotationM(3,3)=1;

        //RotationM.setRow(0,QVector4D(right.x(),right.y(),right.z(),0));

        //RotationM.setRow(1,QVector4D(up.x(),up.y(),up.z(),0));

        //RotationM.setRow(2,QVector4D(forward.x(),forward.y(),forward.z(),0));

        //RotationM.setRow(3,QVector4D(0,0,0,1));

        //QMatrix4x4 RotationM(RotationQ.toRotationMatrix());

        // Interpolate translation and generate translation transformation matrix
        QVector3D Translation = QVector3D(0, 0, 0);
        CalcInterpolatedPosition(&Translation, AnimationTime, pNodeAnim);
        QMatrix4x4 TranslationM;
        TranslationM.setToIdentity();
        TranslationM.translate(Translation.x(), Translation.y(), Translation.z());

        // Combine the above transformations
        NodeTransformation = TranslationM * RotationM * ScalingM;
    }

    QMatrix4x4 GlobalTransformation = ParentTransform * (NodeTransformation);

    Bone* bone = nullptr;

    if ((bone = findBone(NodeName)) != nullptr)
    {
        QMatrix4x4 temp = (globalInverseTransform * (GlobalTransformation));
        bone->finalTransformation = temp * (bone->offsetMatrix);
    }

    for (int i = 0 ; i < pNode->mNumChildren; i++) {
        ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
    }
}

Bone* AnimatedModel::findBone(QString name)
{
    for(Bone* bone : bones)
    {
        if(bone->name.toStdString().compare(name.toStdString()) == 0)
            return bone;
    }

    return nullptr;
}

void AnimatedModel::SetBoneTransforms(float timeInSeconds)
{
    QMatrix4x4 Identity;
    Identity.setToIdentity();

    float TicksPerSecond = (float)(animation->mTicksPerSecond != 0 ? animation->mTicksPerSecond : 25.0f);
    float TimeInTicks = timeInSeconds * TicksPerSecond;
    float AnimationTime = (fmod(TimeInTicks,(float)animation->mDuration));

    ReadNodeHeirarchy(AnimationTime, root, Identity);

//        boneTransforms.resize(m_NumBones);

    for (short i = 0 ; i < bones.length() ; i++) {
        boneTransforms[i] = bones[i]->finalTransformation;
    }
}
