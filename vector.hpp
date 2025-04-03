#ifndef VECTOR_HPP_
#define VECTOR_HPP_

#include <iostream>

using std::ostream;

namespace rtracer {

class Point;

class Triangle;

class Vector {
private:
  double u;
  double v;
  double w;

public:
  static const Vector UNIT_VEC_U;
  static const Vector UNIT_VEC_V;
  static const Vector UNIT_VEC_W;
  static const Vector ZERO_VECTOR;

  Vector();
  Vector(double u, double v, double w);

  const double getU(void) const;
  const double getV(void) const;
  const double getW(void) const;

  const double setU(double);
  const double setV(double);
  const double setW(double);

  const double getLength(void) const;

  const Vector operator+(const Vector &) const;
  const Point operator*(const double x) const;
  const Vector operator-() const;
  const Vector operator-(const Vector &) const;
  double &operator[](const size_t idx);
  const bool operator==(const Vector &) const;
  const bool operator!=(const Vector &) const;

  const double dotProduct(const Vector &other) const;
  const Vector crossProduct(const Vector &other) const;

  friend ostream &operator<<(ostream &os, const Vector &ray);
};

}

#endif  // VECTOR_HPP_
