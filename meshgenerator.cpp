
#include "OBJFileLoader.h"
#include "vertex.h"
#include "meshgenerator.h"

std::vector<Vertex> MeshGenerator::OBJFile(const char* fileName, const QVector3D& color)
{
    return OBJFileLoader::loadOBJ(fileName, color);
}
std::vector<Vertex> MeshGenerator::OBJFile(const char *fileName)
{
    return OBJFileLoader::loadOBJ(fileName, QVector3D(0.f, 0.f, 0.f));

}
std::vector<Vertex> MeshGenerator::Plane(float size)
{
    std::vector<Vertex> verticesOut;

    verticesOut.push_back(Vertex{-size/2.f,  size/2.f, 0.0f,   0.0f, 0.0f, 1.0f,    1.0f, 1.f});
    verticesOut.push_back(Vertex{ size/2.f, -size/2.f, 0.0f,   0.0f, 0.0f, 1.0f,    0.0f, 0.f});
    verticesOut.push_back(Vertex{ size/2.f,  size/2.f, 0.0f,   0.0f, 0.0f, 1.0f,    0.0f, 1.f});

    verticesOut.push_back(Vertex{-size/2.f,  size/2.f, 0.0f,   0.0f, 0.0f, 1.0f,    1.f, 1.f});
    verticesOut.push_back(Vertex{-size/2.f, -size/2.f, 0.0f,   0.0f, 0.0f, 1.0f,    1.f, 0.f});
    verticesOut.push_back(Vertex{ size/2.f, -size/2.f, 0.0f,   0.0f, 0.0f, 1.0f,    0.f, 0.f});



    return verticesOut;
}
std::vector<Vertex> MeshGenerator::Grid(float size)
{
    std::vector<Vertex> verticesOut;

    verticesOut.push_back(Vertex{-size/2.f,  size/2.f, 0.0f,   0.0f, 0.0f, 1.0f,    1.0f, 1.f});
    verticesOut.push_back(Vertex{ size/2.f, -size/2.f, 0.0f,   0.0f, 0.0f, 1.0f,    0.0f, 0.f});
    verticesOut.push_back(Vertex{ size/2.f,  size/2.f, 0.0f,   0.0f, 0.0f, 1.0f,    0.0f, 1.f});

    verticesOut.push_back(Vertex{-size/2.f,  size/2.f, 0.0f,   0.0f, 0.0f, 1.0f,    1.f, 1.f});
    verticesOut.push_back(Vertex{-size/2.f, -size/2.f, 0.0f,   0.0f, 0.0f, 1.0f,    1.f, 0.f});
    verticesOut.push_back(Vertex{ size/2.f, -size/2.f, 0.0f,   0.0f, 0.0f, 1.0f,    0.f, 0.f});



    return verticesOut;
}
std::vector<Vertex> MeshGenerator::Cube()
{
    std::vector<Vertex> verticesOut;

    CreateCubeSide({ 0, 0,-1}, verticesOut);
    CreateCubeSide({ 0, 0, 1}, verticesOut);
    CreateCubeSide({ 0, 1, 0}, verticesOut);
    CreateCubeSide({ 0,-1, 0}, verticesOut);
    CreateCubeSide({-1, 0, 0}, verticesOut);
    CreateCubeSide({ 1, 0, 0}, verticesOut);

    return verticesOut;
}
void MeshGenerator::CreateCubeSide(QVector3D direction, std::vector<Vertex>& verticesOut)
{
    float r, g, b;
    r = abs(direction.x());
    g = abs(direction.y());
    b = abs(direction.z());

    auto SquareCenter = direction*(1/2);

    auto temp1 = SquareCenter;

    temp1.setX(abs(abs(direction.x())-1));
    temp1.setY(abs(abs(direction.y())-1));
    temp1.setZ(abs(abs(direction.z())-1));

    auto corner1 = temp1;
    corner1.setX(corner1.x() * -1);

    auto corner2 = temp1;
    corner2.setY(corner2.y() * -1);

    auto corner3 = temp1;
    corner3.setZ(corner3.z() * -1);

    temp1 = SquareCenter;
    temp1.setX(abs(abs(direction.x())-1)*-1);
    temp1.setY(abs(abs(direction.y())-1)*-1);
    temp1.setZ(abs(abs(direction.z())-1)*-1);

    auto corner4 = temp1;
    corner4.setX(corner4.x() * -1);

    auto corner5 = temp1;
    corner5.setY(corner5.y() * -1);

    auto corner6 = temp1;
    corner6.setZ(corner6.z() * -1);

    corner1 = (corner1 + direction);
    corner2 = (corner2 + direction);
    corner3 = (corner3 + direction);
    corner4 = (corner4 + direction);
    corner5 = (corner5 + direction);
    corner6 = (corner6 + direction);

    verticesOut.push_back(Vertex{corner1, r, g, b});
    verticesOut.push_back(Vertex{corner2, r, g, b});
    verticesOut.push_back(Vertex{corner3, r, g, b});
    verticesOut.push_back(Vertex{corner4, r, g, b});
    verticesOut.push_back(Vertex{corner5, r, g, b});
    verticesOut.push_back(Vertex{corner6, r, g, b});

}
std::vector<Vertex> MeshGenerator::Octahedron(int recursions)
{
    std::vector<Vertex> verticesOut;

    QVector3D v0{ 0, 0, 1};
    QVector3D v1{ 1, 0, 0};
    QVector3D v2{ 0, 1, 0};
    QVector3D v3{-1, 0, 0};
    QVector3D v4{ 0,-1, 0};
    QVector3D v5{ 0, 0,-1};

    OctahedronSubDivide(v0, v1, v2, recursions, verticesOut);
    OctahedronSubDivide(v0, v2, v3, recursions, verticesOut);
    OctahedronSubDivide(v0, v3, v4, recursions, verticesOut);
    OctahedronSubDivide(v0, v4, v1, recursions, verticesOut);
    OctahedronSubDivide(v5, v2, v1, recursions, verticesOut);
    OctahedronSubDivide(v5, v3, v2, recursions, verticesOut);
    OctahedronSubDivide(v5, v4, v3, recursions, verticesOut);
    OctahedronSubDivide(v5, v1, v4, recursions, verticesOut);

    return verticesOut;
}
std::vector<Vertex> MeshGenerator::XYZ(float length)
{
    std::vector<Vertex> verticesOut;

    verticesOut.push_back(Vertex{0,0,0,1,0,0,0,0});   // X-Axis, red
    verticesOut.push_back(Vertex{length,0,0,1,0,0,0,0});

    verticesOut.push_back(Vertex{0,0,0,0,1,0,0,0});   // Y-Axis, green
    verticesOut.push_back(Vertex{0,length,0,0,1,0,0,0});

    verticesOut.push_back(Vertex{0,0,0,0,0,1,0,0});   // z-Axis, blue
    verticesOut.push_back(Vertex{0,0,length,0,0,1,0,0});

    return verticesOut;
}
void MeshGenerator::OctahedronSubDivide(const QVector3D &a, const QVector3D &b, const QVector3D &c, int n, std::vector<Vertex>& verticesOut)
{
    if(n > 0)
    {
        QVector3D v1 = a+b;
        v1.normalize();
        QVector3D v2 = a+c;
        v2.normalize();
        QVector3D v3 = c+b;
        v3.normalize();



        OctahedronSubDivide( a, v1, v2, n-1, verticesOut);
        OctahedronSubDivide( c, v2, v3, n-1, verticesOut);
        OctahedronSubDivide( b, v3, v1, n-1, verticesOut);
        OctahedronSubDivide(v3, v2, v1, n-1, verticesOut);
    }
    else
    {
        Vertex v{a.x(), a.y(), a.z(), a.x(), a.y(), a.z()};
        verticesOut.push_back(v);

        v = Vertex{b.x(), b.y(), b.z(), b.x(), b.y(), b.z()};
        verticesOut.push_back(v);

        v = Vertex{c.x(), c.y(), c.z(), c.x(), c.y(), c.z()};
        verticesOut.push_back(v);
    }
}
float MeshGenerator::SingleVariableFunction(float x)
{
    float y {0};
//    y = 0.86248 * pow(x,2) - 0.02364 * x + 0.34454;

    y = ((6836.f/13209.f)*pow(x, 3)) - ((107987.f/22015.f)*pow(x, 2)) + ((364691.f/26418.f) * x) - (52943.f/6290.f);
//    y = 0.51752f*pow(x, 3) - 4.90515f*pow(x, 2) + 13.80464f * x - 8.41701f;

//    std::cout << "y = " << y << std::endl;
    return y;
}
std::vector<Vertex> MeshGenerator::Create2DGraph()
{
    std::vector<Vertex> verticesOut;

    float xmin = 0.f, xmax = 10.f, h = 0.25f; // The size to draw // x1
    for (auto x = xmin; x < xmax; x += h)
    {
        float y = SingleVariableFunction(x);
        verticesOut.push_back(Vertex{x    ,y  ,1  ,0  ,0}); // Top left
        y = SingleVariableFunction(x+h);
        verticesOut.push_back(Vertex{x+h  ,y  ,1  ,0  ,0}); // Top Right
        verticesOut.push_back(Vertex{x    ,0  ,1  ,0  ,0}); // Bot Left
        verticesOut.push_back(Vertex{x    ,0  ,0  ,1  ,0}); // Bot Left, start of 2nd triangle
        y = SingleVariableFunction(x+h);
        verticesOut.push_back(Vertex{x+h  ,y  ,1  ,0  ,0}); // Top Right, 2nd triangle
        verticesOut.push_back(Vertex{x+h  ,0  ,1  ,0  ,0}); // Bot Right, 2nd triangle
    }

    return verticesOut;
}
float MeshGenerator::MultivariableFunction(float x, float y)
{
    //~~
    // Chosen function
    // f(x,y) = x - x^3 + y^2
    //~~//
//    float z = x - pow(x,3) + pow(y,2);

//    float z = 0.3f * ( sin(sqrt((x*x)+(y*y)) ) + sqrt((x*x)+(y*y)) * sqrt((x*x)+(y*y)) * 0.0001f ) + 0.5f;
//    float z = -.8f + 0.1*sin(sqrt(x*x + y*y));
//    return z * 1.5;  // * 2.5 gives more zoomed out view?

    float z = .2f * sin(sqrt((x*x)+(y*y)));

//    float z = (sin(x/5)+cos(y/5));

    return z;  // * 2.5 gives more zoomed out view?
}
std::vector<Vertex> MeshGenerator::Create3DGraph()
{
    std::vector<Vertex> verticesOut;
    float heightOffset = -.5f;

//    float xmin = -1.0f, xmax = 1.0f, ymin = -1.0f, ymax = 1.0f, h = 0.025f; // The size to draw // x1
    float xmin = -100.f, xmax = 100.f, ymin = -100.f, ymax = 100.f, h = 1.f; // The size to draw // x1
    for (auto x=xmin; x<xmax; x+=h)
        for (auto y=ymin; y<ymax; y+=h)
        {
            float z = MultivariableFunction(x,y);
            verticesOut.push_back(Vertex{x,y,z-1.f      ,z -heightOffset   ,z -heightOffset  ,z -heightOffset      ,0,0});
            z = MultivariableFunction(x+h,y);
            verticesOut.push_back(Vertex{x+h,y,z-1.f    ,z -heightOffset  ,z -heightOffset  ,z -heightOffset      ,0,0});
            z = MultivariableFunction(x,y+h);
            verticesOut.push_back(Vertex{x,y+h,z-1.f    ,z -heightOffset/2  ,z -heightOffset/2  ,z -heightOffset/2      ,0,0});
            verticesOut.push_back(Vertex{x,y+h,z-1.f    ,z   ,z   ,z      ,0,0});
            z = MultivariableFunction(x+h,y);
            verticesOut.push_back(Vertex{x+h,y,z-1.f    ,z   ,z   ,z       ,0,0});
            z = MultivariableFunction(x+h,y+h);
            verticesOut.push_back(Vertex{x+h,y+h,z-1.f  ,z  ,z   ,z       ,0,0});
        }

    return verticesOut;
}

Vertex MeshGenerator::fourPointBezierCurveCalc(Vertex p0, Vertex p1, Vertex p2, Vertex p3, float t)
{
    Vertex returnPoint{0,0,1,0,1};

    returnPoint.setX(
                        ( pow((1-t), 3) * p0.getX()         )
                    +   ( pow((1-t), 2) * 3 * t * p1.getX() )
                    +   ( (1 - t) * 3 * t * t *  p2.getX()  )
                    +   ( t * t * t * p3.getX()             )
                    );
    returnPoint.setY(
                        ( pow((1-t), 3) * p0.getY()         )
                    +   ( pow((1-t), 2) * 3 * t * p1.getY() )
                    +   ( (1 - t) * 3 * t * t *  p2.getY()  )
                    +   ( t * t * t * p3.getY()             )
                    );
    return returnPoint;
}

std::vector<Vertex> MeshGenerator::FourPointBezierCurve(Vertex p0, Vertex p1, Vertex p2, Vertex p3)
{
    std::vector<Vertex> verticesOut;



    float xmin = 0.0f, xmax = 1.0f, h = 0.01f; // The size to draw // x1
    for (float x = xmin; x < xmax; x += h)
    {
        verticesOut.push_back(fourPointBezierCurveCalc(p0, p1, p2, p3, x));
    }
    return verticesOut;
}
