#ifndef OBJFILELOADER_H
#define OBJFILELOADER_H

#include "vertex.h"
#include "iostream"
#include "fstream"
#include "sstream"

class OBJFileLoader
{
private:
static std::vector<Vertex> loadOBJ(const char* fileName, const QVector3D& color)
    {
        std::vector<QVector3D> vertexPos;
        std::vector<QVector2D> vertexTextCoord;
        std::vector<QVector3D> vertexNormal;

        std::vector<int> vertexPositionIndicies;
        std::vector<int> vertexNormalIndicies;
        std::vector<int> vertexTextCoordIndicies;

        std::vector<Vertex> verticesOut;

        std::stringstream ss;
        std::ifstream inFile(fileName);
        std::string line = "";
        std::string prefix = "";

        QVector3D tempVec3;
        QVector2D tempVec2;
        int tempInt;

        if (!inFile.is_open())
        {
            throw "Could not load obj file.";
        }

        while (std::getline(inFile, line))
        {
            ss.clear();
            ss.str(line);
            ss >> prefix;

            if (prefix == "v")
            {
                float x, y, z;
                ss >> x >> y >> z;
                tempVec3 = {x, y, z};
                vertexPos.push_back(tempVec3);
            }
            else if (prefix == "vt")
            {
                float x, y;
                ss >> x >> y;
                tempVec2 = {x, y};
                vertexTextCoord.push_back(tempVec2);
            }
            else if (prefix == "vn")
            {
                float x, y, z;
                ss >> x >> y >> z;
                tempVec3 = {x, y, z};
                vertexNormal.push_back(tempVec3);
            }
            else if (prefix == "f")
            {
                int counter = 0;
                while (ss >> tempInt)
                {
                    if (counter == 0)
                    {
                        vertexPositionIndicies.push_back(tempInt);
                    }
                    else if (counter == 1)
                    {
                        vertexTextCoordIndicies.push_back(tempInt);
                    }
                    else if (counter == 2)
                    {
                       vertexNormalIndicies.push_back(tempInt);
                    }

                    if (ss.peek() == '/')
                    {
                        counter++;
                        ss.ignore(1, '/');
                    }
                    else if (ss.peek() == ' ')
                    {
                        counter++;
                        ss.ignore(1, ' ');
                    }

                    if (counter > 2) {
                        counter = 0;
                    }
                }
            }
        }

        verticesOut.reserve(vertexPositionIndicies.size());

        for (int i = 0; i < vertexPositionIndicies.size(); i++)
        {
            QVector3D position = vertexPos[vertexPositionIndicies[i]-1];
            QVector2D texcoord = vertexTextCoord[vertexTextCoordIndicies[i]-1];
            QVector3D normal = vertexNormal[vertexNormalIndicies[i]-1];
            if (color.x() == 0 && color.y() == 0 && color.z() == 0)
            {
                Vertex vert(position, position.normalized().x(), position.normalized().y(), position.normalized().z());
                verticesOut.push_back(vert);
            }
            else
            {
                Vertex vert(position, color.x(), color.y(), color.z());
                verticesOut.push_back(vert);
            }
        }
        inFile.close();
        return verticesOut;
    }

    friend class MeshGenerator;
};



#endif // OBJFILELOADER_H
