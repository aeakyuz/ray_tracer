#include "ray.hpp"
#include "matrix_3x3.hpp"
#include "util.hpp"
#include "triangle.hpp"

#include <cstdio>
#include <iostream>

using namespace rtracer;
using std::cout;
using std::endl;

Ray::Ray() : origin(Point(0, 0, 0)), direction(Vector(0, 0, 0)) {}

Ray::Ray(const Point &origin, const Vector &direction)
: origin(origin), direction(direction) {}

Ray::Ray(const Point &from, const Point &to) {
  Vector direction = Vector(to.getU() - from.getU(), to.getV() - from.getV(),
                            to.getW() - from.getW());
  Point origin = from;

  this->origin = origin;
  this->direction = direction;
}

const Point Ray::getOrigin(void) const { return origin; }

const Vector Ray::getDirection(void) const { return direction; }

const Ray Ray::normalizeRay(void) {
  double length = direction.getLength();
  if (length <= 0.0 + EPS && length >= 0.0 - EPS) {
    return *this;
  } else if (length <= 1.0 + EPS && length >= 1.0 - EPS) {
    return *this;
  }
  // if (length == 0.0) {
  //   return *this;
  // } else if (length <= 1.0 + EPS && length >= 1.0 - EPS) {
  //   return *this;
  // }

  direction.setU(direction.getU() / length);
  direction.setV(direction.getV() / length);
  direction.setW(direction.getW() / length);
  return *this;
}

const double Ray::isIntersecting(const Triangle &tri) const {
  // NOTE: Implement using Cramer's rule.

  double a_x = tri.getVertices()[0].getU();
  double a_y = tri.getVertices()[0].getV();
  double a_z = tri.getVertices()[0].getW();

  double b_x = tri.getVertices()[1].getU();
  double b_y = tri.getVertices()[1].getV();
  double b_z = tri.getVertices()[1].getW();

  double c_x = tri.getVertices()[2].getU();
  double c_y = tri.getVertices()[2].getV();
  double c_z = tri.getVertices()[2].getW();

  double o_x = origin.getU();
  double o_y = origin.getV();
  double o_z = origin.getW();

  double d_x = direction.getU();
  double d_y = direction.getV();
  double d_z = direction.getW();

  Matrix_3x3 A = Matrix_3x3({
    Vector(a_x - b_x, a_y - b_y, a_z - b_z),
    Vector(a_x - c_x, a_y - c_y, a_z - c_z),
    Vector(d_x, d_y, d_z),
  });

  double det_A = A.determinant();

  // cout << "Matrix A:\n";
  // cout << A << endl;
  // cout << "|A|:\n";
  // cout << det_A << endl;
  
  if (isWithinEps(det_A, 0.0)) {
    return -1.0;
  }

  Matrix_3x3 matrix_beta = Matrix_3x3({
    Vector(a_x - o_x, a_y - o_y, a_z - o_z),
    Vector(a_x - c_x, a_y - c_y, a_z - c_z),
    Vector(d_x, d_y, d_z),
  });

  double beta = matrix_beta.determinant() / det_A;

  // cout << "Matrix Beta:\n";
  // cout << matrix_beta << endl;
  // cout << "|B|:\n";
  // cout << beta << endl;
  
  Matrix_3x3 matrix_gamma = Matrix_3x3({
    Vector(a_x - b_x, a_y - b_y, a_z - b_z),
    Vector(a_x - o_x, a_y - o_y, a_z - o_z),
    Vector(d_x, d_y, d_z),
  });

  double gamma = matrix_gamma.determinant() / det_A;

  // cout << "Matrix Gamma:\n";
  // cout << matrix_gamma << endl;
  // cout << "|G|:\n";
  // cout << gamma << endl;
  
  Matrix_3x3 matrix_t = Matrix_3x3({
    Vector(a_x - b_x, a_y - b_y, a_z - b_z),
    Vector(a_x - c_x, a_y - c_y, a_z - c_z),
    Vector(a_x - o_x, a_y - o_y, a_z - o_z),
  });

  double t = matrix_t.determinant() / det_A;

  // cout << "Matrix T:\n";
  // cout << matrix_t << endl;
  // cout << "t:\n";
  // cout << t << endl;
  
  // printf("detA=%lf B=%lf G=%lf B+G=%lf detT=%lf t=%lf\n", det_A,
  //        beta, gamma, beta+gamma, matrix_t.determinant(), t);

  if (t >= 0.0 && beta + gamma < 1.0
  && (beta > 0.0 || isWithinEps(beta, 0.0)) && (gamma > 0.0 || isWithinEps(gamma, 0.0))) {
    return t;
  }
  else {
    // Since we are not interested in negative values,
    // we can use a negative value to indicate that
    // they don't intersect
    return -1.0;
  }
}

namespace rtracer {
ostream &operator<<(ostream &os, const Ray &ray) {
  os << "Position: " << ray.origin << " Direction: " << ray.direction;
  return os;
}
} // namespace rtracer

const Point Point::operator+(const Point &other) const {
  return Point(getU() + other.getU(), getV() + other.getV(),
               getW() + other.getW());
}

const Point Point::operator-(const Point &other) const {
  return Point(getU() - other.getU(), getV() - other.getV(),
               getW() - other.getW());
}

namespace rtracer {
ostream &operator<<(ostream &os, const Point &point) {
  os << "[" << point.getU() << " " << point.getV() << " " << point.getW()
     << "]";
  return os;
}
} // namespace rtracer

const Point Ray::calculatePoint(const double x) const {
  return origin + (direction * x);
}
