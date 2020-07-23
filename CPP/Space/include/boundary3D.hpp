#pragma once
#ifndef __AGD__SPACE__SHAPE__BOUNDARY3D_HPP__
#define __AGD__SPACE__SHAPE__BOUNDARY3D_HPP__

#include "vector3D.hpp"
#include "shape3D.hpp"

namespace Space
{
    namespace Shape
    {
        #define __ Boundary3D
        class __ : public virtual Shape3D
        {
            virtual bool is_point_inside(FLOAT dx, FLOAT dy, FLOAT dz, VEC *s) const = 0;

        public:

            virtual FLOAT volume() const = 0;
            virtual FLOAT surface_area() const = 0;

            virtual bool contains(FLOAT x, FLOAT y, FLOAT z) const;
            bool contains(VEC const &v) const;

            virtual ~__();
        };
        #undef __
    }
}

#endif
#pragma endregion