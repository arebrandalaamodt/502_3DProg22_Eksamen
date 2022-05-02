#ifndef LIGHT_H
#define LIGHT_H

#include "visualobject.h"

class Light : public VisualObject
{
public:
    Light();
    ~Light(){};
//    void init() override;
//     void draw() override;

     GLfloat mAmbientStrenght{0.3f};
//     GLfloat mAmbientStrenght{200.3f};
     QVector3D mAmbientColor{0.3f, 0.3f, 0.3f};  //Grey

     GLfloat mLightStrenght{0.7f};
//     GLfloat mLightStrenght{300.7f};
     QVector3D mLightColor{0.9f, 0.9f, 0.9f};    //Yellow

     float mSpecularStrenght{0.9f};
     int mSpecularExponent{4};

//     void LightMove() {}
};

#endif // LIGHT_H
