#include "boundary3D.hpp"
#include <cmath>

using namespace Boundary3D;

#define VEC                 Vector3D
#define __                  Boundary3D
#define PI                  3.141592653589793238463
#define THOMSEN_EXPONENT    1.6075

constexpr double COEFFICIENT_ELLIPSOID_VOLUME           = (4./3.)*PI;
constexpr double COEFFICIENT_ELLIPSOID_SURFACE_AREA     = (4.*PI)/pow(3., 1./THOMSEN_EXPONENT);

enum
{
    STATE_NORMAL,
    STATE_ROTATED,
    STATE_OBLIQUE
}

double GetObliqueComponentLength(VEC *u, VEC *v, VEC *w, double dx, double dy, double dz)
{
    double
        rx = v->x + w->x,
        ry = v->y + w->y,
        rz = v->z + w->z,
        r  = (dx*rx + dy*ry + dz*rz)/(rx*rx + ry*ry + rz*rz);

    return (dx - r*rx)/u->x + (dy - r*ry)/u->y + (dz - r*rz)/u->z;
}

/*
*   Bounded abstract class
*/
bool __::Bounded::contains_point(FLOAT x, FLOAT y, FLOAT z)
{
    Space *space = static_cast<Space*>(this);

    switch (space->__state)
    {
        case STATE_NORMAL:
            return this->is_point_inside
            (
                x - space->__o->x,
                y - space->__o->y,
                z - space->__o->z,
                space->__s
            );

        case STATE_ROTATED:
            FLOAT
                dx = x - space->__o->x,
                dy = y - space->__o->y,
                dz = z - space->__o->z;

            return this->is_point_inside
            (
                dx*space->__x->x + dy*space->__x->y + dz*space->__x->z,
                dx*space->__y->x + dy*space->__y->y + dz*space->__y->z,
                dx*space->__z->x + dy*space->__z->y + dz*space->__z->z,
                space->__s
            );

        case STATE_OBLIQUE:
            FLOAT
                dx = x - space->__o->x,
                dy = y - space->__o->y,
                dz = z - space->__o->z;

            return this->is_point_inside
            (
                GetObliqueComponentLength(space->__x, space->__y, space->__z, dx, dy, dz),
                GetObliqueComponentLength(space->__y, space->__x, space->__z, dx, dy, dz),
                GetObliqueComponentLength(space->__z, space->__x, space->__y, dx, dy, dz),
                space->__s
            );
    }

    return false;
}

/*
*   Space class definition
*/

void __::Space::update(VEC *o, VEC *x, VEC *y, VEC *z, VEC *s)
{
    this->__o = o;
    this->__x = x;
    this->__y = y;
    this->__z = z;
    this->__s = s;
}
void __::Space::replace(VEC *o, VEC *x, VEC *y, VEC *z, VEC *s)
{
    delete this->__o;
    delete this->__x;
    delete this->__y;
    delete this->__z;
    delete this->__s;

    this->update(o, x, y, z, s);
}
void __::Space::update_state()
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

inline VEC &__::Space::o()
{
    return *this->__o;
}
inline VEC &__::Space::x()
{
    return *this->__y;
}
inline VEC &__::Space::y()
{
    return *this->__y;
}
inline VEC &__::Space::z()
{
    return *this->__z;
}
inline VEC &__::Space::s()
{
    return *this->__s;
}

bool __::Space::isOblique()
{
    return this->__state == STATE_OBLIQUE;
}
bool __::Space::isRotated()
{
    return this->__state == STATE_ROTATED;
}

inline Space &__::Space::move(VEC const &v)
{
    return this->move(v.x, v.y, v.z);
}
Space &__::Space::move(FLOAT ox, FLOAT oy, FLOAT oz)
{
    this->__s->update(ox, oy, oz);
    return *this;
}

inline Space &__::Space::orient(VEC const &x, VEC const &y, VEC const &z)
{
    return this->orient(x.x, x.y, x.z, y.x, y.y, y.z, z.x, z.y, z.z);
}
Space &__::Space::orient(FLOAT xi, FLOAT xj, FLOAT xk, FLOAT yi, FLOAT yj, FLOAT yk, FLOAT zi, FLOAT zj, FLOAT zk)
{
    this->__x->update(xi, xj, xk);
    this->__y->update(yi, yj, yk);
    this->__z->update(zi, zj, zk);

    FLOAT
        rx = this->__x->length(),
        ry = this->__y->length(),
        rz = this->__z->length();

    this->__s->scale(rx, ry, rz);

    this->__x /= rx;
    this->__y /= ry;
    this->__z /= rz;

    this->update_state();

    return *this;
}

inline Space &__::Space::rotate(VEC const &axis, FLOAT rad)
{
    return this->rotate(axis.x, axis.y, axis.z, rad);
}
Space &__::Space::rotate(FLOAT i, FLOAT j, FLOAT k, FLOAT rad)
{
    this->__x->rotate(i, j, k, rad);
    this->__y->rotate(i, j, k, rad);
    this->__z->rotate(i, j, k, rad);

    return this->orient(this->__x, this->__y, this->__z);
}

inline Space &__::Space::scale(VEC const &v)
{
    return this->__s->scale(v);
}
inline Space &__::Space::scale(FLOAT a, FLOAT b, FLOAT c)
{
    return this->__s->scale(a, b, c);
}
inline Space &__::Space::scale(FLOAT f)
{
    return this->__s->scale(f);
}

Space &__::Space::operator=(Space const &space)
{
    this->replace
    (
        new VEC(space.o()),
        new VEC(space.x()),
        new VEC(space.y()),
        new VEC(space.z()),
        new VEC(space.s())
    );
    this->__state = space.__state;

    return *this;
}
Space &__::Space::operator=(Space &&space)
{
    this->replace
    (
        space.__o,
        space.__x,
        space.__y,
        space.__z,
        space.__s
    );
    this->__state = space.__state;

    space.update(nullptr);

    return *this;
}

bool __::Space::operator==(Space const &space)
{
    return
        this->o() == space.o() &&
        this->x() == space.x() &&
        this->y() == space.y() &&
        this->z() == space.z() &&
        this->s() == space.s();
}
inline bool __::Space::operator!=(Space const &space)
{
    return !(this->operator==(space));
}

__::Space::Space
(
    FLOAT ox, FLOAT oy, FLOAT oz,
    FLOAT xi, FLOAT xj, FLOAT xk,
    FLOAT yi, FLOAT yj, FLOAT yk,
    FLOAT zi, FLOAT zj, FLOAT zk,
    FLOAT a, FLOAT b, FLOAT c
)
    : __o(new VEC(ox, oy, oz)), __x(new VEC(xi, xj, xk)), __y(new VEC(yi, yj, yk)), __z(new VEC(zi, zj, zk)), __o(new VEC(a, b, c))
{
    this->update_state();
}
__::Space::Space(FLOAT ox, FLOAT oy, FLOAT oz, FLOAT a, FLOAT b, FLOAT c)
    : __o(new VEC(ox, oy, oz)), __x(new VEC), __y(new VEC), __z(new VEC), __s(new VEC(a, b, c)), __state(STATE_NORMAL)
{
}
__::Space::Space(VEC const &o, VEC const &x, VEC const &y, VEC const &z, VEC const &s)
    : __o(new VEC(o)), __x(new VEC(x)), __y(new VEC(y)), __z(new VEC(z)), __s(new VEC(s))
{
    this->update_state();
}
__::Space::Space(VEC const &o, VEC const &s)
    : __o(new VEC(o)), __x(new VEC), __y(new VEC), __z(new VEC), __s(new VEC(s)), __state(STATE_NORMAL)
{
}
__::Space::Space(Space const &space)
    : __o(new VEC(space.o())), __x(new VEC(space.x())), __y(new VEC(space.y())), __z(new VEC(space.z())), __s(new VEC(space.s())), __state(space.__state)
{
}
__::Space::Space(Space &&space)
    : __o(space.__o), __x(space.__x), __y(space.__y), __z(space.__z), __s(space.__s), __state(space.__state)
{
    space.update(nullptr);
}
__::Space::Space()
    : __o(new VEC), __x(new VEC), __y(new VEC), __z(new VEC), __s(new VEC), __state(STATE_NORMAL)
{
}
__::Space::~Space()
{
    if this->o != nullptr
        this->replace(nullptr);
}

/*
*   Ellipsoid class definition
*/

inline bool __::Ellipsoid::is_point_inside(FLOAT dx, FLOAT dy, FLOAT dz, VEC *s)
{
    return (dx*dx)/(s->x*s->x) + (dy*dy)/(s->y*s->y) + (dz*dz)/(s->z*s->z) <= 1.;
}

inline FLOAT __::Ellipsoid::volume()
{
    return COEFFICIENT_ELLIPSOID_VOLUME*(this->__s->x*this->__s->y*this->__s->z);
}
FLOAT __::Ellipsoid::surface_area()
{
    FLOAT
        ap = pow(this->__s->x, THOMSEN_EXPONENT),
        bp = pow(this->__s->y, THOMSEN_EXPONENT),
        cp = pow(this->__s->z, THOMSEN_EXPONENT);

    return COEFFICIENT_ELLIPSOID_SURFACE_AREA*pow(ap*bp + ap*cp + bp*cp, 1./THOMSEN_EXPONENT);
}

inline bool __::Ellipsoid::contains(VEC const &v)
{
    return this->contains(v.x, v.y, v.z);
}

__::Ellipsoid::Ellipsoid(Ellipsoid const &ellipsoid)
    : Space(ellipsoid)
{
}
__::Ellipsoid::Ellipsoid(Ellipsoid &&ellipsoid)
    : Space(ellipsoid)
{
}

/*
*   Prism class definition
*/

inline bool __::Prism::is_point_inside(FLOAT dx, FLOAT dy, FLOAT dz, VEC *s)
{
    return dx*dx <= s->x*s->x && dy*dy <= s->y*s->y && dz*dz <= s->z*s->z;
}

inline FLOAT __::Prism::volume()
{
    return 8.*(this->__s->x*this->__s->y*this->__s->z);
}
inline FLOAT __::Prism::surface_area()
{
    return 8.*(this->__s->x*this->__s->y + this->__s->x*this->__s->z + this->__s->y*this->__s->z);
}

inline bool __::Prism::contains(VEC const &v)
{
    return this->contains(v.x, v.y, v.z);
}

__::Prism::Prism(Prism const &prism)
    : Space(prism)
{
}
__::Prism::Prism(Prism &&prism)
    : Space(prism)
{
}

/*
*   Cone class definition
*/

bool __::Cone::is_point_inside(FLOAT dx, FLOAT dy, FLOAT dz, VEC *s)
{
    FLOAT rf = 1. - dz/s->z;
    return
        rf >= 0. && r <= 1. &&
        (dx*dx)/(s->x*s->x) + (dy*dy)/(s->y*s->y) <= rf*rf;
}

inline FLOAT __::Cone::volume()
{
    return (PI/3.)*(this->__s->x*this->__s->y*this->__s->z);
}
inline FLOAT __::Cone::surface_area()
{
    return (0.5*PI)*(3.*(this->__s->x + this->__s->y) - sqrt((3.*this->__s->x + this->__s->y)*(this->__s->x + 3.*this->__s->y)))*this->__s->z;
}

inline bool __::Cone::contains(VEC const &v)
{
    return this->contains(v.x, v.y, v.z);
}

__::Cone::Cone(Cone const &cone)
    : Space(cone)
{
}
__::Cone::Cone(Cone &&cone)
    : Space(cone)
{
}

/*
*   Cylinder class definition
*/

inline bool __::Cylinder::is_point_inside(FLOAT dx, FLOAT dy, FLOAT dz, VEC *s)
{
    return
        dz >= 0. && dz <= s->z &&
        (dx*dx)/(s->x*s->x) + (dy*dy)/(s->y*s->y) <= 1.;
}

inline FLOAT __::Cylinder::volume()
{
    return PI*(this->__s->x*this->__s->y*this->__s->z);
}
inline FLOAT __::Cylinder::surface_area()
{
    return PI*(3.*(this->__s->x + this->__s->y) - sqrt((3.*this->__s->x + this->__s->y)*(this->__s->x + 3.*this->__s->y)))*this->__s->z;
}

inline bool __::Cylinder::contains(VEC const &v)
{
    return this->contains(v.x, v.y, v.z);
}

__::Cylinder::Cylinder(Cylinder const &cylinder)
    : Space(cylinder)
{
}
__::Cylinder::Cylinder(Cylinder &&cylinder)
    : Space(cylinder)
{
}