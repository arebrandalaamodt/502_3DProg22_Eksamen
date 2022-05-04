
#include "meshgenerator.h"
#include "trophy.h"

Trophy::Trophy(bool bRedTrophy)
    : bRedTrophy {bRedTrophy}
{
    mVertices = MeshGenerator::OBJFile("../3Dprog22/Assets/trophy.obj");
    if (bRedTrophy == true)
    {
        this->setMonoColor(QVector3D(1.f, 0.f, 0.f));
    }
    else if(bRedTrophy == false)
    {
        setMonoColor(QVector3D(0.f, 0.f, 1.f));
    }
}

//void Trophy::rotateOnTick()
//{

//}
