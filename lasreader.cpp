#include "lasreader.h"

#include "qdebug.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "vertex.h"

PointCloudMesh LASReader::GenerateVerticesFromFile(std::string FileName, int Resolution, float Size)
{
    PointCloudMesh OutPointCloudMesh;
    PointCloud PointCloud;

    std::ifstream file;
    file.open("../3DProg22/triangulation_files/"+FileName);

    if (file.is_open())
    {
        qDebug() << __FUNCTION__ " | Reading file...: ";

        double x,y,z;
        file >> x >> y >> z;
        PointCloud.MinMaxPointCloud.MaxX = x;
        PointCloud.MinMaxPointCloud.MaxY = y;
        PointCloud.MinMaxPointCloud.MaxZ = z;
        PointCloud.MinMaxPointCloud.MinX = x;
        PointCloud.MinMaxPointCloud.MinY = y;
        PointCloud.MinMaxPointCloud.MinZ = z;

        PointCloud.MinMaxPointCloud.NumberOfPoints++;

        while (!file.eof())
        {
             file >> x >> y >> z;
             if (x > PointCloud.MinMaxPointCloud.MaxX) PointCloud.MinMaxPointCloud.MaxX = x;
             else if (x < PointCloud.MinMaxPointCloud.MinX) PointCloud.MinMaxPointCloud.MinX = x;
             if (y > PointCloud.MinMaxPointCloud.MaxY) PointCloud.MinMaxPointCloud.MaxY = y;
             else if (y < PointCloud.MinMaxPointCloud.MinY) PointCloud.MinMaxPointCloud.MinY = y;
             if (z < PointCloud.MinMaxPointCloud.MinZ) PointCloud.MinMaxPointCloud.MinZ = z;
             else if (z > PointCloud.MinMaxPointCloud.MaxZ) PointCloud.MinMaxPointCloud.MaxZ = z;
             PointCloud.MinMaxPointCloud.NumberOfPoints++;
        }

        file.close();

//        qDebug() << __FUNCTION__ "MaxX: " << PointCloud.MinMaxPointCloud.MaxX;
//        qDebug() << __FUNCTION__ "MaxY: " << PointCloud.MinMaxPointCloud.MaxY;
//        qDebug() << __FUNCTION__ "MaxZ: " << PointCloud.MinMaxPointCloud.MaxZ;
//        qDebug() << __FUNCTION__ "MinX: " << PointCloud.MinMaxPointCloud.MinX;
//        qDebug() << __FUNCTION__ "MinY: " << PointCloud.MinMaxPointCloud.MinY;
//        qDebug() << __FUNCTION__ "MinZ: " << PointCloud.MinMaxPointCloud.MinZ;


        PointCloud.Points.reserve(PointCloud.MinMaxPointCloud.NumberOfPoints);

        float step = Size / (float)Resolution;

        float span = std::max(PointCloud.MinMaxPointCloud.MaxX - PointCloud.MinMaxPointCloud.MinX, PointCloud.MinMaxPointCloud.MaxY - PointCloud.MinMaxPointCloud.MinY);

        span += 0.001f;

        std::vector<std::vector<std::pair<int, float>>> averageHeights(Resolution, std::vector<std::pair<int, float>>(Resolution));

        file.open("../3DProg22/triangulation_files/"+FileName);
        while (!file.eof())
        {
            file >> x >> y >> z;

            float normalisedx = x - PointCloud.MinMaxPointCloud.MinX;
            float normalisedy = y - PointCloud.MinMaxPointCloud.MinY;
            float normalisedz = z - PointCloud.MinMaxPointCloud.MinZ;

//            qDebug() << __FUNCTION__ " | Normalized Z: " << normalisedz;
//            qDebug() << __FUNCTION__ " | PointCloud.MinMaxPointCloud.MinZ: " << PointCloud.MinMaxPointCloud.MinZ;

            int xPos, yPos;
            xPos = (normalisedx / span) * Resolution;
            yPos = (normalisedy / span) * Resolution;

            averageHeights[xPos][yPos].first++;
            averageHeights[xPos][yPos].second += normalisedz;
        }

        float ScaleDifference = Size / span;
        qDebug() << __FUNCTION__ " | ScaleDifference: " << ScaleDifference;
        OutPointCloudMesh.Scale = ScaleDifference;

        qDebug() << __FUNCTION__ " | Creating Mesh..." << ScaleDifference;

        OutPointCloudMesh.IndicesAndNeighbors.reserve(Resolution * Resolution * 2);

        for (int y = 0; y * step < Size; y++) {
            for (int x = 0; x * step < Size; x++) {
                float height = GetHeight(averageHeights[x][y]);

                if (x == 0 || y == 0) {
                    OutPointCloudMesh.Vertices.push_back(Vertex(x * step, y * step, height * ScaleDifference, 0,0,1, ((float)x / (float)Resolution), ((float)y / (float)Resolution)));
                } else {

                    QVector3D adjacentVert1{(x-1)*step, y*step, (float)GetHeight(averageHeights[x-1][y]) * ScaleDifference};
                    QVector3D adjacentVert2{x*step, (y-1)*step, (float)GetHeight(averageHeights[x][y-1]) * ScaleDifference};
                    QVector3D mypos{x * step, y * step, height * ScaleDifference};

                    QVector3D tri2normal = QVector3D::crossProduct(adjacentVert1-mypos, adjacentVert2-mypos);

                    OutPointCloudMesh.Vertices.push_back(Vertex(x * step, y * step, height * ScaleDifference, tri2normal.x(),tri2normal.y(),tri2normal.z(),  ((float)x / (float)Resolution), ((float)y / (float)Resolution)));
                }

                if (x == Resolution-1 || y == Resolution-1) {
                    continue;
                }
                int currentTriangleIndex = (Resolution - 1) * y * 2 + (2 * x);
                Triangle tri;
                tri.Indices[0] = y*Resolution+x;
                tri.Indices[1] = y*Resolution+x+1;
                tri.Indices[2] = (y+1)*Resolution+x;



                tri.Neighbors[0] = currentTriangleIndex + 1;
                tri.Neighbors[1] = currentTriangleIndex - 1;
                tri.Neighbors[2] = currentTriangleIndex - (2 * (Resolution-1)) + 1;

                currentTriangleIndex +=1;

                Triangle tri2;
                tri2.Indices[0] = (y+1)*Resolution+x;
                tri2.Indices[1] = y*Resolution+x+1;
                tri2.Indices[2] = (y+1)*Resolution+x+1;

                tri2.Neighbors[0] = currentTriangleIndex + 1;
                tri2.Neighbors[1] = currentTriangleIndex + ((Resolution-1) * 2) - 1;
                tri2.Neighbors[2] = currentTriangleIndex - 1;

                if (y == 0)
                {
                    tri.Neighbors[2] = -1;
                }
                if (x == 0)
                {
                    tri.Neighbors[1] = -1;
                }
                if (y == Resolution-2)
                {
                    tri2.Neighbors[1] = -1;
                }
                if (x == Resolution-2)
                {
                    tri2.Neighbors[0] = -1;
                }


                OutPointCloudMesh.IndicesAndNeighbors.push_back(tri);
                OutPointCloudMesh.IndicesAndNeighbors.push_back(tri2);
            }
        }

        file.close();

        WriteToFile<Vertex>(FileName.substr(0, FileName.find(".txt")) + "-Vertices.txt", OutPointCloudMesh.Vertices);
        WriteToFile<Triangle>(FileName.substr(0, FileName.find(".txt")) + "-Triangles.txt", OutPointCloudMesh.IndicesAndNeighbors);
        WriteToFile<float>(FileName.substr(0, FileName.find(".txt")) + "-Scale.txt", std::vector<float>{OutPointCloudMesh.Scale});
    }
    else
    {
        qDebug() << __FUNCTION__ " | Failed to open file." << PointCloud.MinMaxPointCloud.MaxX;
    }

    return OutPointCloudMesh;
}


bool LASReader::ReadFromFile(std::string FileName, PointCloudMesh& InPointCloudMesh)
{

    if(false == ReadTemplate<Vertex>(FileName.substr(0, FileName.find(".txt")) + "-Vertices.txt", InPointCloudMesh.Vertices))
    {
        qDebug() << __FUNCTION__ " | Vertices.txt not found!";
        return false;
    }

    if(false == ReadTemplate<Triangle>(FileName.substr(0, FileName.find(".txt")) + "-Triangles.txt", InPointCloudMesh.IndicesAndNeighbors))
    {
        qDebug() << __FUNCTION__ " | Triangles.txt not found!";
        return false;
    }

    std::vector<float> FloatVector;

    if(false == ReadTemplate<float>(FileName.substr(0, FileName.find(".txt")) + "-Scale.txt", FloatVector))
    {
        qDebug() << __FUNCTION__ " | Scale.txt not found!";
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
        qDebug() << __FUNCTION__ "| Failed to open file.";
        return false;
    }

    return true;
}



double LASReader::GetHeight(std::pair<int, float> AverageHeights)
{
    if (AverageHeights.first == 0)
    {
        return 0;
    }
    else
    {
        return AverageHeights.second / (float)AverageHeights.first;
    }
}

template<typename T>
void LASReader::WriteToFile(std::string FileName, const std::vector<T> &InVector)
{
    std::ofstream File;
    File.open("../3DProg22/triangulation_files/"+FileName);
    if(File.is_open())
    {
        File << InVector.size() << std::endl;
        for(unsigned int i = 0; i < InVector.size(); i++)
        {
            File << InVector[i] << std::endl;
        }
        File.close();
    }
    else
    {
        qDebug() << __FUNCTION__ " | Could not open file.";
    }
}
