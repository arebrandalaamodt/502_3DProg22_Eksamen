#include <iostream>

#include "meshgenerator.h"
#include "npc.h"


NPC::NPC(std::string name, std::vector<Vertex> verticesVector)
{
    mName = name;
    mVertices = verticesVector;
    fillBezierVertices();


}

void NPC::setPatrolPathObject(VisualObject* objectToSet)
{
    patrolPathObject = objectToSet;

    if (patrolPathObject)
    {
        patrolPathLocation = patrolPathObject->getPosition();
    }
}

void NPC::fillBezierVertices()
{
        Vertex v0(10,10,10,     1,0,1);
        Vertex v1(30,20,10,     1,0,1);
        Vertex v2(40,10,10,     1,0,1);
        Vertex v3(50,30,10,     1,0,1);

    float xmin = 0.0f, xmax = 1.0f, h = 0.01f; // The size to draw // x1
    for (float x = xmin; x < xmax; x += h)
    {
        bezierVertices.push_back(MeshGenerator::fourPointBezierCurveCalc(v0, v1, v2, v3, x));
    }
    sizeOfBezierVertices = bezierVertices.size() - 1;
}

void NPC::patrol()
{
    Vertex temp = bezierVertices.at(patrolTracker);
    QVector3D tempVector(temp.getX()+patrolPathLocation.x(), temp.getY()+patrolPathLocation.y(), temp.getZ()+patrolPathLocation.z());

    this->setPosition(tempVector.x(), tempVector.y(), tempVector.z());

    if(bGoingRight == true)
    {
        patrolTracker++;
        if (patrolTracker > sizeOfBezierVertices)
        {
            patrolTracker = sizeOfBezierVertices;
            bGoingRight = false;
        }
        return;
    }
    else if (bGoingRight == false)
    {
        patrolTracker--;
        if (patrolTracker < 0)
        {
            patrolTracker = 0;
            bGoingRight = true;
        }
    }
}

