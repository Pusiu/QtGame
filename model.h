#ifndef MODEL_H
#define MODEL_H
#include "mesh.h"
#include "gamewindow.h"
#include <utils.h>
#include <AnimationSystem/bone.h>
#include <iostream>

using namespace std;

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Model
{
public:
    /*  Model Data */
    QVector<Mesh*> meshes;
    QString directory;
    bool gammaCorrection;
    aiNode* assimpRootNode = nullptr;
    QList<aiBone*> allBones;
    aiNode* rootBone;

    Assimp::Importer import;
    const aiScene* scene;


    Model();
    ~Model();
    /*  Functions   */
    // constructor, expects a filepath to a 3D model.
    Model(QString path);

    // draws the model, and thus all its meshes
    void Draw();

private:
    /*  Functions   */
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    bool LoadModel(QString path);

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode *node, const aiScene *scene);

    void processMesh(aiMesh *mesh, const aiScene *scene);
};



#endif // MODEL_H
