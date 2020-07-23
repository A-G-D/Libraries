#pragma once
#ifndef __AGD__SPACE__VECTOR3D_HPP__
#define __AGD__SPACE__VECTOR3D_HPP__

#include "space.hpp"

namespace Space
{
    #define __ Vector3D
    class __
    {
    public:

        struct Constants
        {
            static __ &null();

            static __ &i();
            static __ &j();
            static __ &k();
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

        Vector3D(FLOAT x, FLOAT y, FLOAT z);
        Vector3D(__ const &v);
        Vector3D();
        ~Vector3D();

        FLOAT length() const;
        FLOAT square() const;
        FLOAT dot(__ const &v) const;
        FLOAT get_angle(__ const &v) const;

        __ &update(FLOAT x, FLOAT y, FLOAT z);
        __ &update(__ const &v);

        __ &scale(FLOAT f);
        __ &scale(FLOAT a, FLOAT b, FLOAT c);
        __ &scale(__ const &v);

        __ &normalize();

        __ &add(__ const &v);
        __ &subtract(__ const &v);

        __ &cross(__ const &v);

        __ &project_to_vector(__ const &v);
        __ &project_to_plane(__ const &n);

        __ &rotate(FLOAT i, FLOAT j, FLOAT k, FLOAT rad);
        __ &rotate(__ const &axis, FLOAT rad);

        __ &operator=(__ const &v);

        __ &operator+=(__ const &v);
        __ &operator-=(__ const &v);
        __ &operator*=(__ const &v);

        __ &operator*=(FLOAT f);
        __ &operator/=(FLOAT f);

        __ operator+(__ const &v) const;
        __ operator-(__ const &v) const;
        __ operator*(__ const &v) const;

        __ operator*(FLOAT f) const;
        __ operator/(FLOAT f) const;

        __ operator-() const;

        bool operator==(__ const &v) const;
        bool operator!=(__ const &v) const;
    };
    #undef __
}

#endif
#pragma endregion