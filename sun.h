#ifndef SUN_H
#define SUN_H

#include "light.h"

class Sun : public Light
{
public:
    Sun();
    ~Sun(){};

private:
    QVector3D mOrbitCenter {250.f, 250.f, 30.f};
    QVector3D mOrbitPoint {300.f, 250.f, 30.f};
    QVector3D mOrbitPointOrigo {0.f, 0.f, 30.f};
    float mOrbitRadius {50.f};
    float mRotationAngleDeg {5.f};
    float PI = 3.1415928f;


public:
    void progressOrbit();

};

#endif // SUN_H
