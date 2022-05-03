// VisualObject.cpp
#include "visualobject.h"



VisualObject::VisualObject()
{
//    std::cout << "VisualObject::VisualObject()";

}

VisualObject::VisualObject(std::string name, std::vector<Vertex> verticesVector)
    : mVertices {verticesVector}, mName {name}
{

}

void VisualObject::init()
{
//    mMatrixUniform = matrixUniform;
    //must call this to use OpenGL functions
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
    //glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
//    glDrawElements(mDrawMethod, mVertices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));//mVertices.size());
    glDrawArrays(mDrawMethod, 0, mVertices.size());

    glBindVertexArray(0);
}

//VisualObject::~VisualObject() {
//   glDeleteVertexArrays( 1, &mVAO );
//   glDeleteBuffers( 1, &mVBO );
//}



void VisualObject::move(float x, float y, float z)
{
    mMatrix.translate(x, y, z);
}

QVector3D VisualObject::getPosition()
{
//    std::cout << "VisualObject::getPosition()";
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

//auto i = mObjects.begin(); i != mObjects.end(); i++)


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
