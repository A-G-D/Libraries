#include <cmath>
#include "vector3D.hpp"
#include "cylinder.hpp"

using namespace Space;
using namespace Space::Shape;

#define PI 3.141592653589793238463

/*
*   Cylinder class definition
*/
#define __ Cylinder

inline bool __::is_point_inside(FLOAT dx, FLOAT dy, FLOAT dz, VEC *s) const
{
    return
        dz >= 0. && dz <= s->z &&
        (dx*dx)/(s->x*s->x) + (dy*dy)/(s->y*s->y) <= 1.;
}

inline FLOAT __::volume() const
{
    return PI*(this->__s->x*this->__s->y*this->__s->z);
}
inline FLOAT __::surface_area() const
{
    return PI*(3.*(this->__s->x + this->__s->y) - sqrt((3.*this->__s->x + this->__s->y)*(this->__s->x + 3.*this->__s->y)))*this->__s->z;
}

__::__(__ const &cylinder)
    : Shape3D(cylinder)
{
}
__::__(__ &&cylinder)
    : Shape3D(cylinder)
{
}
__::~__()
{
}