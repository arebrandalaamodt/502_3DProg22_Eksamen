#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <string>
#include <QImage>
#include <QString>
#include <QVector3D>
#include <vector>
#include "vertex.h"
#include "visualobject.h"

class Heightmap : public VisualObject
{
public:

    Heightmap();
//    Heightmap(QString fileName = "../3Dprog22/Assets/heightmap.bmp", float heightMultiplier=1, float scalePerStep=1, int pixelsPerStep=1);
    std::vector<Vertex> CreateArraysFromHeightmap(const QString &filepath, float multiplier, float scalePerStep, int pixelsPerStep);

    UV CalculateUV(float xMin, float xMax, float yMin, float yMax, float x, float y);
//    void init(GLint matrixUniform) override;
//    void draw() override;
    QImage mImg;
    float mScalePerStep;
    float mHeightMultiplier;
    int mPixelsPerStep;
    QVector3D mCurrentNormal;

//    DebugPoint* dbpt1{nullptr};
//    DebugPoint* dbpt2{nullptr};
//    DebugPoint* dbpt3{nullptr};

    float GetHeight(float x, float y);
    QVector3D GetNormal(float x, float y);

//    Heightmap(QString fuleName, float heightMultiplier=1, float scalePerStep=1, int pixelsPerStep=1);

//    std::vector<Vertex> CreateArraysFromHeightmap(const QString &filepath, float multiplier, float scalePerStep, int pixelsPerStep);

////    static UV CalculateUV(float xMin, float xMax, float yMin, float yMax, float x, float y);

////    float mScalePerStep;
////    float mHeightMultiplier;
////    int mPixelsPerStep;
//    QVector3D mCurrentNormal;

////    DebugPoint* dbpt1{nullptr};
////    DebugPoint* dbpt2{nullptr};
////    DebugPoint* dbpt3{nullptr};

//    float GetHeight(float x, float y);
//    QVector3D GetNormal(float x, float y);
};

#endif // HEIGHTMAP_H
