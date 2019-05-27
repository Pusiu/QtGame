#include "model.h"
#include "debug.h"
#include <QQueue>

Model::Model()
{

}

Model::~Model()
{
    for(int i = 0; i < meshes.size(); i++)
        delete meshes[i];
}

Model::Model(QString path)
{
        LoadModel(path);
}

void Model::Draw()
{
    for(int i = 0; i < meshes.size(); i++)
        meshes[i]->render(GameWindow::instance);
}

bool Model::LoadModel(QString path)
{
    scene = import.ReadFile(path.toLocal8Bit().data(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
        return false;
    }
    directory = path.mid(0, path.lastIndexOf('/'));

    processNode(scene->mRootNode, scene);
    assimpRootNode=scene->mRootNode;
    return true;
}




void Model::processNode(aiNode *node, const aiScene *scene)
{
    // process each mesh located at the current node
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene.
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene);
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

void Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
     // data to fill
            QVector<Vertex> vertices;
            QVector<unsigned int> indices;
            vector<Texture> textures;


            // Walk through each of the mesh's vertices
            for(unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                Vertex vertex;
                // positions
                QVector3D v(mesh->mVertices[i].x, mesh->mVertices[i].y,mesh->mVertices[i].z);
                vertex.Position = v;

                QVector3D n(mesh->mNormals[i].x, mesh->mNormals[i].y,mesh->mNormals[i].z);
                vertex.Normal=n;
                // texture coordinates
                if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
                {
                    // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
                    // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                    QVector2D vec(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
                    vertex.TexCoords = vec;
                }
                else
                    vertex.TexCoords = QVector2D(0.0f, 0.0f);

                /*
                // tangent
                QVector3D tangent(mesh->mTangents[i].x,mesh->mTangents[i].y,mesh->mTangents[i].z);
                vertex.tangent = tangent;
                // bitangent
                QVector3D bitangent(mesh->mBitangents[i].x,mesh->mBitangents[i].y,mesh->mBitangents[i].z);
                vertex.bitangent = bitangent;
                */

                vertices.push_back(vertex);
            }
            // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
            for(unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                // retrieve all indices of the face and store them in the indices vector
                for(unsigned int j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }
            /*
            // process materials
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
            // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
            // Same applies to other texture as the following list summarizes:
            // diffuse: texture_diffuseN
            // specular: texture_specularN
            // normal: texture_normalN

            // 1. diffuse maps
            QVector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            // 2. specular maps
            QVector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
            // 3. normal maps
            QVector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
            // 4. height maps
            QVector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
            textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
             */
            // return a mesh object created from the extracted mesh data

            meshes.push_back(new Mesh(mesh, QString(scene->mMeshes[0]->mName.C_Str()), vertices, indices, QVector<Texture>().fromStdVector(textures)));
}


