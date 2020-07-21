#include "vector3D.hpp"
#include <cmath>

#define VEC Vector3D

/*
*   Constant vectors
*/

VEC &VEC::Constants::null()
{
    static VEC zero;
    return zero;
}

VEC &VEC::Constants::i()
{
    static VEC i(1.00, 0.00, 0.00);
    return i;
}
VEC &VEC::Constants::j()
{
    static VEC j(0.00, 1.00, 0.00);
    return j;
}
VEC &VEC::Constants::k()
{
    static VEC k(0.00, 0.00, 1.00);
    return k;
}

/*
*   Static members
*/

inline VEC VEC::sum(VEC const &v, VEC const &w)
{
    return v + w;
}
inline VEC VEC::difference(VEC const &v, VEC const &w)
{
    return v - w;
}

inline VEC::FLOAT VEC::scalar_product(VEC const &v, VEC const &w)
{
    return v.dot(w);
}
inline VEC::FLOAT VEC::scalar_triple_product(VEC const &u, VEC const &v, VEC const &w)
{
    return vector_product(u, v).dot(w);
}

inline VEC VEC::vector_product(VEC const &v, VEC const &w)
{
    return v*w;
}
inline VEC VEC::vector_triple_product(VEC const &u, VEC const &v, VEC const &w)
{
    return scaled(v, scalar_product(u, w)).subtract(scaled(w, scalar_product(u, v)));
}

inline VEC VEC::scaled(VEC const &v, VEC const &w)
{
    return VEC(v.x*w.x, v.y*w.y, v.z*w.z);
}
inline VEC VEC::scaled(VEC const &v, FLOAT a, FLOAT b, FLOAT c)
{
    return VEC(v.x*a, v.y*b, v.z*c);
}
inline VEC VEC::scaled(VEC const &v, FLOAT f)
{
    return v*f;
}

inline VEC VEC::normalized(VEC const &v)
{
    return scaled(v, 1./v.length());
}
inline VEC VEC::inverted(VEC const &v)
{
    return -v;
}

/*
*   Instance members
*/

VEC::VEC(FLOAT x, FLOAT y, FLOAT z)
    : x(x), y(y), z(z)
{
}
VEC::VEC(VEC const &v)
    : x(v.x), y(v.y), z(v.z)
{
}
VEC::VEC()
    : x(0.00), y(0.00), z(0.00)
{
}
VEC::~VEC()
{
}

inline VEC::FLOAT VEC::length() const
{
    return sqrt(this->square());
}
inline VEC::FLOAT VEC::square() const
{
    return this->x*this->x + this->y*this->y + this->z*this->z;
}
inline VEC::FLOAT VEC::dot(VEC const &v) const
{
    return this->x*v.x + this->y*v.y + this->z*v.z;
}
inline VEC::FLOAT VEC::get_angle(VEC const &v) const
{
    return acos(this->dot(v)/(this->length()*v.length()));
}

inline VEC &VEC::update(VEC const &v)
{
    this->update(v.x, v.y, v.z);
}
VEC &VEC::update(FLOAT x, FLOAT y, FLOAT z)
{
    this->x = x;
    this->y = y;
    this->z = z;
    return *this;
}
inline VEC &VEC::scale(FLOAT f)
{
    return this->update(this->x*f, this->y*f, this->z*f);
}
inline VEC &VEC::scale(FLOAT a, FLOAT b, FLOAT c)
{
    return this->update(this->x*a, this->y*b, this->z*c);
}
inline VEC &VEC::scale(VEC const &v)
{
    return this->update(this->x*v.x, this->y*v.y, this->z*v.z);
}
inline VEC &VEC::normalize()
{
    return this->scale(1./this->length());
}

inline VEC &VEC::add(VEC const &v)
{
    return this->update(this->x + v.x, this->y + v.y, this->z + v.z);
}
inline VEC &VEC::subtract(VEC const &v)
{
    return this->update(this->x - v.x, this->y - v.y, this->z - v.z);
}

inline VEC &VEC::cross(VEC const &v)
{
    return this->update(this->y*v.z - this->z*v.y, this->z*v.x - this->x*v.z, this->x*v.y - this->y*v.x);
}

VEC &VEC::project_to_vector(VEC const &v)
{
    FLOAT l = this->dot(v)/v.square();
    return this->update(l*v.x, l*v.y, l*v.z);
}
VEC &VEC::project_to_plane(VEC const &n)
{
    FLOAT l = this->dot(n)/n.square();
    return this->update(this->x - l*n.x, this->y - l*n.y, this->z - l*n.z);
}

inline VEC &VEC::rotate(VEC const &axis, FLOAT rad)
{
    return this->rotate(axis.x, axis.y, axis.z, rad);
}
VEC &VEC::rotate(FLOAT i, FLOAT j, FLOAT k, FLOAT rad)
{
    FLOAT
        al      = i*i + j*j + k*k,
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

inline VEC &VEC::operator=(VEC const &v)
{
    return this->update(v.x, v.y, v.z);
}

inline VEC &VEC::operator+=(VEC const &v)
{
    return this->add(v);
}
inline VEC &VEC::operator-=(VEC const &v)
{
    return this->subtract(v);
}
inline VEC &VEC::operator*=(VEC const &v)
{
    return this->cross(v);
}

inline VEC &VEC::operator*=(FLOAT f)
{
    return this->scale(f);
}
inline VEC &VEC::operator/=(FLOAT f)
{
    return this->scale(1./f);
}

inline VEC VEC::operator+(VEC const &v) const
{
    return VEC(this->x + v.x, this->y + v.y, this->z + v.z);
}
inline VEC VEC::operator-(VEC const &v) const
{
    return VEC(this->x - v.x, this->y - v.y, this->z - v.z);
}
inline VEC VEC::operator*(VEC const &v) const
{
    return VEC(this->y*v.z - this->z*v.y, this->z*v.x - this->x*v.z, this->x*v.y - this->y*v.x);
}

inline VEC VEC::operator*(FLOAT f) const
{
    return VEC(this->x*f, this->y*f, this->z*f);
}
inline VEC VEC::operator/(FLOAT f) const
{
    return VEC(this->x/f, this->y/f, this->z/f);
}

inline VEC VEC::operator-() const
{
    return VEC(-(this->x), -(this->y), -(this->z));
}

inline bool VEC::operator==(VEC const &v) const
{
    return this->x == v.x && this->y == v.y && this->z == v.z;
}
inline bool VEC::operator!=(VEC const &v) const
{
    return this->x != v.x || this->y != v.y || this->z != v.z;
}