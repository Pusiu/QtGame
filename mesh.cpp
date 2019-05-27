#include "mesh.h"
#include "gamewindow.h"
#include <qmath.h>
#include <iostream>
#include <QFile>
#include <QTextStream>

using namespace std;

Mesh::Mesh()
    : m_count(0), m_primitive(0)
{
}

Mesh::~Mesh()
{
    m_vbo.destroy();
    delete m_vao_binder;
}
QOpenGLBuffer vio;

Mesh::Mesh(aiMesh* assimpMesh, QString name, QVector<Vertex> vertices, QVector<unsigned int> indices, QVector<Texture> textures)
{
    this->assimpMesh=new aiMesh(*assimpMesh);
    m_count=0;
    this->name = name;
    this->vertices = vertices;
    this->indices = indices;
    for (int i=0; i < indices.size(); i++)
    {
        Vertex v = vertices[indices[i]];
        add(v.Position, v.Normal, v.TexCoords);
    }
    this->textures = textures;

    m_primitive = GL_TRIANGLES;
    // now that we have all the required data, set the vertex buffers and its attribute pointers.
    initVboAndVao();
    vio.create();
    vio.bind();
    vio.allocate(indices.constData(), indices.size());

}



void Mesh::add(const QVector3D &v, const QVector3D &n, const QVector2D &uv)
{
    m_data.append(v.x());
    m_data.append(v.y());
    m_data.append(v.z());
    m_data.append(n.normalized().x());
    m_data.append(n.normalized().y());
    m_data.append(n.normalized().z());
    m_data.append(uv.x());
    m_data.append(uv.y());
    m_count++;
}


void Mesh::initVboAndVao()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    int dataSize = m_data.size() * int(sizeof(GLfloat));

    m_vao.create(); // creates vertex array object
    m_vao_binder = new QOpenGLVertexArrayObject::Binder(&m_vao); // binds vertex array object
    m_vbo.create(); // creates vertex buffer object
    m_vbo.bind(); // binds vertex buffer object
    m_vbo.allocate(constData(), dataSize); // copies mesh data to vertex buffer object

    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);
    f->glEnableVertexAttribArray(2);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    f->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void *>(6 * sizeof(GLfloat)));

}



void Mesh::render(GameWindow* window)
{
    m_vao_binder->rebind();
    window->glDrawArrays(m_primitive, 0, vertexCount());

    //window->glDrawElements(m_primitive, indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::quad3(GLfloat x1, GLfloat y1, GLfloat z1,
                 GLfloat x2, GLfloat y2, GLfloat z2,
                 GLfloat x3, GLfloat y3, GLfloat z3,
                 GLfloat x4, GLfloat y4, GLfloat z4)
{
    QVector3D n = QVector3D::normal(QVector3D(x4 - x1, y4 - y1, z4 - z1), QVector3D(x2 - x1, y2 - y1, z2 - z1));

    add(QVector3D(x1, y1, z1), n, QVector2D(0,1));
    add(QVector3D(x4, y4, z4), n, QVector2D(1,1));
    add(QVector3D(x2, y2, z2), n, QVector2D(0,0));

    add(QVector3D(x3, y3, z3), n, QVector2D(1,0));
    add(QVector3D(x2, y2, z2), n, QVector2D(0,0));
    add(QVector3D(x4, y4, z4), n, QVector2D(0,1));
}

void Mesh::generateCube(GLfloat w, GLfloat h, GLfloat d)
{
    quad3(-w/2, -h/2, -d/2,  +w/2, -h/2, -d/2,  +w/2, +h/2, -d/2,  -w/2, +h/2, -d/2);
    quad3(-w/2, -h/2, +d/2,  -w/2, +h/2, +d/2,  +w/2, +h/2, +d/2,  +w/2, -h/2, +d/2);

    quad3(-w/2, -h/2, -d/2,  -w/2, -h/2, +d/2,  +w/2, -h/2, +d/2,  +w/2, -h/2, -d/2);
    quad3(-w/2, +h/2, -d/2,  +w/2, +h/2, -d/2,  +w/2, +h/2, +d/2,  -w/2, +h/2, +d/2);

    quad3(+w/2, -h/2, -d/2,  +w/2, -h/2, +d/2,  +w/2, +h/2, +d/2,  +w/2, +h/2, -d/2);
    quad3(-w/2, -h/2, -d/2,  -w/2, +h/2, -d/2,  -w/2, +h/2, +d/2,  -w/2, -h/2, +d/2);

    m_primitive = GL_TRIANGLES;
    name="Generated cube";

    initVboAndVao();
}

void Mesh::generateMeshFromObjFile(QString filename)
{
    QFile file(filename);
    file.open(QFile::OpenMode::enum_type::ReadOnly);

    std::cout << "Loading " << filename.toStdString() << " - " << (file.isOpen() ? "Found!" : "Not Found!") << std::endl;

    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;
    QVector<QVector2D> texCoords;

    bool hasNormals = false;
    bool hasTexCoords = false;

    QTextStream stream(&file);

    while (!stream.atEnd()) {
        QString line = stream.readLine();
        line = line.simplified();

        if (line.length() > 0 && line.at(0) != QChar::fromLatin1('#')) {
            QTextStream lineStream(&line, QIODevice::ReadOnly);
            QString token;
            lineStream >> token;

            if (token == QStringLiteral("v")) {
                float x, y, z;
                lineStream >> x >> y >> z;
                vertices.append(QVector3D( x, y, z ));
            } else if (token == QStringLiteral("vt")) {
                float s,t;
                lineStream >> s >> t;
                texCoords.append(QVector2D(s, t));
                hasTexCoords = true;
            } else if (token == QStringLiteral("vn")) {
                float x, y, z;
                lineStream >> x >> y >> z;
                normals.append(QVector3D( x, y, z ));
                hasNormals = true;
            } else if (token == QStringLiteral("f")) {
                while (!lineStream.atEnd()) {
                    QString faceString;
                    lineStream >> faceString;

                    QVector3D v;
                    QVector3D n;
                    QVector2D uv;

                    QStringList indices = faceString.split(QChar::fromLatin1('/'));

                    v = vertices.at(indices.at(0).toInt() - 1);

                    if(hasTexCoords)
                        uv = texCoords.at(indices.at(1).toInt() - 1);

                    if(hasNormals)
                        n = normals.at(indices.at(2).toInt() - 1);

                    add(v, n, QVector2D(0,0));
                }
            }
        }
    }

    m_primitive = GL_TRIANGLES;

    initVboAndVao();
}



