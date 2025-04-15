#include "point.hpp"
#include <cmath>

using namespace rtracer;

Point::Point() : Vector() {}

Point::Point(double u, double v, double w) : Vector(u, v, w) {}

const double Point::calcDistance(const Point &other) const {
  Vector diff = *this - other;
  double sumSquares = (diff.getU() * diff.getU()) + (diff.getV() * diff.getV()) + (diff.getW() * diff.getW());
  return sqrt(sumSquares);
}
