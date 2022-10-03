#ifndef REGULARSURFACE_H
#define REGULARSURFACE_H

#include "visualobject.h"

class RegularSurface : public VisualObject
{
public:
    RegularSurface();
    RegularSurface(std::string name, std::vector<Vertex> verticesVector);

    float XMax, YMax, XMin, YMin{0};


    void setMinMaxValues(std::vector<Vertex>& verticesIn);

};

#endif // REGULARSURFACE_H
