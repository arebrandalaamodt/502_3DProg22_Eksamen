#ifndef RAINDROP_H
#define RAINDROP_H

#include "interactiveobject.h"
#include "regulartriangulation.h"

class Raindrop : public InteractiveObject
{
public:
    Raindrop(QVector3D InStartPos, float InWorldScale, RegularTriangulation* InGround, int InIndex, float InScale=1);
    void TickRaindrop(float DeltaTime);
    RegularTriangulation* GroundRef;

private:
    unsigned long long Index{0};
//    float TimeAlive{0};
//    float TimeToLive{5.0f};
    float LastReportTime{0.f};
    float TimeToKill{60.f};
    bool IsOnGround{false};
    bool HasReachedGround{false};
    float Friction{0.999};
    float Scale{0.5};
    static inline float WorldScale{1};
    QVector3D Acceleration{0, 0, -9.81};
    QVector3D Velocity{0, 0, 0};

};

#endif // RAINDROP_H
