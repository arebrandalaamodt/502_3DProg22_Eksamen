// VisualObject.cpp
#include "visualobject.h"



VisualObject::VisualObject()
{

}

VisualObject::VisualObject(std::string name, std::vector<Vertex> verticesVector)
    : mVertices {verticesVector}, mName {name}
{

}

void VisualObject::init()
{
    initializeOpenGLFunctions();

    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    //Vertex Buffer Object to hold vertices - VBO
    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof( Vertex ), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0  );          // array buffer offset
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void VisualObject::draw()
{
    initializeOpenGLFunctions();
    glBindVertexArray( mVAO );
    glDrawArrays(mDrawMethod, 0, mVertices.size());

    glBindVertexArray(0);
}



void VisualObject::move(float x, float y, float z)
{
    mMatrix.translate(x, y, z);
}

QVector3D VisualObject::getPosition()
{
    QVector4D position = mPosition.column(3);
    return QVector3D{position.x(), position.y(), position.z()};
}

void VisualObject::setPosition(const QVector3D positionToSet)
{
        mx = positionToSet.x();
        my = positionToSet.y();
        mz = positionToSet.z();
        mPosition.setToIdentity();
        mPosition.translate(positionToSet.x(), positionToSet.y(), positionToSet.z());
        updateMatrix();
}

void VisualObject::updateMatrix()
{
    mMatrix = mPosition * mRotation * mScale;
}

void VisualObject::setMonoColor(QVector3D colorToSet)
{
    for(auto i = mVertices.begin(); i != mVertices.end(); i++)
    {
        i->setR(colorToSet.x());
        i->setG(colorToSet.y());
        i->setB(colorToSet.z());
    }
}

void VisualObject::setName(std::string name)
{
    mName = name;
}

std::string VisualObject::getName()
{
    return mName;
}

void VisualObject::toggleEditorMode()
{
    bEditorMode = !bEditorMode;
    if (bEditorOnlyRender)
    {
        if(bEditorMode == true)
        {
            bShouldBeRendered = true;
        }
        else if (bEditorMode == false)
        {
            bShouldBeRendered = false;
        }
    }
}

QVector3D VisualObject::Barycentric(QVector3D object, QVector3D P1, QVector3D P2, QVector3D P3)
{
    P1.setZ(0);
    P2.setZ(0);
    P3.setZ(0);

    QVector3D P12 = (P2-P1);
    QVector3D P13 = (P3-P1);

    QVector3D n = QVector3D::crossProduct(P12, P13);
    float areal_123 = n.length();

    QVector3D baryc;

    QVector3D p = P2 - object;
    QVector3D q = P3 - object;
    n = QVector3D::crossProduct(p,q);
    baryc.setX(n.z()/areal_123);

    p = P3 - object;
    q = P1 - object;
    n = QVector3D::crossProduct(p,q);
    baryc.setY(n.z()/areal_123);

    p = P1 - object;
    q = P2 - object;
    n = QVector3D::crossProduct(p,q);
    baryc.setZ(n.z()/areal_123);

    return baryc;
}

bool VisualObject::isOverlappingTriangle(QVector3D baryc, QVector3D P1, QVector3D P2, QVector3D P3)
{
    return baryc.x() >= 0.f && baryc.y() >= 0.f && baryc.z() >= 0.f;
}

float VisualObject::GetBarycentricHeight(QVector3D baryc, QVector3D P1, QVector3D P2, QVector3D P3)
{
    return P1.z() * baryc.x() + P2.z() * baryc.y() + P3.z() * baryc.z();
}
