#pragma once
#ifndef __BOUNDARY3D_HPP__
#define __BOUNDARY3D_HPP__

#include "vector3D.hpp"

#define VEC Vector3D

namespace Boundary3D
{
    typedef double FLOAT;

    class Space
    {
        typedef Space __;

        void update(VEC *o = nullptr, VEC *x = nullptr, VEC *y = nullptr, VEC *z = nullptr, VEC *s = nullptr);
        void replace(VEC *o = nullptr, VEC *x = nullptr, VEC *y = nullptr, VEC *z = nullptr, VEC *s = nullptr);
        void update_state();

    protected:

        VEC
            *__o,
            *__x,
            *__y,
            *__z,
            *__s;

        unsigned short __state;

    public:

        VEC &o() const;
        VEC &x() const;
        VEC &y() const;
        VEC &z() const;
        VEC &s() const;

        bool isOblique();
        bool isRotated();

        virtual __ &move(VEC const &v);
        virtual __ &move(FLOAT ox, FLOAT oy, FLOAT oz);

        virtual __ &orient(VEC const &x, VEC const &y, VEC const &z);
        virtual __ &orient(FLOAT xi, FLOAT xj, FLOAT xk, FLOAT yi, FLOAT yj, FLOAT yk, FLOAT zi, FLOAT zj, FLOAT zk);

        virtual __ &rotate(VEC const &axis, FLOAT rad);
        virtual __ &rotate(FLOAT i, FLOAT j, FLOAT k, FLOAT rad);

        virtual __ &scale(VEC const &v);
        virtual __ &scale(FLOAT f);
        virtual __ &scale(FLOAT a, FLOAT b, FLOAT c);

        virtual __ &operator=(__ const &space);
        virtual __ &operator=(__ &&space);

        bool operator==(__ const &space);
        bool operator!=(__ const &space);

        Space
        (
            FLOAT ox, FLOAT oy, FLOAT oz,
            FLOAT xi, FLOAT xj, FLOAT xk,
            FLOAT yi, FLOAT yj, FLOAT yk,
            FLOAT zi, FLOAT zj, FLOAT zk,
            FLOAT a, FLOAT b, FLOAT c
        );
        Space(FLOAT ox, FLOAT oy, FLOAT oz, FLOAT a, FLOAT b, FLOAT c);
        Space(VEC const &o, VEC const &x, VEC const &y, VEC const &z, VEC const &s);
        Space(VEC const &o, VEC const &s);
        Space(__ const &space);
        Space(__ &&space);
        Space();
        virtual ~Space();
    };

    class Bounded : public virtual Space
    {
        virtual bool is_point_inside(FLOAT dx, FLOAT dy, FLOAT dz, VEC *s) const = 0;

    public:

        virtual FLOAT volume() const = 0;
        virtual FLOAT surface_area() const = 0;

        virtual bool contains(FLOAT x, FLOAT y, FLOAT z) const;
    };

    class Ellipsoid : public virtual Bounded, public virtual Space
    {
        typedef Ellipsoid __;

        virtual bool is_point_inside(FLOAT dx, FLOAT dy, FLOAT dz, VEC *s) const override;

    public:

        virtual FLOAT volume() const override;
        virtual FLOAT surface_area() const override;

        bool contains(VEC const &v) const;

        using Space::Space;

        Ellipsoid(__ const &ellipsoid);
        Ellipsoid(__ &&ellipsoid);
    };

    class Prism : public virtual Bounded, public virtual Space
    {
        typedef Prism __;

        virtual bool is_point_inside(FLOAT dx, FLOAT dy, FLOAT dz, VEC *s) const override;

    public:

        virtual FLOAT volume() const override;
        virtual FLOAT surface_area() const override;

        bool contains(VEC const &v) const;

        using Space::Space;

        Prism(__ const &prism);
        Prism(__ &&prism);
    };

    class Cone : public virtual Bounded, public virtual Space
    {
        typedef Cone __;

        virtual bool is_point_inside(FLOAT dx, FLOAT dy, FLOAT dz, VEC *s) const override;

    public:

        virtual FLOAT volume() const override;
        virtual FLOAT surface_area() const override;

        bool contains(VEC const &v) const;

        using Space::Space;

        Cone(__ const &cone);
        Cone(__ &&cone);
    };

    class Cylinder : public virtual Bounded, public virtual Space
    {
        typedef Cylinder __;

        virtual bool is_point_inside(FLOAT dx, FLOAT dy, FLOAT dz, VEC *s) const override;

    public:

        virtual FLOAT volume() const override;
        virtual FLOAT surface_area() const override;

        bool contains(VEC const &v) const;

        using Space::Space;

        Cylinder(__ const &cylinder);
        Cylinder(__ &&cylinder);
    };
}

#undef VEC

#endif
#pragma endregion