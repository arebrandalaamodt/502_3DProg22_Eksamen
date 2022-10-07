#include <iomanip>

#include "regularsurface.h"

RegularSurface::RegularSurface()
{
    std::cout << "RegularSurface::RegularSurface() | Default Constructor" << std::endl;
}

RegularSurface::RegularSurface(std::string name, std::vector<Vertex> verticesVector)
    : VisualObject(name, verticesVector)
{
    std::cout << "RegularSurface::RegularSurface(std::string name, std::vector<Vertex> verticesVector) | Constructor" << std::endl;
//    FullVerticesVector = verticesVector;
    setMinMaxValues(verticesVector);
    averageHeights(10.0000000000000000000000f, verticesVector);
}

void RegularSurface::setMinMaxValues(std::vector<Vertex> &verticesIn)
{

    mXMin = verticesIn[0].getX();
    mYMin = verticesIn[0].getY();
//    std::vector<Vertex>::iterator iter = verticesIn.begin();

//    int x{0};

    for(int i = 0; i < verticesIn.size(); i++)
    {
        if (verticesIn[i].getX() > mXMax)
        {
            mXMax = verticesIn[i].getX();
//            std::cout << "XMax Updated" << std::endl;
        }
        if (verticesIn[i].getY() > mYMax)
        {
            mYMax = verticesIn[i].getY();
//            std::cout << "YMax Updated" << std::endl;
        }
        if (verticesIn[i].getX() < mXMin)
        {
            mXMin = verticesIn[i].getX();
//            std::cout << "XMin Updated" << std::endl;
        }
        if (verticesIn[i].getY() < mYMin)
        {
            mYMin = verticesIn[i].getX();
//            std::cout << "YMin Updated" << std::endl;
        }

        std::cout << std::setprecision(2);
        std::cout << "Index: " << i << " | X: " << verticesIn[i].getX() << " | Y: " << verticesIn[i].getY();
        std::cout << std::setprecision(2);
        std::cout << " | Z: " << verticesIn[i].getZ()<< std::endl;

//        std::cout << std::setprecision(8);
//        std::cout << "Index: " << i << " | Y: " << verticesIn[i].getY()<< std::endl;
    }

    std::cout << "XMax = " << mXMax << std::endl;
    std::cout << "YMax = " << mYMax << std::endl;
    std::cout << "XMin = " << mXMin << std::endl;
    std::cout << "YMin = " << mYMin << std::endl;


    //    for(int i : verticesIn)


}

//std::vector<Vertex> RegularSurface::averageHeights(float gridResolution, std::vector<Vertex> &verticesIn)
void RegularSurface::averageHeights(float gridResolution, std::vector<Vertex> &verticesIn)
{
//    std::cout << "Size of verticesIn: " << verticesIn.size() << std::endl;
    std::vector<Vertex> verticesOut;
    float incrementStep{gridResolution};
    std::cout << std::setprecision(8);


    for (float currentX = mXMin; currentX < mXMax; currentX += incrementStep)
    {
        std::cout << "currentX: " << currentX << std::endl;
        for (float currentY = mYMin; currentY < mYMax; currentY += incrementStep)
        {
            std::cout << "currentY: " << currentY << std::endl;

            int currentSquareCounter{0};
            float currentHeightTotal{0.f};
            float vertexHeight {0.f};
            for(int i = 0; i < verticesIn.size(); i++)
            {
//                std::cout << "i = " << i << std::endl;

//                float tempX = verticesIn[i].getX();
//                float tempY = verticesIn[i].getY();
//                float tempZ = verticesIn[i].getZ();

                if (verticesIn[i].getX() >= currentX && verticesIn[i].getX() < currentX+incrementStep)
                {
                    if (verticesIn[i].getY() >= currentY && verticesIn[i].getY() < currentY+incrementStep)
                    {
                        currentSquareCounter++;
                        currentHeightTotal += verticesIn[i].getZ();
                    }
                }
            }
            if (currentSquareCounter == 0)
            {
                vertexHeight = 0.f;
            }
            else
            {
                vertexHeight = (currentHeightTotal/currentSquareCounter);
            }
//            float vertX = (currentX+(incrementStep)/2);
//            float vertY = (currentY+(incrementStep)/2);
//            float vertZ = vertexHeight;
//            std::cout << "-----------------------------------------------" << std::endl;
//            std::cout << "currentSquareCounter: " << currentSquareCounter << std::endl;

//            std::cout << "vert X: " << (currentX+(incrementStep)/2) << std::endl;
//            std::cout << "vert Y: " << (currentY+(incrementStep)/2) << std::endl;
//            std::cout << "vert Z: " << vertexHeight << std::endl;
//            verticesOut.push_back (Vertex(((currentX+incrementStep)/2),((currentY+incrementStep)/2),vertexHeight, 1,1,1,1,1));
            verticesOut.push_back (Vertex((currentX+(incrementStep)/2),(currentY+(incrementStep)/2),vertexHeight,1,1,1,1,1));
        }
    }

//    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Size of verticesOut: " << verticesOut.size() << std::endl;


    for (int i = 0; i < verticesOut.size(); i++)
    {
//        std::cout << std::setprecision(3);
//        std::cout << "VerticesOut - Index: " << i << " | X: " << verticesOut[i].getX() << " | Y: " << verticesOut[i].getY() << " | Z: " << verticesOut[i].getZ() << std::endl;

        // Find how to step through
        // Find how to step through
        // Find how to step through
        // Find how to step through
        // Find how to step through
        // Find how to step through
        // Find how to step through
        // Find how to step through
    }



//    return verticesOut;
}

