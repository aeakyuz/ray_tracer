#ifndef RAY_HPP_
#define RAY_HPP_

#include <iostream>

using std::ostream;

namespace rtracer {

const double EPS = 0.00001;

class Point;

class Vector {
private:
  double u;
  double v;
  double w;

public:
  static const Vector UNIT_VEC_U;
  static const Vector UNIT_VEC_V;
  static const Vector UNIT_VEC_W;

  Vector();
  Vector(double u, double v, double w);

  const double getU(void) const;
  const double getV(void) const;
  const double getW(void) const;

  const double setU(double);
  const double setV(double);
  const double setW(double);

  const double getLength(void) const;

  const Point operator*(const double x) const;
  const Vector operator-() const;

  friend ostream & operator<<(ostream & os, const Vector & ray);
};

class Point : public Vector {
public:
  Point();
  Point(double u, double v, double w);

  const Point operator+(const Point &) const;
  const Point operator-(const Point &) const;

  friend ostream & operator<<(ostream & os, const Point & ray);
};

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

  friend ostream & operator<<(ostream & os, const Ray & ray);
};

} // namespace rtracer

#endif  // RAY_HPP_
