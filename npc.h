#ifndef NPC_H
#define NPC_H

#include "interactiveobject.h"
#include <chrono>


//class Cube;
class NPC : public InteractiveObject
{
public:
    NPC(std::string name, std::vector<Vertex> verticesVector);
    ~NPC() {}

    void patrol();
    void setPatrolPathObject(VisualObject* objectToSet);
private:
    std::vector<Vertex> mBezierVertices;
    bool bGoingRight    {true};

    VisualObject* patrolPathObject;
    QVector3D patrolPathLocation {0.f, 0.f, 0.f};
    std::vector<Vertex> bezierVertices;
    int sizeOfBezierVertices {0};
    int patrolTracker {0};
    void fillBezierVertices();

//    double msPerDraw;
//    double xPos {-5.f};
//    double yPos {0};
//    double zPos {0};


};

#endif // NPC_H
