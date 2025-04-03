#ifndef RAY_HPP_
#define RAY_HPP_

#include "point.hpp"
#include "vector.hpp"

namespace rtracer {

class Ray {
private:
  Point origin;
  Vector direction;

public:
  Ray();
  Ray(const Point &origin, const Vector &direction);
  Ray(const Point &from, const Point &to);

  const Ray calculateRay(const Point &from, const Point &to) const;
  const Point calculatePoint(const double x) const;

  const Point getOrigin(void) const;
  const Vector getDirection(void) const;

  const Ray normalizeRay(void);
  const double isIntersecting(const Triangle &tri) const;

  friend ostream &operator<<(ostream &os, const Ray &ray);
};

}

#endif  // RAY_HPP_
