#ifndef TROPHY_H
#define TROPHY_H

#include "interactiveobject.h"

class Trophy : public InteractiveObject
{
public:
    Trophy(bool bRedTrophy);
    ~Trophy(){};
    bool getIsRed()                 {return bRedTrophy;}
    bool getIsTaken()               {return bIsTaken;}
    void setIsTaken(bool boolToSet) {bIsTaken = boolToSet;}
//    void rotateOnTick();
private:
    bool bRedTrophy {true};
    bool bIsTaken   {false};
};

#endif // TROPHY_H
