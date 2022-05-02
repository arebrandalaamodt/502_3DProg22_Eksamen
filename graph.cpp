//#include "graph.h"

//Graph::Graph()
//{
//    createGraphVertices();
//}

//void Graph::init(GLint matrixUniform)
//{
//    mMatrixUniform = matrixUniform;
//    initializeOpenGLFunctions();

//    glGenVertexArrays( 1, &mVAO );
//    glBindVertexArray( mVAO );

//    glGenBuffers( 1, &mVBO );
//    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

//    glBufferData(
//                GL_ARRAY_BUFFER,                          //what buffer type
//                mVertices.size() * sizeof( Vertex ),      //how big buffer do we need
//                mVertices.data(),                         //the actual vertices
//                GL_STATIC_DRAW                            //should the buffer be updated on the GPU
//            );

//    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
//    glVertexAttribPointer(
//                0,
//                3,
//                GL_FLOAT,
//                GL_FALSE,
//                sizeof(Vertex),
//                reinterpret_cast<GLvoid*>(0)
//            );                              // array buffer offset
//    glEnableVertexAttribArray(0);

//    glVertexAttribPointer(
//                1,
//                3,
//                GL_FLOAT,
//                GL_FALSE,
//                sizeof(Vertex),
//                reinterpret_cast<GLvoid*>((3 * sizeof(GLfloat)))
//                );
//    glEnableVertexAttribArray(1);

//    glBindVertexArray(0); //Releases binds

//}

//void Graph::draw()
//{
//    glBindVertexArray( mVAO );
//    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());

////    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
////    glDrawArrays(GL_LINE_STRIP, 0, mVertices.size());
//    glDrawArrays(GL_LINES, 0, mVertices.size());
//}

//float Graph::function(float x)
//{
//    float y {0};
////    y = 0.86248 * pow(x,2) - 0.02364 * x + 0.34454;

//    y = ((6836.f/13209.f)*pow(x, 3)) - ((107987.f/22015.f)*pow(x, 2)) + ((364691.f/26418.f) * x) - (52943.f/6290.f);
////    y = 0.51752f*pow(x, 3) - 4.90515f*pow(x, 2) + 13.80464f * x - 8.41701f;

////    std::cout << "y = " << y << std::endl;
//    return y;
//}

//void Graph::createGraphVertices()
//{
//    float xmin = 0.f, xmax = 10.f, h = 0.25f; // The size to draw // x1
//    for (auto x = xmin; x < xmax; x += h)
//    {
//        float y = function(x);
//        mVertices.push_back(Vertex{x    ,y  ,1  ,0  ,0}); // Top left
//        y = function(x+h);
//        mVertices.push_back(Vertex{x+h  ,y  ,1  ,0  ,0}); // Top Right
//        mVertices.push_back(Vertex{x    ,0  ,1  ,0  ,0}); // Bot Left
//        mVertices.push_back(Vertex{x    ,0  ,0  ,1  ,0}); // Bot Left, start of 2nd triangle
//        y = function(x+h);
//        mVertices.push_back(Vertex{x+h  ,y  ,1  ,0  ,0}); // Top Right, 2nd triangle
//        mVertices.push_back(Vertex{x+h  ,0  ,1  ,0  ,0}); // Bot Right, 2nd triangle
//    }
//}
