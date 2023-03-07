#include "lasreader.h"

#include "qdebug.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "vertex.h"

PointCloudMesh LASReader::GenerateVerticesFromFile(std::string FileName, int Resolution, float Size)
{

}


bool LASReader::ReadFromFile(std::string FileName, PointCloudMesh& InPointCloudMesh)
{

    if(false == ReadTemplate<Vertex>(FileName.substr(0, FileName.find(".txt")) + "-Vertices.txt", InPointCloudMesh.Vertices))
    {
        return false;
    }

    if(false == ReadTemplate<Triangle>(FileName.substr(0, FileName.find(".txt")) + "-Triangles.txt", InPointCloudMesh.IndicesAndNeighbors))
    {
        return false;
    }

    std::vector<float> FloatVector;

    if(false == ReadTemplate<float>(FileName.substr(0, FileName.find(".txt")) + "-Scale.txt", FloatVector))
    {
        return false;
    }

    InPointCloudMesh.Scale = FloatVector.at(0);

    return true;
}

template <typename T>
bool LASReader::ReadTemplate (std::string FileName, std::vector<T>& Vector)
{
    std::ifstream File;
    File.open("../3DProg22/triangulation_files/"+FileName);

    if (File.is_open())
    {
        int X {0};
        T Input;
        File >> X;
        for (int i = 0; i < X; ++i)
        {
            File >> Input;
            Vector.push_back(Input);
        }
        File.close();
    }
    else
    {
        qDebug() << __FUNCTION__ "Failed to open file.";
        return false;
    }

    return true;
}
