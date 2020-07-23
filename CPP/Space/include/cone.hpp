#pragma once
#ifndef __AGD__SPACE__SHAPE__CONE_HPP__
#define __AGD__SPACE__SHAPE__CONE_HPP__

#include "shape3D.hpp"
#include "boundary3D.hpp"

namespace Space
{
    namespace Shape
    {
        #define __ Cone
        class __ : public virtual Boundary3D, public virtual Shape3D
        {
            virtual bool is_point_inside(FLOAT dx, FLOAT dy, FLOAT dz, VEC *s) const override;

        public:

            virtual FLOAT volume() const override;
            virtual FLOAT surface_area() const override;

            using Shape3D::Shape3D;

            __(__ const &cone);
            __(__ &&cone);
            ~__();
        };
        #undef __
    }
}

#endif
#pragma endregion