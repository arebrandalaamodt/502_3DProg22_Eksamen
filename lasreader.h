#ifndef LASREADER_H
#define LASREADER_H

#include <vector>
#include "vertex.h"


struct Triangle
{
    int Indeces[3] {0,0,0};
    int Neighbors[3] {0,0,0};
};

struct MinAndMax
{
    float MaxX{0};
    float MinX{0};
    float MaxY{0};
    float MinY{0};
    float MaxZ{0};
    float MinZ{0};

    unsigned int NumberOfPoint{0};
};

struct XYZCoords
{
    int X {0};
    int Y {0};
    int Z {0};
};

struct PointCloud
{
    std::vector<XYZCoords> Points;
    MinAndMax MinMaxPointCloud;
};

struct PointCloudMesh
{
    std::vector<Vertex> Vertices;
    std::vector<Triangle> IndicesAndNeighbors;
    float Scale {1.0f};
};


class LASReader
{
public:

    static PointCloudMesh GenerateVerticesFromFile(std::string FileName, int Resolution, float Size = 10.0f);



};

#endif // LASREADER_H
