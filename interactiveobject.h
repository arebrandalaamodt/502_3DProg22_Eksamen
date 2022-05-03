#ifndef INTERACTIVEOBJECT_H
#define INTERACTIVEOBJECT_H

#include "visualobject.h"

class InteractiveObject : public VisualObject
{
public:
    InteractiveObject();
    InteractiveObject(std::string name, std::vector<Vertex> verticesVector);
    ~InteractiveObject() override;

//    void init222222(GLint matrixUniform);
//    void draw() override;
    //~~
    void move(float x, float y, float z) override;
    void move(const QVector3D& xyz) override;
    void setHeight (float z);
    QVector3D getPosition() override;

    void setPosition(float x, float y, float z);
    void rotate(float degrees);
    void setScale(float scale);



    //~~//
     private:
    QVector3D ForwardVector{1.f,0.f,0.f};
    QVector3D RightVector{0.f,1.f,0.f};

public:
    QVector3D GetForward() const {return ForwardVector;}
    QVector3D GetRight() const {return RightVector;}

    void SetForward(const QVector3D& newVector) {ForwardVector = newVector;}
    void SetRight(const QVector3D& newVector) {RightVector = newVector;}

    void MoveForward(float value);
    void MoveRight(float value);

    float mx, my, mz {0.f}; // posisjon


//    void updateMatrix() override;
};

#endif // INTERACTIVEOBJECT_H
