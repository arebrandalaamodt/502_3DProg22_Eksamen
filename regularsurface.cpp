#include "regularsurface.h"

RegularSurface::RegularSurface()
{
    std::cout << "RegularSurface::RegularSurface() | Default Constructor" << std::endl;
}

RegularSurface::RegularSurface(std::string name, std::vector<Vertex> verticesVector)
    : VisualObject(name, verticesVector)
{
    std::cout << "RegularSurface::RegularSurface(std::string name, std::vector<Vertex> verticesVector) | Constructor" << std::endl;

}

void RegularSurface::setMinMaxValues(std::vector<Vertex> &verticesIn)
{

    std::vector<Vertex>::iterator iter = verticesIn.begin();

    int x{0};

    for(iter; iter < verticesIn.end(); iter++)
    {
        x++;
        std::cout << "Iterating: " << x << std::endl;

        std::cout << *iter << std::endl;
    }
//    for(int i : verticesIn)


}
