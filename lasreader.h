#ifndef LASREADER_H
#define LASREADER_H

#include <vector>
#include <string>
#include "vertex.h"


struct Triangle
{
    int Indices[3];
    int Neighbors[3];


    friend std::ostream& operator<< (std::ostream& os, const Triangle& t)
    {
      os << std::fixed;
      os << t.Indices[0] << " " << t.Indices[1] << " " << t.Indices[2] << " ";
      os << t.Neighbors[0] << " " << t.Neighbors[1] << " " << t.Neighbors[2];
      return os;
    }
    friend std::istream& operator>> (std::istream& is, Triangle& t)
    {
        is >> t.Indices[0] >> t.Indices[1] >> t.Indices[2];
        is >> t.Neighbors[0] >> t.Neighbors[1] >> t.Neighbors[2];
      return is;
    }

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
    static bool ReadFromFile (std::string FileName, PointCloudMesh &InPointCloudMesh);


private:

    template<typename T>
    static bool ReadTemplate (std::string FileName, std::vector<T> &Vector);



};

#endif // LASREADER_H
