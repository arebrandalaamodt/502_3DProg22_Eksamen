#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include "vertex.h"


struct UV
{
    float u;
    float v;
};

enum EDrawMethod
{
    Triangles = GL_TRIANGLES, LinesStrip = GL_LINE_STRIP, Lines = GL_LINES, Points = GL_POINTS
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

    bool bEditorOnlyRender {false};
    bool bShouldBeRendered {true};
    bool bEditorMode       {true};
    void toggleEditorMode();

    QVector3D Barycentric(QVector3D object, QVector3D P1, QVector3D P2, QVector3D P3);
    bool isOverlappingTriangle(QVector3D baryc, QVector3D P1, QVector3D P2, QVector3D P3);
    float GetBarycentricHeight(QVector3D baryc, QVector3D P1, QVector3D P2, QVector3D P3);

   friend class RenderWindow;
};
#endif // VISUALOBJECT_H
