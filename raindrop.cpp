#include "raindrop.h"
#include "meshgenerator.h"

Raindrop::Raindrop(QVector3D InStartPos, float InWorldScale, RegularTriangulation* InGround, int InIndex, float InScale)
{
    Scale = InScale;
    Index = InIndex;
    GroundRef = InGround;
    WorldScale = InWorldScale;
    Acceleration *= WorldScale;
    Acceleration *= (InWorldScale * InWorldScale * InWorldScale);
    mMatrix.translate(InStartPos);
    mVertices = MeshGenerator::Octahedron(4);
}

void Raindrop::TickRaindrop(float DeltaTime)
{
//    TimeAlive += DeltaTime;
    QVector3D CurrentPosition = GetPosition();
    Triangle Triangle = GroundRef->GetTriangle(CurrentPosition.x(), CurrentPosition.y());

    QVector3D P1 = GroundRef->PointCloud.Vertices[Triangle.Indices[0]].GetXYZ();
    QVector3D P2 = GroundRef->PointCloud.Vertices[Triangle.Indices[1]].GetXYZ();
    QVector3D P3 = GroundRef->PointCloud.Vertices[Triangle.Indices[2]].GetXYZ();
    QVector3D Baryc = Barycentric(CurrentPosition, P1, P2, P3);
    float GroundHeight = GetBarycentricHeight(Baryc, P1, P2, P3);

    if (CurrentPosition.z() - Scale - 0.1f > GroundHeight)
    {
        Velocity = Velocity + (Acceleration * DeltaTime);

        QVector3D NewVelocity = Velocity * DeltaTime;

        mMatrix.translate(NewVelocity);
        IsOnGround = false;
    }
    else
    {
        float Difference = GroundHeight - (CurrentPosition.z() - Scale);
        if (Difference > 0)
        {
            mMatrix.translate(0,0,Difference);
        }

        Velocity = Velocity + (Acceleration * DeltaTime);

        QVector3D GroundNormal = QVector3D::crossProduct(P2-P1, P3-P1);
        GroundNormal.normalize();

        QVector3D SlideAlongNormal = Velocity - 2 * (QVector3D::dotProduct(Velocity, GroundNormal)) * GroundNormal;
        Velocity = (Velocity + SlideAlongNormal) * 0.5f;
        Velocity = Velocity * Friction;
        if (!IsOnGround)
        {
            Velocity = Velocity * 0.9;
        }
        IsOnGround = true;
        HasReachedGround = true;

        QVector3D NewVelocity = Velocity * DeltaTime;

        mMatrix.translate(NewVelocity);
    }

//    if (HasReachedGround)
//    {
//        if (TimeAlive > TimeToLive)
//        {
//            qDebug() << __FUNCTION__ " | Should Delete now.";
////            delete this; // Crashes
//        }
//    }

}
