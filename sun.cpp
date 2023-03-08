//#include <cmath>
#include "sun.h"

Sun::Sun()
{
    mMatrix.setToIdentity();
    mMatrix.translate(mOrbitCenter);
    this->move(mOrbitRadius, 0.f, 300.f);

    mOrbitPoint = mOrbitCenter + QVector3D(mOrbitRadius, 0.f, 0.f);
}

void Sun::progressOrbit()
{
    mOrbitPointOrigo = mOrbitPoint - mOrbitCenter;

    float tempX = (mOrbitPointOrigo.x() * cos(PI/100) ) - (mOrbitPointOrigo.y() * sin(PI/100));
    float tempY = (mOrbitPointOrigo.y() * cos(PI/100) ) + (mOrbitPointOrigo.x() * sin(PI/100));

    mOrbitPointOrigo.setX(tempX);
    mOrbitPointOrigo.setY(tempY);

    mOrbitPoint = mOrbitPointOrigo + mOrbitCenter;
    setPosition(mOrbitPoint);
}
