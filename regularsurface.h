#ifndef REGULARSURFACE_H
#define REGULARSURFACE_H

#include <limits>
#include "iostream"
#include "visualobject.h"

class RegularSurface : public VisualObject
{
public:
    RegularSurface();
    RegularSurface(std::string name, std::vector<Vertex> verticesVector);

    float mXMax, mYMax{0};
//    float mXMin, mYMin{std::numeric_limits<float>::max()};
    float mXMin, mYMin{0};

    float mDefaultResolution{1.0f};

    std::vector<Vertex> FullVerticesVector;
    std::vector<Vertex> mAveragedHeightVerticesVector;

    void setMinMaxValues(std::vector<Vertex>& verticesIn);
//    std::vector<Vertex> averageHeights(float gridResolution, std::vector<Vertex>& verticesIn);
    void averageHeights(float gridResolution, std::vector<Vertex>& verticesIn);

};

#endif // REGULARSURFACE_H
