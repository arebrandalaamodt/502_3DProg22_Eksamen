#include "vertex.h"

std::ostream& operator<< (std::ostream& os, const Vertex& v) {
  os << std::fixed;
  os << "(" << v.m_xyz[0] << ", " << v.m_xyz[1] << ", " << v.m_xyz[2] << ") ";
  os << "(" << v.m_normal[0] << ", " << v.m_normal[1] << ", " << v.m_normal[2] << ") ";
  os << "(" << v.m_st[0] << ", " << v.m_st[1] << ") ";
  return os;
}
std::istream& operator>> (std::istream& is, Vertex& v) {
// Trenger fire temporære variabler som kun skal lese inn parenteser og komma
  char dum, dum2, dum3, dum4;
  is >> dum >> v.m_xyz[0] >> dum2 >> v.m_xyz[1] >> dum3 >> v.m_xyz[2] >> dum4;
  is >> dum >> v.m_normal[0] >> dum2 >> v.m_normal[1] >> dum3 >> v.m_normal[2] >> dum4;
  is >> dum >> v.m_st[0] >> dum2 >> v.m_st[1] >> dum3;
  return is;
}

Vertex::Vertex()
{

}

Vertex::Vertex(float x, float y, float z, float r, float g, float b)
    : m_xyz{x,y,z}, m_normal{r,g,b}
{
//    m_xyz[0] = x; // USELESS & BAD DONT DO IT MR. FRODO
}

Vertex::Vertex(float x, float y, float z, float r, float g, float b, float u, float v)
    : m_xyz{x,y,z}, m_normal{r,g,b}, m_st{u,v}
{
//    std::cout << "8 Argument constructor" << std::endl;
}

Vertex::Vertex(float x, float y, float r, float g, float b)
    : m_xyz{x,y,0}, m_normal{r,g,b}, m_st{0,0}
{
//    std::cout << "y = 0 vertex constructor" << std::endl;
}

Vertex::Vertex(QVector3D xyz, float r, float g, float b)
    :m_xyz{xyz.x(), xyz.y(), xyz.z()}, m_normal{r,g,b}
{

}

Vertex::Vertex(QVector3D xyz, QVector3D normal, float s, float t)
    :m_xyz{xyz.x(), xyz.y(), xyz.z()}, m_normal{normal.x(), normal.y(), normal.z()}, m_st{s, t}
{

}

void Vertex::setX(float px)
{
    m_xyz[0] = px;
}

void Vertex::setY(float py)
{
    m_xyz[1] = py;
}

void Vertex::setZ(float pz)
{
    m_xyz[2] = pz;
}

void Vertex::setR(float pr)
{
    m_normal[0] = pr;
}

void Vertex::setG(float pg)
{
    m_normal[1] = pg;
}

void Vertex::setB(float pb)
{
    m_normal[2] = pb;
}



float Vertex::getX()
{
    return m_xyz[0];
}

float Vertex::getY()
{
    return m_xyz[1];
}

float Vertex::getZ()
{
    return m_xyz[2];
}
