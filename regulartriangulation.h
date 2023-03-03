#ifndef REGULARTRIANGULATION_H
#define REGULARTRIANGULATION_H

#include "visualobject.h"





class RegularTriangulation : public VisualObject
{
public:
    RegularTriangulation(std::string FileName);



private:
    int Resolution{0};
    float Size{0.0f};

};

#endif // REGULARTRIANGULATION_H
