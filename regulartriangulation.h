#ifndef REGULARTRIANGULATION_H
#define REGULARTRIANGULATION_H

#include "lasreader.h"
#include "visualobject.h"





class RegularTriangulation : public VisualObject
{
public:
    RegularTriangulation(std::string FileName);



private:
    int Resolution{500};
    float Size{500.0f};
    struct PointCloudMesh PointCloud;

};

#endif // REGULARTRIANGULATION_H
