#ifndef UTIL_HPP_
#define UTIL_HPP_

#include "triangle.hpp"
#include "vector.hpp"

#define MAX_COLOR_VAL 255

namespace rtracer {

const double EPS = 0.00001;

const bool isWithinEps(double x, double y);

Vector barycentric(const Triangle &, const Point &);

} // namespace rtracer

#endif // UTIL_HPP_
