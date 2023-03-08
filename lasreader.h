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
    double MaxX{0};
    double MinX{0};
    double MaxY{0};
    double MinY{0};
    double MaxZ{0};
    double MinZ{0};

    unsigned long long NumberOfPoints{0};
};

struct XYZCoords
{
    double X {0};
    double Y {0};
    double Z {0};
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

    static double GetHeight(std::pair<int, float> AverageHeights);

    template<typename T>
    static void WriteToFile(std::string FileName, const std::vector<T> &InVector);
};

#endif // LASREADER_H
