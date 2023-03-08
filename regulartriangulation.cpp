#include "regulartriangulation.h"
#include "lasreader.h"
#include <QDebug>


RegularTriangulation::RegularTriangulation(std::string FileName)
{ 
    if (LASReader::ReadFromFile(FileName, PointCloud) == false)
    {
        qDebug() << "Generating triangulation vertices from file...";
        PointCloud = LASReader::GenerateVerticesFromFile(FileName, Resolution, Size);
    }

    Scale = PointCloud.Scale;
    mVertices = PointCloud.Vertices;
    mIndices.reserve(PointCloud.IndicesAndNeighbors.size() * 3);
    for (unsigned int i = 0; i < PointCloud.IndicesAndNeighbors.size(); ++i)
    {
        mIndices.push_back(PointCloud.IndicesAndNeighbors[i].Indices[0]);
        mIndices.push_back(PointCloud.IndicesAndNeighbors[i].Indices[1]);
        mIndices.push_back(PointCloud.IndicesAndNeighbors[i].Indices[2]);
    }

    qDebug() << __FUNCTION__ " | mIndices size = " << mIndices.size();

}

Triangle RegularTriangulation::GetTriangle(float x, float y)
{
    float SizeOfSquares = Size / Resolution;
    int ix, iy;
    ix = x / SizeOfSquares;
    iy = y / SizeOfSquares;
    if (ix >= Resolution-1) {
        ix = Resolution-2;
    }
    if (iy >= Resolution-1) {
        iy = Resolution-2;
    }

    Triangle tri;
    tri.Indices[0] = iy*Resolution+ix;
    tri.Indices[1] = iy*Resolution+ix+1;
    tri.Indices[2] = (iy+1)*Resolution+ix;

    QVector3D p1 = mVertices[tri.Indices[0]].GetXYZ();
    QVector3D p2 = mVertices[tri.Indices[1]].GetXYZ();
    QVector3D p3 = mVertices[tri.Indices[2]].GetXYZ();
    auto baryc = Barycentric({x,y,0}, p1, p2, p3);
    if (isOverlappingTriangle(baryc, p1, p2, p3))
    {
        return tri;
    }

    Triangle tri2;
    tri2.Indices[0] = (iy+1)*Resolution+ix;
    tri2.Indices[1] = iy*Resolution+ix+1;
    tri2.Indices[2] = (iy+1)*Resolution+ix+1;

    return tri2;
}

bool RegularTriangulation::CheckTriangleHeight(QVector3D p1, QVector3D p2, QVector3D p3, float h)
{
    float maxHeight = std::max(p1.z(), p2.z());
    maxHeight = std::max(maxHeight, p3.z());

    float minHeight = std::min(p1.z(), p2.z());
    minHeight = std::min(minHeight, p3.z());

    bool out = minHeight < h && maxHeight > h;
    return out;
}

bool RegularTriangulation::CheckTriangleHeight(int triIndex, float h)
{
    int i1 = PointCloud.IndicesAndNeighbors[triIndex].Indices[0];
    int i2 = PointCloud.IndicesAndNeighbors[triIndex].Indices[1];
    int i3 = PointCloud.IndicesAndNeighbors[triIndex].Indices[2];
    Vertex v1 = mVertices[i1];
    Vertex v2 = mVertices[i2];
    Vertex v3 = mVertices[i3];
    QVector3D p1 = v1.GetXYZ();
    QVector3D p2 = v2.GetXYZ();
    QVector3D p3 = v3.GetXYZ();

    return CheckTriangleHeight(p1,p2,p3,h);
}


void RegularTriangulation::init()
{
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData(
                GL_ARRAY_BUFFER,
                mVertices.size()*sizeof(Vertex),
                mVertices.data(),
                GL_STATIC_DRAW
                );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(
                0,
                3,
                GL_FLOAT,
                GL_FALSE,
                sizeof(Vertex),
                reinterpret_cast<const void*>(0)
                );
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(
                1,
                3,
                GL_FLOAT,
                GL_FALSE,
                sizeof(Vertex),
                reinterpret_cast<const void*>(3 * sizeof(GLfloat))
                );
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    //enable the matrixUniform
    // mMatrixUniform = glGetUniformLocation( matrixUniform, "matrix" );
    glGenBuffers(1, &mIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size()*sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void RegularTriangulation::draw()
{
    initializeOpenGLFunctions();
    glBindVertexArray( mVAO );
    // GL_FALSE for QMatrix4x4
    //glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mMatrix.constData());
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));//mVertices.size());
}
