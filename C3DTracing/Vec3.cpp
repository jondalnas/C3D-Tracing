#include "pch.h"
#include "Vec3.h"

std::ostream& operator<<(std::ostream &strm, const Vec3 &v) {
    return strm << "{" << v.x << ", " << v.y << ", " << v.z << "}";
}