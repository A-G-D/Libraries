#include <cmath>
#include <stdexcept>
#include "vector3D.hpp"

using namespace Space;

#define __ Vector3D

/*
*   Constant vectors
*/

__ &__::Constants::null()
{
    static __ zero;
    return zero;
}

__ &__::Constants::i()
{
    static __ i(1.00, 0.00, 0.00);
    return i;
}
__ &__::Constants::j()
{
    static __ j(0.00, 1.00, 0.00);
    return j;
}
__ &__::Constants::k()
{
    static __ k(0.00, 0.00, 1.00);
    return k;
}

/*
*   Static members
*/

inline __ __::sum(__ const &v, __ const &w)
{
    return v + w;
}
inline __ __::difference(__ const &v, __ const &w)
{
    return v - w;
}

inline FLOAT __::scalar_product(__ const &v, __ const &w)
{
    return v.dot(w);
}
inline FLOAT __::scalar_triple_product(__ const &u, __ const &v, __ const &w)
{
    return vector_product(u, v).dot(w);
}

inline __ __::vector_product(__ const &v, __ const &w)
{
    return v*w;
}
inline __ __::vector_triple_product(__ const &u, __ const &v, __ const &w)
{
    return scaled(v, scalar_product(u, w)).subtract(scaled(w, scalar_product(u, v)));
}

inline __ __::scaled(__ const &v, __ const &w)
{
    return __(v.x*w.x, v.y*w.y, v.z*w.z);
}
inline __ __::scaled(__ const &v, FLOAT a, FLOAT b, FLOAT c)
{
    return __(v.x*a, v.y*b, v.z*c);
}
inline __ __::scaled(__ const &v, FLOAT f)
{
    return v*f;
}

inline __ __::normalized(__ const &v)
{
    FLOAT l = v.length();

    if (l == 0.)
        throw std::runtime_error("Attempted to operate on null vector");

    return scaled(v, 1./l);
}
inline __ __::inverted(__ const &v)
{
    return -v;
}

/*
*   Instance members
*/

__::__(FLOAT x, FLOAT y, FLOAT z)
    : x(x), y(y), z(z)
{
}
__::__(__ const &v)
    : x(v.x), y(v.y), z(v.z)
{
}
__::__()
    : x(0.00), y(0.00), z(0.00)
{
}
__::~__()
{
}

inline FLOAT __::length() const
{
    return sqrt(this->square());
}
inline FLOAT __::square() const
{
    return this->x*this->x + this->y*this->y + this->z*this->z;
}
inline FLOAT __::dot(__ const &v) const
{
    return this->x*v.x + this->y*v.y + this->z*v.z;
}
inline FLOAT __::get_angle(__ const &v) const
{
    if (this->length() == 0. || v.length() == 0.)
        throw std::runtime_error("Division by zero caused by null vectors");

    return acos(this->dot(v)/(this->length()*v.length()));
}

__ &__::update(FLOAT x, FLOAT y, FLOAT z)
{
    this->x = x;
    this->y = y;
    this->z = z;
    return *this;
}
inline __ &__::update(__ const &v)
{
    this->update(v.x, v.y, v.z);
}

inline __ &__::scale(FLOAT f)
{
    return this->update(this->x*f, this->y*f, this->z*f);
}
inline __ &__::scale(FLOAT a, FLOAT b, FLOAT c)
{
    return this->update(this->x*a, this->y*b, this->z*c);
}
inline __ &__::scale(__ const &v)
{
    return this->update(this->x*v.x, this->y*v.y, this->z*v.z);
}

inline __ &__::normalize()
{
    FLOAT l = this->length();
    
    if (l == 0.)
        throw std::runtime_error("Division by zero");

    return this->scale(1./l);
}

inline __ &__::add(__ const &v)
{
    return this->update(this->x + v.x, this->y + v.y, this->z + v.z);
}
inline __ &__::subtract(__ const &v)
{
    return this->update(this->x - v.x, this->y - v.y, this->z - v.z);
}

inline __ &__::cross(__ const &v)
{
    return this->update(this->y*v.z - this->z*v.y, this->z*v.x - this->x*v.z, this->x*v.y - this->y*v.x);
}

__ &__::project_to_vector(__ const &v)
{
    FLOAT square = v.square();

    if (square == 0.)
        throw std::runtime_error("Division by zero");

    FLOAT l = this->dot(v)/square;
    return this->update(l*v.x, l*v.y, l*v.z);
}
__ &__::project_to_plane(__ const &n)
{
    FLOAT square = n.square();

    if (square == 0.)
        throw std::runtime_error("Division by zero");

    FLOAT l = this->dot(n)/square;
    return this->update(this->x - l*n.x, this->y - l*n.y, this->z - l*n.z);
}

__ &__::rotate(FLOAT i, FLOAT j, FLOAT k, FLOAT rad)
{
    FLOAT al    = i*i + j*j + k*k;

    if (al == 0.)
        throw std::runtime_error("Division by zero");

    FLOAT
        factor  = (this->x*i + this->y*j + this->z*k)/al,
        zx      = i*factor,
        zy      = j*factor,
        zz      = k*factor,
        xx      = this->x - zx,
        xy      = this->y - zy,
        xz      = this->z - zz,
        cosine  = cos(rad),
        sine    = sin(rad);
    al          = sqrt(al);

    return this->update
    (
        xx*cosine + ((j*xz - k*xy)/al)*sine + zx,
        xy*cosine + ((k*xx - i*xz)/al)*sine + zy,
        xz*cosine + ((i*xy - j*xx)/al)*sine + zz
    );
}
inline __ &__::rotate(__ const &axis, FLOAT rad)
{
    return this->rotate(axis.x, axis.y, axis.z, rad);
}

inline __ &__::operator=(__ const &v)
{
    return this->update(v.x, v.y, v.z);
}

inline __ &__::operator+=(__ const &v)
{
    return this->add(v);
}
inline __ &__::operator-=(__ const &v)
{
    return this->subtract(v);
}
inline __ &__::operator*=(__ const &v)
{
    return this->cross(v);
}

inline __ &__::operator*=(FLOAT f)
{
    return this->scale(f);
}
inline __ &__::operator/=(FLOAT f)
{
    if (f == 0.)
        throw std::runtime_error("Division by zero");

    return this->scale(1./f);
}

inline __ __::operator+(__ const &v) const
{
    return __(this->x + v.x, this->y + v.y, this->z + v.z);
}
inline __ __::operator-(__ const &v) const
{
    return __(this->x - v.x, this->y - v.y, this->z - v.z);
}
inline __ __::operator*(__ const &v) const
{
    return __(this->y*v.z - this->z*v.y, this->z*v.x - this->x*v.z, this->x*v.y - this->y*v.x);
}

inline __ __::operator*(FLOAT f) const
{
    return __(this->x*f, this->y*f, this->z*f);
}
inline __ __::operator/(FLOAT f) const
{
    if (f == 0.)
        throw std::runtime_error("Division by zero");

    return __(this->x/f, this->y/f, this->z/f);
}

inline __ __::operator-() const
{
    return __(-(this->x), -(this->y), -(this->z));
}

inline bool __::operator==(__ const &v) const
{
    return this->x == v.x && this->y == v.y && this->z == v.z;
}
inline bool __::operator!=(__ const &v) const
{
    return this->x != v.x || this->y != v.y || this->z != v.z;
}