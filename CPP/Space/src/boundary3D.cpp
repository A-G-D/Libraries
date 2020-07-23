#include "boundary3D.hpp"
#include "vector3D.hpp"

using namespace Space;
using namespace Space::Shape;

FLOAT GetObliqueComponentLength(VEC *u, VEC *v, VEC *w, FLOAT dx, FLOAT dy, FLOAT dz)
{
    FLOAT
        rx = v->x + w->x,
        ry = v->y + w->y,
        rz = v->z + w->z,
        r  = (dx*rx + dy*ry + dz*rz)/(rx*rx + ry*ry + rz*rz);

    return (dx - r*rx)/u->x + (dy - r*ry)/u->y + (dz - r*rz)/u->z;
}

/*
*   Boundary3D abstract class
*/
#define __ Boundary3D

bool __::contains(FLOAT x, FLOAT y, FLOAT z) const
{
    switch (this->__state)
    {
        case STATE_NORMAL:
            return this->is_point_inside
            (
                x - this->__o->x,
                y - this->__o->y,
                z - this->__o->z,
                this->__s
            );

        case STATE_ROTATED:
        {
            FLOAT
                dx = x - this->__o->x,
                dy = y - this->__o->y,
                dz = z - this->__o->z;

            return this->is_point_inside
            (
                dx*this->__x->x + dy*this->__x->y + dz*this->__x->z,
                dx*this->__y->x + dy*this->__y->y + dz*this->__y->z,
                dx*this->__z->x + dy*this->__z->y + dz*this->__z->z,
                this->__s
            );
        }
        case STATE_OBLIQUE:
        {
            FLOAT
                dx = x - this->__o->x,
                dy = y - this->__o->y,
                dz = z - this->__o->z;

            return this->is_point_inside
            (
                GetObliqueComponentLength(this->__x, this->__y, this->__z, dx, dy, dz),
                GetObliqueComponentLength(this->__y, this->__x, this->__z, dx, dy, dz),
                GetObliqueComponentLength(this->__z, this->__x, this->__y, dx, dy, dz),
                this->__s
            );
        }
    }

    return false;
}
inline bool __::contains(VEC const &v) const
{
    return this->contains(v.x, v.y, v.z);
}

__::~__()
{
}