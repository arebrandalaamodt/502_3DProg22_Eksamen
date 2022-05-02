#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include "vertex.h"

//#define GL_POINTS                         0x0000
//#define GL_LINES                          0x0001
//#define GL_LINE_LOOP                      0x0002
//#define GL_LINE_STRIP                     0x0003
//#define GL_TRIANGLES                      0x0004
//#define GL_TRIANGLE_STRIP                 0x0005
//#define GL_TRIANGLE_FAN                   0x0006
//#define GL_QUADS                          0x0007
//#define GL_QUAD_STRIP                     0x0008
//#define GL_POLYGON                        0x0009

struct UV
{
    float u;
    float v;
};

enum EDrawMethod
{
    Triangles = GL_TRIANGLES, LinesStrip = GL_LINE_STRIP, Lines = GL_LINES
};

struct ShaderInfo{
    class Shader* Shader                {nullptr};
    GLint* MatrixUniform                {nullptr};

    GLint* ViewMatrixUniform            {nullptr};
    GLint* ProjectionMatrixUniform      {nullptr};

    GLint* TextureUniform               {nullptr};


    GLint TextureID {0};

};


class VisualObject : public QOpenGLFunctions_4_1_Core {
public:
    VisualObject();
    VisualObject(std::string name, std::vector<Vertex> verticesVector);
    ~VisualObject() {glDeleteVertexArrays( 1, &mVAO); glDeleteBuffers( 1, &mVBO );};

    virtual void init();
    virtual void draw();

    virtual void move(float x, float y, float z);
    virtual void move(float dt) { };
    virtual void move(const QVector3D& xyz) { };


    virtual QVector3D getPosition();
    virtual void setPosition (const QVector3D positionToSet);

    virtual void updateMatrix();

    virtual void setMonoColor(QVector3D colorToSet);


    virtual void setVertices(std::vector<Vertex> vertices) {mVertices = vertices; }

public:
   QMatrix4x4 mMatrix;
protected:
   QMatrix4x4 mPosition;
   QMatrix4x4 mRotation;
   QMatrix4x4 mScale;
//   QVector3D mVelocity{0.1f, 0.f, 0.f};

//////////////////////////////////////////////
protected:
   std::vector<Vertex> mVertices;
   GLuint mVAO{0};
   GLuint mVBO{0};

   float mx, my, mz {0.f};

   std::string mName{"Default"};
   void setName(std::string name);
   std::string getName();

   void setDrawMethod(EDrawMethod method) {mDrawMethod = method;}
   EDrawMethod mDrawMethod {EDrawMethod::Triangles};

   ShaderInfo mShaderInfo;
   void setupShader(ShaderInfo shaderInfo) {mShaderInfo = shaderInfo;}




//   void setMatrix(GLint* matrixToSet) {mMatrixUniform = matrixToSet;}
//   void setShader(Shader* shaderToSet) {mShader = shaderToSet;}


   friend class RenderWindow;
};
#endif // VISUALOBJECT_H
