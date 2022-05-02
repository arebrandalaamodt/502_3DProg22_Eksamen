#ifndef GRAPH_H
#define GRAPH_H

#include "visualobject.h"

class Graph : public VisualObject
{
public:
    Graph();
    ~Graph() {}
    void init2(GLint matrixUniform);
    void draw() override;

    static float function(float x);
    void createGraphVertices();



};

#endif // GRAPH_H
