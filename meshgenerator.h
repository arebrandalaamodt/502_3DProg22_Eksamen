#ifndef MESHGENERATOR_H
#define MESHGENERATOR_H

#include <vector>

#include "vertex.h"


class MeshGenerator
{
//~~~~~~~~~ OBJ File, Plane, Grid, Cube, Octahedron, Disc, TriangleSurface, Graph

// OBJ File
public:     static std::vector<Vertex> OBJFile(const char* fileName, const QVector3D& color);
public:     static std::vector<Vertex> OBJFile(const char* fileName);

// Plane
public:     static std::vector<Vertex> Plane(float size);
//            static void CreatePlane(float size);

// Grid
public:     static std::vector<Vertex> Grid(float size);

// Cube
public:     static std::vector<Vertex> Cube();
private:    static void CreateCubeSide(QVector3D direction, std::vector<Vertex>& verticesOut);

// Octahedron
public:     static std::vector<Vertex> Octahedron(int recursions);
private:    static void OctahedronSubDivide(const QVector3D& a, const QVector3D& b, const QVector3D& c, int n, std::vector<Vertex>& verticesOut);

// 2D Graph
public:     static std::vector<Vertex> Create2DGraph();
private:    static float SingleVariableFunction(float x);

// 3D Graph
public:     static std::vector<Vertex> Create3DGraph();
private:    static float MultivariableFunction(float x, float y);

// XYZ
public:     static std::vector<Vertex> XYZ(float length);

// BezierCurve
public:     static std::vector<Vertex> FourPointBezierCurve();
private:    static Vertex fourPointBezierCurveCalc(Vertex p0, Vertex p1, Vertex p2, Vertex p3, float t);
};



#endif // MESHGENERATOR_H
