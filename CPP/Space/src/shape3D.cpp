#include <cmath>
#include "vector3D.hpp"
#include "shape3D.hpp"

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
*   Space class definition
*/
#define __ Shape3D

void __::update(VEC *o, VEC *s)
{
    this->__o = o;
    this->__s = s;
}
void __::replace(VEC *o, VEC *s)
{
    delete this->__o;
    delete this->__s;

    this->update(o, s);
}

inline VEC &__::o() const
{
    return *this->__o;
}
inline VEC &__::s() const
{
    return *this->__s;
}

inline __ &__::move(VEC const &v)
{
    return this->move(v.x, v.y, v.z);
}
__ &__::move(FLOAT ox, FLOAT oy, FLOAT oz)
{
    this->__s->update(ox, oy, oz);
    return *this;
}

inline __ &__::scale(VEC const &v)
{
    return this->scale(v.x, v.y, v.z);
}
inline __ &__::scale(FLOAT f)
{
    return this->scale(f, f, f);
}
__ &__::scale(FLOAT a, FLOAT b, FLOAT c)
{
    this->__s->scale(a, b, c);
    return *this;
}

__ &__::orient(FLOAT xi, FLOAT xj, FLOAT xk, FLOAT yi, FLOAT yj, FLOAT yk, FLOAT zi, FLOAT zj, FLOAT zk)
{
    this->Orientation::orient(xi, xj, xk, yi, yj, yk, zi, zj, zk);
    this->__s->scale(sqrt(xi*xi + xj*xj + xk*xk), sqrt(yi*yi + yj*yj + yk*yk), sqrt(zi*zi + zj*zj + zk*zk));

    return *this;
}

__ &__::operator=(__ const &shape)
{
    this->Orientation::operator=(shape);

    this->replace(new VEC(shape.o()), new VEC(shape.s()));
    this->__state = shape.__state;

    return *this;
}
__ &__::operator=(__ &&shape)
{
    this->Orientation::operator=(shape);

    this->replace(shape.__o, shape.__s);
    this->__state = shape.__state;

    shape.update(nullptr);

    return *this;
}

bool __::operator==(__ const &shape)
{
    return this->Orientation::operator==(shape) && this->o() == shape.o() && this->s() == shape.s();
}
inline bool __::operator!=(__ const &shape)
{
    return !(this->operator==(shape));
}

__::__
(
    FLOAT ox, FLOAT oy, FLOAT oz,
    FLOAT xi, FLOAT xj, FLOAT xk,
    FLOAT yi, FLOAT yj, FLOAT yk,
    FLOAT zi, FLOAT zj, FLOAT zk,
    FLOAT a, FLOAT b, FLOAT c
)
    : Orientation(VEC(xi, xj, xk), VEC(yi, yj, yk), VEC(zi, zj, zk)), __o(new VEC(ox, oy, oz)), __s(new VEC(a, b, c))
{
}
__::__(FLOAT ox, FLOAT oy, FLOAT oz, FLOAT a, FLOAT b, FLOAT c)
    : Orientation(), __o(new VEC(ox, oy, oz)), __s(new VEC(a, b, c))
{
}
__::__(VEC const &o, Orientation const &orientation, VEC const &s)
    : Orientation(orientation), __o(new VEC(o)), __s(new VEC(s))
{
}
__::__(VEC const &o, VEC const &x, VEC const &y, VEC const &z, VEC const &s)
    : Orientation(x, y, z), __o(new VEC(o)), __s(new VEC(s))
{
}
__::__(VEC const &o, VEC const &s)
    : Orientation(), __o(new VEC(o)), __s(new VEC(s))
{
}
__::__(__ const &shape)
    : Orientation(shape), __o(new VEC(shape.o())), __s(new VEC(shape.s()))
{
}
__::__(__ &&shape)
    : Orientation(shape), __o(shape.__o), __s(shape.__s)
{
    shape.update(nullptr);
}
__::__()
    : Orientation(), __o(new VEC), __s(new VEC)
{
}
__::~__()
{
    if (this->__o != nullptr)
        this->replace(nullptr);
}