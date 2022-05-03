#include "interactiveobject.h"

InteractiveObject::InteractiveObject()
    : mx{0.0f}, my{0.0f}, mz{0.0f}
{
    // DO NOT USE
}

InteractiveObject::InteractiveObject(std::string name, std::vector<Vertex> verticesVector)
    : mx{0.0f}, my{0.0f}, mz{0.0f}

{
    mName = name;
    mVertices = verticesVector;
}

InteractiveObject::~InteractiveObject() {}


void InteractiveObject::move(float dx, float dy, float dz)
{
//    std::cout << dx << " - " << dy << " - " << dz << std::endl;
    mx += dx;
    my += dy;
    mz += dz;
    mPosition.translate (dx, dy, dz);


    updateMatrix();
}

void InteractiveObject::move(const QVector3D& xyz)
{
    mx += xyz.x();
    my += xyz.y();
    mz += xyz.z();
    mPosition.translate(xyz.x(), xyz.y(), xyz.z());
    updateMatrix();
}

void InteractiveObject::setHeight(float z)
{
    z = z + 1;
    mPosition.translate(0, 0, z-mz);
    mz = z;
    updateMatrix();
}

QVector3D InteractiveObject::getPosition()
{
//    std::cout << mx << " - " << my << " - " << mz << std::endl;
    return QVector3D{mx, my, mz};
}

void InteractiveObject::rotate(float degrees)
{
    QMatrix4x4 newRotation;
    newRotation.setToIdentity();
    newRotation.rotate(degrees, {0, 0, 1});
    mRotation = newRotation * mRotation;

    updateMatrix();
}

void InteractiveObject::setScale(float scale)
{
    QMatrix4x4 newScale;
    newScale.setToIdentity();
    newScale.scale({scale, scale, scale});
    mScale = newScale;

    updateMatrix();
}

void InteractiveObject::MoveForward(float value)
{
    mx += value;
    mPosition.translate(value, 0.f, 0.f);
    updateMatrix();
}

void InteractiveObject::MoveRight(float value)
{
    my += value;
    mPosition.translate(0.f, value, 0.f);
    updateMatrix();
}

void InteractiveObject::MoveForwardLocal(float value)
{
    QVector3D temp = getPosition();
    QVector3D tempForwardVector = GetForward();

//    std::cout << "temp x: " << temp.x() << " | y: " << temp.y() << std::endl;

    setPosition(temp.x() + tempForwardVector.x() * value,
                temp.y() + tempForwardVector.y() * value,
                temp.z());
}

void InteractiveObject::RotateRight(float value)
{
    QVector3D forwardOrigo = GetForward();

    std::cout << "forwardOrigo x: " << forwardOrigo.x() << " | y: " << forwardOrigo.y() << std::endl;

    float tempX {0};
    float tempY {0};

    if(value > 0)
    {
        tempX = (forwardOrigo.x() * cos(PI/100) ) + (forwardOrigo.y() * sin(PI/100));
        tempY = (forwardOrigo.y() * cos(PI/100) ) - (forwardOrigo.x() * sin(PI/100));
    }
    else if (value < 0)
    {
        tempX = (forwardOrigo.x() * cos(PI/100) ) - (forwardOrigo.y() * sin(PI/100));
        tempY = (forwardOrigo.y() * cos(PI/100) ) + (forwardOrigo.x() * sin(PI/100));
    }
    else
    {
        std::cout << "Rotating with 0 as input" << std::endl;
    }

    forwardOrigo.setX(tempX);
    forwardOrigo.setY(tempY);

    SetForward(forwardOrigo);
}

void InteractiveObject::RotateLeft(float value)
{

}

//void InteractiveObject::MoveRightLocal(float value)
//{
//    QVector3D temp = getPosition();
//    QVector3D tempForwardVector = GetForward();

//    std::cout << "temp x: " << temp.x() << " | y: " << temp.y() << std::endl;

//    setPosition(temp.x() + tempForwardVector.x() * value,
//                temp.y() + tempForwardVector.y() * value,
//                temp.z());
//}

void InteractiveObject::setPosition(float x, float y, float z)
{
//    x = x +1;
//    y = y +1;
//    z = z + 1;
    mPosition.translate(x-mx, y-my, z-mz);
    mx = x;
    my = y;
    mz = z;
    updateMatrix();
}

//void InteractiveObject::updateMatrix()
//{
//    mMatrix = mPosition * mRotation * mScale;
//}
