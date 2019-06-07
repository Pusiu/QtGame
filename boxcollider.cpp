#include "boxcollider.h"
#include "cube.h"

QList<BoxCollider*> BoxCollider::allColliders;

void BoxCollider::LoadColliders(QString path)
{
    Assimp::Importer* importer = new Assimp::Importer();

    const aiScene* scene =  importer->ReadFile(path.toStdString().data(),
                aiProcess_Triangulate |
                aiProcess_FlipUVs |
                aiProcess_JoinIdenticalVertices
            );


    QList<aiNode*> nodes;
    nodes.push_back(scene->mRootNode);
    while (!nodes.empty())
    {
        aiNode* n = nodes.front();
        nodes.pop_front();
        for (int i=0; i < n->mNumMeshes;i++)
        {
            aiQuaternion airot;
            aiVector3D aipos;
            aiVector3D aiscale;
            n->mTransformation.Decompose(aiscale, airot, aipos);


            //map scale is 5
            QVector3D position = Utils::VectorFromAssimp(aipos) / 100 * 5;
            position.setX(-position.x());
            position.setZ(-position.z());
            QVector3D scale = Utils::VectorFromAssimp(aiscale) / 10 / 2;
            scale=QVector3D(scale.y(), scale.z(), scale.x());
            QQuaternion rot = Utils::QuaternionFromAssimp(airot);

            //quaternion magic. I really don't know why
            rot = QQuaternion(airot.z, -airot.y, -airot.w, airot.x);


            /*(Cube* go = new Cube(new Model("resources/meshes/cube.fbx"));
            go->position=position;
            go->rotation=rot;
            go->scale=scale.normalized();
            go->shader=GameWindow::instance->shaders["simple"];
            GameWindow::instance->gameObjects.push_back(go);*/

            BoxCollider* c = new BoxCollider(position, rot, scale);
            c->name=QString(n->mName.C_Str());
        }

        for (int i=0; i < n->mNumChildren;i++)
        {
            nodes.push_back(n->mChildren[i]);
        }

    }

    if (BoxCollider::allColliders[0]->IsPointInside(BoxCollider::allColliders[0]->position + QVector3D(0,0.2f,0)))
    {
        qDebug("success");
    }

}

BoxCollider::BoxCollider(QVector3D position, QQuaternion rotation, QVector3D scale)
{
    this->position=position;
    this->rotation=rotation;
    this->scale=scale;
    allColliders.push_back(this);
}

bool BoxCollider::IsPointInside(QVector3D point)
{
 QVector3D forward = rotation * QVector3D(1,0,0);
 forward.normalize();
 QVector3D right = QVector3D::crossProduct(forward, QVector3D(0,1,0));
 forward.normalize();

 float xSize=(scale*right).length();
 float zSize=(scale*forward).length();

 if (point.y() <= position.y()+scale.y() && point.y() >= position.y()-scale.y())
    if (point.x() <= position.x()+xSize && point.x() >= position.x()-xSize)
        if (point.z() <= position.z()+zSize && point.z() >= position.z()-zSize)
        {
            /*char c[256];
            sprintf(c, "collision with %s", name.toStdString().data());
            qDebug(c);*/
            return true;
        }

 return false;
}
