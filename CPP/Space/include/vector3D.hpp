#pragma once
#ifndef __VECTOR3D_HPP__
#define __VECTOR3D_HPP__

#define __ Vector3D

class Vector3D
{
public:

    typename FLOAT double

    struct Constants
    {
        static __ &null() const;

        static __ &i() const;
        static __ &j() const;
        static __ &k() const;
    };

    static __ sum(__ const &v, __ const &w);
    static __ difference(__ const &v, __ const &w);

    static FLOAT scalar_product(__ const &v, __ const &w);
    static FLOAT scalar_triple_product(__ const &u, __ const &v, __ const &w);

    static __ vector_product(__ const &v, __ const &w);
    static __ vector_triple_product(__ const &u, __ const &v, __ const &w);

    static __ scaled(__ const &v, __ const &w);
    static __ scaled(__ const &v, FLOAT a, FLOAT b, FLOAT c);
    static __ scaled(__ const &v, FLOAT f);

    static __ normalized(__ const &v);
    static __ inverted(__ const &v);

    FLOAT x;
    FLOAT y;
    FLOAT z;

    __(FLOAT x, FLOAT y, FLOAT z);
    __(__ const &v);
    __();
    ~__();

    FLOAT length() const;
    FLOAT square() const;
    FLOAT dot(__ const &v) const;
    FLOAT get_angle(__ const &v) const;

    __ &update(__ const &v);
    __ &update(FLOAT x, FLOAT y, FLOAT z);

    __ &scale(__ const &v);
    __ &scale(FLOAT f);
    __ &scale(FLOAT a, FLOAT b, FLOAT c);

    __ &normalize();

    __ &add(__ const &v);
    __ &subtract(__ const &v);

    __ &cross(__ const &v);

    __ &project_to_vector(__ const &v);
    __ &project_to_plane(__ const &n);

    __ &rotate(__ const &axis, FLOAT rad);
    __ &rotate(FLOAT i, FLOAT j, FLOAT k, FLOAT rad);

    __ &operator=(__ const &v);

    __ &operator+=(__ const &v);
    __ &operator-=(__ const &v);
    __ &operator*=(__ const &v);

    __ &operator*=(FLOAT f);
    __ &operator/=(FLOAT f);

    __ operator+(__ const &v);
    __ operator-(__ const &v);
    __ operator*(__ const &v);

    __ operator*(FLOAT f);
    __ operator/(FLOAT f);

    __ operator-();

    bool operator==(__ const &v) const;
    bool operator!=(__ const &v) const;
};

#undef __

#endif
#pragma endregion