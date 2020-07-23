#pragma once
#ifndef __AGD__SPACE__ORIENTATION_HPP__
#define __AGD__SPACE__ORIENTATION_HPP__

#include "vector3D.hpp"

namespace Space
{
    #define __ Orientation
    class __
    {
        void update(VEC *x = nullptr, VEC *y = nullptr, VEC *z = nullptr);
        void replace(VEC *x = nullptr, VEC *y = nullptr, VEC *z = nullptr);
        void update_state();

    protected:

        VEC
            *__x,
            *__y,
            *__z;

        unsigned short __state;

    public:

        enum
        {
            STATE_NORMAL,
            STATE_ROTATED,
            STATE_OBLIQUE
        };

        VEC &x() const;
        VEC &y() const;
        VEC &z() const;

        unsigned short state() const;

        virtual __ &orient(FLOAT xi, FLOAT xj, FLOAT xk, FLOAT yi, FLOAT yj, FLOAT yk, FLOAT zi, FLOAT zj, FLOAT zk);
        __ &orient(VEC const &x, VEC const &y, VEC const &z);

        __ &rotate(FLOAT i, FLOAT j, FLOAT k, FLOAT rad);
        __ &rotate(VEC const &axis, FLOAT rad);

        __ &operator=(__ const &orientation);
        __ &operator=(__ &&orientation);

        bool operator==(__ const &orientation);
        bool operator!=(__ const &orientation);

        __(FLOAT xi, FLOAT xj, FLOAT xk, FLOAT yi, FLOAT yj, FLOAT yk, FLOAT zi, FLOAT zj, FLOAT zk);
        __(VEC const &x, VEC const &y, VEC const &z);
        __(__ const &orientation);
        __(__ &&orientation);
        __();
        virtual ~__();
    };
    #undef __
}

#endif
#pragma endregion