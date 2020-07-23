#include "vector3D.hpp"
#include "prism.hpp"

using namespace Space;
using namespace Space::Shape;

/*
*   Prism class definition
*/
#define __ Prism

inline bool __::is_point_inside(FLOAT dx, FLOAT dy, FLOAT dz, VEC *s) const
{
    return dx*dx <= s->x*s->x && dy*dy <= s->y*s->y && dz*dz <= s->z*s->z;
}

inline FLOAT __::volume() const
{
    return 8.*(this->__s->x*this->__s->y*this->__s->z);
}
inline FLOAT __::surface_area() const
{
    return 8.*(this->__s->x*this->__s->y + this->__s->x*this->__s->z + this->__s->y*this->__s->z);
}

__::__(__ const &prism)
    : Shape3D(prism)
{
}
__::__(__ &&prism)
    : Shape3D(prism)
{
}
__::~__()
{
}