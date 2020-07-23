#pragma once
#ifndef __AGD__SPACE__SHAPE__SHAPE3D_HPP__
#define __AGD__SPACE__SHAPE__SHAPE3D_HPP__

#include "vector3D.hpp"
#include "orientation.hpp"

namespace Space
{
    namespace Shape
    {
        #define __ Shape3D
        class __ : public Orientation
        {
            void update(VEC *o = nullptr, VEC *s = nullptr);
            void replace(VEC *o = nullptr, VEC *s = nullptr);
            void update_state();

        protected:

            VEC
                *__o,
                *__s;

        public:

            VEC &o() const;
            VEC &s() const;

            __ &move(VEC const &v);
            __ &move(FLOAT ox, FLOAT oy, FLOAT oz);

            __ &scale(VEC const &v);
            __ &scale(FLOAT f);
            __ &scale(FLOAT a, FLOAT b, FLOAT c);

            virtual __ &orient(FLOAT xi, FLOAT xj, FLOAT xk, FLOAT yi, FLOAT yj, FLOAT yk, FLOAT zi, FLOAT zj, FLOAT zk) override;

            __ &operator=(__ const &shape);
            __ &operator=(__ &&shape);

            bool operator==(__ const &shape);
            bool operator!=(__ const &shape);

            __
            (
                FLOAT ox, FLOAT oy, FLOAT oz,
                FLOAT xi, FLOAT xj, FLOAT xk,
                FLOAT yi, FLOAT yj, FLOAT yk,
                FLOAT zi, FLOAT zj, FLOAT zk,
                FLOAT a, FLOAT b, FLOAT c
            );
            __(FLOAT ox, FLOAT oy, FLOAT oz, FLOAT a, FLOAT b, FLOAT c);
            __(VEC const &o, Orientation const &orientation, VEC const &s);
            __(VEC const &o, VEC const &x, VEC const &y, VEC const &z, VEC const &s);
            __(VEC const &o, VEC const &s);
            __(__ const &shape);
            __(__ &&shape);
            __();
            virtual ~__();
        };
        #undef __
    }
}

#endif
#pragma endregion