
//#include <vector>

#include "vertex.h"
#include "heightmap.h"

Heightmap::Heightmap(QString fileName, float heightMultiplier, float scalePerStep, int pixelsPerStep)
    : mScalePerStep(scalePerStep), mHeightMultiplier(heightMultiplier), mPixelsPerStep(pixelsPerStep)
{
    setVertices(CreateArraysFromHeightmap(fileName, heightMultiplier, scalePerStep, pixelsPerStep));
}




std::vector<Vertex> Heightmap::CreateArraysFromHeightmap(const QString &filepath, float multiplier, float scalePerStep, int pixelsPerStep)
{

    QImage img(filepath);
    mImg = img;

    std::vector<Vertex>mVertices;
//    std::cout << pixelsPerStep << " " << mImg.height() << std::endl;

    for (int r = mImg.height()-pixelsPerStep; r > 0; r -= pixelsPerStep)
    {
        for (int c = 0; c < mImg.width()-pixelsPerStep; c += pixelsPerStep)
        {
            // top left
            QVector3D vecTopLeft = {float(c) * scalePerStep, float(mImg.height()-r) * scalePerStep, (1-mImg.pixelColor(c, r).lightnessF()) * multiplier};

            // top right
            QVector3D vecTopRight = {float(c + pixelsPerStep) * scalePerStep, float(mImg.height()-r) * scalePerStep, (1-mImg.pixelColor(c+pixelsPerStep, r).lightnessF()) * multiplier};

            // bottom left
            QVector3D vecBottomLeft = {float(c) * scalePerStep, float(mImg.height()-r+pixelsPerStep) * scalePerStep, (1-mImg.pixelColor(c, r-pixelsPerStep).lightnessF()) * multiplier};

            // bottom right
            QVector3D vecBottomRight = {float(c+pixelsPerStep) * scalePerStep, float(mImg.height()-r+pixelsPerStep) * scalePerStep, (1-mImg.pixelColor(c+pixelsPerStep, r-pixelsPerStep).lightnessF()) * multiplier};

            QVector3D tri1normal = QVector3D::crossProduct(vecTopLeft-vecTopRight, vecTopLeft-vecBottomLeft);

            QVector3D tri2normal = QVector3D::crossProduct(vecBottomRight-vecBottomLeft, vecBottomRight-vecTopLeft);


            UV uvs = CalculateUV(0, mImg.width() * scalePerStep,0, mImg.height() * scalePerStep,  vecTopLeft.x(), vecTopLeft.y());

            mVertices.push_back(Vertex{vecTopLeft, tri1normal, uvs.u, uvs.v});
            uvs = CalculateUV(0, mImg.width() * scalePerStep,0, mImg.height() * scalePerStep,  vecBottomLeft.x(), vecBottomLeft.y());
            mVertices.push_back(Vertex{vecBottomLeft, tri1normal, uvs.u, uvs.v});
            uvs = CalculateUV(0, mImg.width() * scalePerStep,0, mImg.height() * scalePerStep,  vecTopRight.x(), vecTopRight.y());
            mVertices.push_back(Vertex{vecTopRight, tri1normal, uvs.u, uvs.v});

            mVertices.push_back(Vertex{vecTopRight, tri2normal, uvs.u, uvs.v});
            uvs = CalculateUV(0, mImg.width() * scalePerStep,0, mImg.height() * scalePerStep,  vecBottomLeft.x(), vecBottomLeft.y());
            mVertices.push_back(Vertex{vecBottomLeft, tri2normal, uvs.u, uvs.v});
            uvs = CalculateUV(0, mImg.width() * scalePerStep,0, mImg.height() * scalePerStep,  vecBottomRight.x(), vecBottomRight.y());
            mVertices.push_back(Vertex{vecBottomRight, tri2normal, uvs.u, uvs.v});

        }
    }
    return mVertices;
}

UV Heightmap::CalculateUV(float xMin, float xMax, float yMin, float yMax, float x, float y)
{
    UV outUV;

    float xRange = xMax - xMin;
    float xPos = x - xMin;
    outUV.u = xPos / xRange;
    float yRange = yMax - yMin;
    float yPos = y - yMin;
    outUV.v = yPos / yRange;

    return outUV;
}


float Heightmap::GetHeight(float x, float y)
{
    int xInt = x / mScalePerStep;
    int yInt = mImg.height() - y / mScalePerStep;
    float height = 0;
    if (xInt + 2 > mImg.width())
    {
        xInt = mImg.width() - 2;
    }
        if (yInt + 2 > mImg.height())
        {
            yInt = mImg.height() - 2;
        }
    if (xInt < 0) xInt = 0;
    if (yInt < 0) yInt = 0;

    QVector3D p1{float((xInt) * mScalePerStep), float((mImg.height() - yInt -1) * mScalePerStep), (1-mImg.pixelColor(xInt, yInt + 1).lightnessF()) * mHeightMultiplier};
    QVector3D p2{float((xInt + 1) * mScalePerStep), float((mImg.height() - yInt - 1) * mScalePerStep), (1-mImg.pixelColor(xInt + 1, yInt + 1).lightnessF()) * mHeightMultiplier};
    QVector3D p3{float((xInt) * mScalePerStep), float((mImg.height() - yInt) * mScalePerStep), (1-mImg.pixelColor(xInt, yInt).lightnessF()) * mHeightMultiplier};

    QVector3D baryc = Barycentric({x, y, 0}, p1, p2, p3);
    if (isOverlappingTriangle(baryc, p1, p2, p3)) {
        height = GetBarycentricHeight(baryc, p1, p2, p3);
        mCurrentNormal = QVector3D::crossProduct((p2-p1), (p3-p2));
    } else {
        p1 = {float((xInt+1) * mScalePerStep), float((mImg.height() - yInt) * mScalePerStep), (1-mImg.pixelColor(xInt + 1, yInt).lightnessF()) * mHeightMultiplier};
        // 2 1 3
        baryc = Barycentric({x, y, 0}, p2, p1, p3);
        height = GetBarycentricHeight(baryc, p2, p1, p3);
        mCurrentNormal = QVector3D::crossProduct((p1-p2), (p3-p1));
    }
    return height;

}

