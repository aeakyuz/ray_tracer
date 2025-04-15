#ifndef POINT_HPP_
#define POINT_HPP_

#include "vector.hpp"

namespace rtracer {

class Point : public Vector {
public:
  Point();
  Point(double u, double v, double w);

  const double calcDistance(const Point &) const;

  const Point operator+(const Point &) const;
  const Point operator-(const Point &) const;

  friend ostream &operator<<(ostream &os, const Point &ray);
};

}

#endif  // POINT_HPP_

