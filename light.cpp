//#include "renderwindow.h"
#include "light.h"

Light::Light()
{
    mVertices.push_back(Vertex(0.0f, 0.0f,  0.0f,   0.8f, 0.8f, 0.3f,   0.f,  0.f));
    mVertices.push_back(Vertex(0.5f, 0.0f,  0.0f,   0.8f, 0.8f, 0.3f,   1.f,  0.f));
    mVertices.push_back(Vertex(0.0f, 0.5f,  0.0f,   0.8f, 0.8f, 0.3f,   0.5f, 0.5f));
//    mVertices.push_back(Vertex(0.0f, -0.25f, -0.25f,     0.8f, 0.8f, 0.3f,  0.5f, 0.5f));


//    mVertices.insert( mVertices.end(),
//    {
//     Vertex{QVector3D(-0.25f, -0.25f,  0.25f),    QVector3D(0.8f, 0.8f, 0.3f),  QVector2D(0.f,  0.f)},   //Left low
//     Vertex{QVector3D( 0.25f, -0.25f,  0.25f),    QVector3D(0.8f, 0.8f, 0.3f),  QVector2D(1.f,  0.f)},   //Right low
//     Vertex{QVector3D( 0.0f,  0.25f,  0.0f),      QVector3D(0.8f, 0.8f, 0.3f),  QVector2D(0.5f, 0.5f)},  //Top
//     Vertex{QVector3D( 0.0f, -0.25f, -0.25f),     QVector3D(0.8f, 0.8f, 0.3f),  QVector2D(0.5f, 0.5f)}   //Back low
//     });

//    mIndices.insert( mIndices.end(),
//    { 0, 1, 2,
//      1, 3, 2,
//      3, 0, 2,
//      0, 3, 1
//    });

    mMatrix.setToIdentity();
}

//void Light::draw()
//{
//    glBindVertexArray( mVAO );
//    glDrawElements(GL_TRIANGLES, 0, GL_UNSIGNED_INT, nullptr);
//    glBindVertexArray(0);
//}


