#ifndef TEXTFILEINTERFACE_H
#define TEXTFILEINTERFACE_H


#include "vertex.h"

class TextFileInterface
{
//public:
//    TextFileInterface();



public:
    std::vector<Vertex> static VertexReadFromFile();
    void static VertexWriteToFile(std::vector<Vertex> vectorToFile, std::string fileName);


};

#endif // TEXTFILEINTERFACE_H
