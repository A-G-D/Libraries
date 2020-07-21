#pragma once
#ifndef __VECTOR3D_HPP__
#define __VECTOR3D_HPP__

#define VEC Vector3D

class Vector3D
{
public:

    typedef double FLOAT;

    struct Constants
    {
        static VEC &null();

        static VEC &i();
        static VEC &j();
        static VEC &k();
    };

    static VEC sum(VEC const &v, VEC const &w);
    static VEC difference(VEC const &v, VEC const &w);

    static FLOAT scalar_product(VEC const &v, VEC const &w);
    static FLOAT scalar_triple_product(VEC const &u, VEC const &v, VEC const &w);

    static VEC vector_product(VEC const &v, VEC const &w);
    static VEC vector_triple_product(VEC const &u, VEC const &v, VEC const &w);

    static VEC scaled(VEC const &v, VEC const &w);
    static VEC scaled(VEC const &v, FLOAT a, FLOAT b, FLOAT c);
    static VEC scaled(VEC const &v, FLOAT f);

    static VEC normalized(VEC const &v);
    static VEC inverted(VEC const &v);

    FLOAT x;
    FLOAT y;
    FLOAT z;

    VEC(FLOAT x, FLOAT y, FLOAT z);
    VEC(VEC const &v);
    VEC();
    ~VEC();

    FLOAT length() const;
    FLOAT square() const;
    FLOAT dot(VEC const &v) const;
    FLOAT get_angle(VEC const &v) const;

    VEC &update(VEC const &v);
    VEC &update(FLOAT x, FLOAT y, FLOAT z);

    VEC &scale(VEC const &v);
    VEC &scale(FLOAT f);
    VEC &scale(FLOAT a, FLOAT b, FLOAT c);

    VEC &normalize();

    VEC &add(VEC const &v);
    VEC &subtract(VEC const &v);

    VEC &cross(VEC const &v);

    VEC &project_to_vector(VEC const &v);
    VEC &project_to_plane(VEC const &n);

    VEC &rotate(VEC const &axis, FLOAT rad);
    VEC &rotate(FLOAT i, FLOAT j, FLOAT k, FLOAT rad);

    VEC &operator=(VEC const &v);

    VEC &operator+=(VEC const &v);
    VEC &operator-=(VEC const &v);
    VEC &operator*=(VEC const &v);

    VEC &operator*=(FLOAT f);
    VEC &operator/=(FLOAT f);

    VEC operator+(VEC const &v) const;
    VEC operator-(VEC const &v) const;
    VEC operator*(VEC const &v) const;

    VEC operator*(FLOAT f) const;
    VEC operator/(FLOAT f) const;

    VEC operator-() const;

    bool operator==(VEC const &v) const;
    bool operator!=(VEC const &v) const;
};

#undef VEC

#endif
#pragma endregion