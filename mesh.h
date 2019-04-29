#ifndef Mesh_H
#define Mesh_H

#define GL_GLEXT_PROTOTYPES

#include <qopengl.h>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <string>


using namespace std;

class GameWindow;

struct Texture {
    unsigned int id;
    string type;
    string path;
};

struct Vertex {
    QVector3D Position;
    QVector3D Normal;
    QVector2D TexCoords;
    QVector3D tangent;
    QVector3D bitangent;
};

const int NUM_BONES_PER_VERTEX = 4;

struct VertexBoneData
{
    uint IDs[NUM_BONES_PER_VERTEX];
    float Weights[NUM_BONES_PER_VERTEX];
};

class Mesh
{
public:
    Mesh();
    ~Mesh();


    QString name;
    QVector<Vertex> vertices;
    QVector<unsigned int> indices;
    QVector<Texture> textures;

    aiMesh* assimpMesh;

    Mesh(aiMesh* assimpMesh, QString name, QVector<Vertex> vertices, QVector<unsigned int> indices, QVector<Texture> textures);

    const GLfloat *constData() const { return m_data.constData(); }
    unsigned int vertexCount() const { return m_count; }
    GLenum primitive() {return m_primitive; }

    void generateCube(GLfloat ww, GLfloat hh, GLfloat dd);
    void generateSphere(float r, int N);
    void generateMeshFromObjFile(QString filename);

    void initVboAndVao();

    void render(GameWindow* window);

    QVector<GLfloat> m_data;
    unsigned int m_count;
    GLenum m_primitive = GL_TRIANGLES;
    GLint stride = 6 * sizeof(GLfloat); //stride is elements * size of element

    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
    QOpenGLVertexArrayObject::Binder* m_vao_binder;

private:
    void add(const QVector3D &v, const QVector3D &n);

    void quad3(GLfloat x1, GLfloat y1, GLfloat z1,
               GLfloat x2, GLfloat y2, GLfloat z2,
               GLfloat x3, GLfloat y3, GLfloat z3,
               GLfloat x4, GLfloat y4, GLfloat z4);


};

#endif // CMesh_H

