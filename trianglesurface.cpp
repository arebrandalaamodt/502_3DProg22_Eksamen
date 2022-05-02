//#include "trianglesurface.h"

//TriangleSurface::TriangleSurface()
//{
//    //        x   y   z    r g b
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

//    mMatrix.setToIdentity();
//}

//TriangleSurface::TriangleSurface(std::string fileName)
//{
//    CreateGraphVertices();
//    WriteToFile(fileName);

//    mVertices.clear();

//    ReadFromFile(fileName);
//    mMatrix.setToIdentity();
//}

//TriangleSurface::~TriangleSurface() {}

//void TriangleSurface::init(GLint matrixUniform)
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

//void TriangleSurface::draw()
//{
//    glBindVertexArray( mVAO );
//    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
//    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
//}

//void TriangleSurface::WriteToFile(std::string fileName)
//{
////~~~~~~~~~~ Incremental File Names = x_fileName
////          Reading counterFile to create new filename & increment counterFile (counter.txt)
//    if (bIncrementFileName)
//    {
//        int counter{0};
//        std::fstream counterFile;
//        std::string counterString{0};
//        counterFile.open("../3Dprog22/txt_files/counter.txt", std::ios::in);
//        if (counterFile.is_open())
//        {
//            std::cout << "CounterFile read correctly" << std::endl;
//            counterFile >> counter;
//            counterFile.close();
//        }
//        counterFile.open("../3Dprog22/txt_files/counter.txt", std::ios::out);
//        if (counterFile.is_open())
//        {
//            std::cout << "CounterFile incremented correctly" << std::endl;
//            counterFile << (counter+1);
//            counterFile.close();
//        }
//        if(counter != 0)
//        {
//            counterString = std::to_string(counter);
//        }
//        else
//        {
//            std::cout << "CounterFile = 0" << std::endl;
//        }
//        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//        std::ofstream file;
//        std::string combinedFileName = counterString + "_" + fileName;
//        file.open("../3Dprog22/txt_files/"+combinedFileName);
//        if(file.is_open())
//        {
//            std::cout << "Write File open" << std::endl;
//            file << mVertices.size() << std::endl;
//            for(auto i = 0; i < mVertices.size(); i++)
//            {
//                file << mVertices[i] << std::endl;
//            }
//            file.close();
//        }
//    }
//    else
//    {
//        std::ofstream file;
//        file.open("../3Dprog22/txt_files/"+fileName);
//        if(file.is_open())
//        {
//            std::cout << "Write File open" << std::endl;
//            file << mVertices.size() << std::endl;
//            for(auto i = 0; i < mVertices.size(); i++)
//            {
//                file << mVertices[i] << std::endl;
//            }
//            file.close();
//        }
//    }
//}

//void TriangleSurface::ReadFromFile(std::string fileName)
//{
//    if (bIncrementFileName)
//    {
//    //~~~~~~~~~~ Reading counterFile to find and read correct iteration(x) of file (x_fileName.txt)
//        int counter{0};
//        std::fstream counterFile;
//        std::string counterString{0};
//        counterFile.open("../3Dprog22/txt_files/counter.txt", std::ios::in);
//        if (counterFile.is_open())
//        {
//            std::cout << "CounterFile read correctly" << std::endl;
//            counterFile >> counter;
//            counterFile.close();
//            counter--;
//        }
//        if(counter != 0)
//        {
//            counterString = std::to_string(counter);
//        }
//        else
//        {
//            std::cout << "CounterFile = 0" << std::endl;
//        }
//    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//        std::ifstream file;
//        std::string combinedFileName = counterString + "_" + fileName;
//        file.open("../3Dprog22/txt_files/"+combinedFileName);

//        if (file.is_open())
//        {
//            std::cout << "Read file open" << std::endl;
//            int x{0};
//            Vertex localVertex;
//            file >> x;                          // First line assigned to x
//            mVertices.reserve(x);               // Reserves space for x number of vertices
//            for (int i=0; i<x; i++)
//            {
//                 file >> localVertex;
//                 mVertices.push_back(localVertex);
//    //             std::cout << "localVertex:" << localVertex << std::endl;
//            }
//            file.close();
//        }
//        else
//        {
//            std::cout << "Read file failed" << std::endl;
//        }
//    }
//    else
//    {
//        std::ifstream file;
//        file.open("../3Dprog22/txt_files/"+fileName);

//        if (file.is_open())
//        {
//            std::cout << "Read file open" << std::endl;
//            int x{0};
//            Vertex localVertex;
//            file >> x;                          // First line assigned to x
//            mVertices.reserve(x);               // Reserves space for x number of vertices
//            for (int i=0; i<x; i++)
//            {
//                 file >> localVertex;
//                 mVertices.push_back(localVertex);
//    //             std::cout << "localVertex:" << localVertex << std::endl;
//            }
//            file.close();
//        }
//        else
//        {
//            std::cout << "Read file failed" << std::endl;
//        }
//    }
//}

//float TriangleSurface::MultivariableFunction(float x, float y)
//{
//    //~~
//    // Chosen function
//    // f(x,y) = x - x^3 + y^2
//    //~~//
//    float z = x - pow(x,3) + pow(y,2);


////    return z * 1.5;  // * 2.5 gives more zoomed out view?
//    return z;  // * 2.5 gives more zoomed out view?
//}

//void TriangleSurface::CreateGraphVertices()
//{
////    float xmin = -1.0f, xmax = 1.0f, ymin = -1.0f, ymax = 1.0f, h = 0.025f; // The size to draw // x1
//    float xmin = 0.0f, xmax = 1.0f, ymin = 0.0f, ymax = 1.0f, h = 0.25f; // The size to draw // x1
//    for (auto x=xmin; x<xmax; x+=h)
//        for (auto y=ymin; y<ymax; y+=h)
//        {
//            float z = MultivariableFunction(x,y);
//            mVertices.push_back(Vertex{x,y,z,x,y,z,0,0});

//            z = MultivariableFunction(x+h,y);
//            mVertices.push_back(Vertex{x+h,y,z,x,y,z,0,0});

//            z = MultivariableFunction(x,y+h);
//            mVertices.push_back(Vertex{x,y+h,z,x,y,z,0,0});

//            mVertices.push_back(Vertex{x,y+h,z,x,y,z,0,0});

//            z = MultivariableFunction(x+h,y);
//            mVertices.push_back(Vertex{x+h,y,z,x,y,z,0,0});

//            z = MultivariableFunction(x+h,y+h);
//            mVertices.push_back(Vertex{x+h,y+h,z,x,y,z,0,0});
//        }
//}

