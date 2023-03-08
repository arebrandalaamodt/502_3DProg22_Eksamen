#ifndef REGULARTRIANGULATION_H
#define REGULARTRIANGULATION_H

#include "lasreader.h"
#include "visualobject.h"





class RegularTriangulation : public VisualObject
{
public:
    RegularTriangulation(std::string FileName);

    void init() override;
    void draw() override;

    static inline float Scale{1};
    std::vector<GLuint> mIndices;
    GLuint mIBO{0};

    Triangle GetTriangle(float x, float y);
    struct PointCloudMesh PointCloud;

private:
    int Resolution{500};
    float Size{500.0f};

    bool CheckTriangleHeight(QVector3D p1, QVector3D p2, QVector3D p3, float h);
    bool CheckTriangleHeight(int triIndex, float h);
};

#endif // REGULARTRIANGULATION_H
