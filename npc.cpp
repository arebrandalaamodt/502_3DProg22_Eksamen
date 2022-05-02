//#include "npc.h"
//#include "graph.h"
//#include <iostream>
//#include <iomanip>


//NPC::NPC()
//{
//    Vertex v0{0.0,0.0,0.0, 1,0,0,0,0};
//    mVertices.push_back(v0);
//    Vertex v1{0.5,0.0,0.0, 0,1,0,0,0};
//    mVertices.push_back(v1);
//    Vertex v2{0.5,0.5,0.0, 0,0,1,0,0};
//    mVertices.push_back(v2);
//    Vertex v3{0.0,0.0,0.0, 0,0,1,0,0};
//    mVertices.push_back(v3);
//    Vertex v4{0.5,0.5,0.0, 0,1,0,0,0};
//    mVertices.push_back(v4);
//    Vertex v5{0.0,0.5,0.0, 1,0,0,0,0};
//    mVertices.push_back(v5);

////    end = std::chrono::system_clock::now(); // this is the end point

//}

//void NPC::patrol(long pnnsPerDraw)
//{

//    msPerDraw = pnnsPerDraw/1000000.f;


////    std::cout << std::setprecision(9) << msPerDraw << std::endl;


//    if (bGoingRight)
//    {
//        xPos += msPerDraw;
//        yPos = Graph::function(xPos);

//        if (xPos > 5.0f)
//           {
//               bGoingRight = false;
//               xPos = 5.0f;
//           }
//    }
//    else if (!bGoingRight)
//    {
//        xPos -= msPerDraw;
//        yPos = Graph::function(xPos);

//        if (xPos < -5.0f)
//        {
//            bGoingRight = true;
//            xPos = -5.0f;
//        }
//    }

//    this->setPosition(xPos, yPos, zPos);




////    std::cout << std::setprecision(9) << xPos << std::endl;


////    std::chrono::time_point<std::chrono::system_clock> end;
////    std::chrono::milliseconds ms(5000);

////    end = std::chrono::system_clock::now() + ms; // this is the end point

////    while(std::chrono::system_clock::now() < end) // still less than the end?
////    {

////        qDebug() << "Hello";
////    }
//}

////void NPC::setPosition(float x, float y, float z)
////{
//////    mMatrix.translate(0,0,0);
//////    mMatrix.
////}

//void NPC::init22222222(GLint matrixUniform)
//{
////    mMatrixUniform = matrixUniform;
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

//void NPC::draw()
//{
//    glBindVertexArray( mVAO );
//    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
//    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
//}
