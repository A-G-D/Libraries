#include "orientation.hpp"
#include "vector3D.hpp"

using namespace Space;

#define __ Orientation

void __::update(VEC *x, VEC *y, VEC *z)
{
    this->__x = x;
    this->__y = y;
    this->__z = z;
}
void __::replace(VEC *x, VEC *y, VEC *z)
{
    delete this->__x;
    delete this->__y;
    delete this->__z;

    this->update(x, y, z);
}
void __::update_state()
{
    if
    (
        VEC::scalar_product(this->x(), this->y()) != 0. ||
        VEC::scalar_product(this->x(), this->z()) != 0. ||
        VEC::scalar_product(this->y(), this->z()) != 0.
    )
        this->__state = STATE_OBLIQUE;

    else if
    (
        this->x() != VEC::Constants::i() ||
        this->y() != VEC::Constants::j() ||
        this->z() != VEC::Constants::k()
    )
        this->__state = STATE_ROTATED;

    else
        this->__state = STATE_NORMAL;
}

inline VEC &__::x() const
{
    return *this->__y;
}
inline VEC &__::y() const
{
    return *this->__y;
}
inline VEC &__::z() const
{
    return *this->__z;
}

inline unsigned short __::state() const
{
    return this->__state;
}

inline __ &__::orient(VEC const &x, VEC const &y, VEC const &z)
{
    return this->orient(x.x, x.y, x.z, y.x, y.y, y.z, z.x, z.y, z.z);
}
__ &__::orient(FLOAT xi, FLOAT xj, FLOAT xk, FLOAT yi, FLOAT yj, FLOAT yk, FLOAT zi, FLOAT zj, FLOAT zk)
{
    this->__x->update(xi, xj, xk).normalize();
    this->__y->update(yi, yj, yk).normalize();
    this->__z->update(zi, zj, zk).normalize();

    this->update_state();

    return *this;
}

inline __ &__::rotate(VEC const &axis, FLOAT rad)
{
    return this->rotate(axis.x, axis.y, axis.z, rad);
}
__ &__::rotate(FLOAT i, FLOAT j, FLOAT k, FLOAT rad)
{
    this->__x->rotate(i, j, k, rad);
    this->__y->rotate(i, j, k, rad);
    this->__z->rotate(i, j, k, rad);

    this->update_state();

    return *this;
}

__ &__::operator=(__ const &orientation)
{
    this->replace(new VEC(orientation.x()), new VEC(orientation.y()), new VEC(orientation.z()));
    this->__state = orientation.__state;

    return *this;
}
__ &__::operator=(__ &&orientation)
{
    this->replace(orientation.__x, orientation.__y, orientation.__z);
    this->__state = orientation.__state;

    orientation.update(nullptr);

    return *this;
}

bool __::operator==(__ const &orientation)
{
    return this->x() == orientation.x() && this->y() == orientation.y() && this->z() == orientation.z();
}
inline bool __::operator!=(__ const &orientation)
{
    return !(this->operator==(orientation));
}

__::__(FLOAT xi, FLOAT xj, FLOAT xk, FLOAT yi, FLOAT yj, FLOAT yk, FLOAT zi, FLOAT zj, FLOAT zk)
    : __x(new VEC(xi, xj, xk)), __y(new VEC(yi, yj, yk)), __z(new VEC(zi, zj, zk))
{
    this->update_state();
}
__::__(VEC const &x, VEC const &y, VEC const &z)
    : __x(new VEC(x)), __y(new VEC(y)), __z(new VEC(z))
{
    this->update_state();
}
__::__(__ const &orientation)
    : __x(new VEC(orientation.x())), __y(new VEC(orientation.y())), __z(new VEC(orientation.z())), __state(orientation.__state)
{
}
__::__(__ &&orientation)
    : __x(orientation.__x), __y(orientation.__y), __z(orientation.__z), __state(orientation.__state)
{
    orientation.update(nullptr);
}
__::__()
    : __x(new VEC), __y(new VEC), __z(new VEC), __state(STATE_NORMAL)
{
}
__::~__()
{
    if (this->__x != nullptr)
        this->replace(nullptr);
}