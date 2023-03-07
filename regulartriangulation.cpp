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

}

