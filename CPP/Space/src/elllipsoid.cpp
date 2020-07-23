#include <cmath>
#include "vector3D.hpp"
#include "ellipsoid.hpp"

using namespace Space;
using namespace Space::Shape;

#define PI                  3.141592653589793238463
#define THOMSEN_EXPONENT    1.6075

const FLOAT COEFFICIENT_ELLIPSOID_VOLUME           = (4./3.)*PI;
const FLOAT COEFFICIENT_ELLIPSOID_SURFACE_AREA     = (4.*PI)/pow(3., 1./THOMSEN_EXPONENT);

/*
*   Ellipsoid class definition
*/
#define __ Ellipsoid

inline bool __::is_point_inside(FLOAT dx, FLOAT dy, FLOAT dz, VEC *s) const
{
    return (dx*dx)/(s->x*s->x) + (dy*dy)/(s->y*s->y) + (dz*dz)/(s->z*s->z) <= 1.;
}

inline FLOAT __::volume() const
{
    return COEFFICIENT_ELLIPSOID_VOLUME*(this->__s->x*this->__s->y*this->__s->z);
}
FLOAT __::surface_area() const
{
    FLOAT
        ap = pow(this->__s->x, THOMSEN_EXPONENT),
        bp = pow(this->__s->y, THOMSEN_EXPONENT),
        cp = pow(this->__s->z, THOMSEN_EXPONENT);

    return COEFFICIENT_ELLIPSOID_SURFACE_AREA*pow(ap*bp + ap*cp + bp*cp, 1./THOMSEN_EXPONENT);
}

__::__(__ const &ellipsoid)
    : Shape3D(ellipsoid)
{
}
__::__(__ &&ellipsoid)
    : Shape3D(ellipsoid)
{
}
__::~__()
{
}