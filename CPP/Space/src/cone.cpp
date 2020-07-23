#include <cmath>
#include "vector3D.hpp"
#include "cone.hpp"

using namespace Space;
using namespace Space::Shape;

#define PI 3.141592653589793238463

/*
*   Cone class definition
*/
#define __ Cone

bool __::is_point_inside(FLOAT dx, FLOAT dy, FLOAT dz, VEC *s) const
{
    FLOAT rf = 1. - dz/s->z;
    return
        rf >= 0. && rf <= 1. &&
        (dx*dx)/(s->x*s->x) + (dy*dy)/(s->y*s->y) <= rf*rf;
}

inline FLOAT __::volume() const
{
    return (PI/3.)*(this->__s->x*this->__s->y*this->__s->z);
}
inline FLOAT __::surface_area() const
{
    return (0.5*PI)*(3.*(this->__s->x + this->__s->y) - sqrt((3.*this->__s->x + this->__s->y)*(this->__s->x + 3.*this->__s->y)))*this->__s->z;
}

__::__(__ const &cone)
    : Shape3D(cone)
{
}
__::__(__ &&cone)
    : Shape3D(cone)
{
}
__::~__()
{
}