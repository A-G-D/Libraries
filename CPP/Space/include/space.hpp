#pragma once
#ifndef __AGD__SPACE_HPP__
#define __AGD__SPACE_HPP__

namespace Space
{
    class Vector3D;
    class Orientation;

    namespace Shape
    {
        class Shape3D;
        class Boundary3D;
        class Ellipsoid;
        class Prism;
        class Cone;
        class Cylinder;
    }

    typedef double FLOAT;
    typedef Vector3D VEC;
}

#endif
#pragma endregion