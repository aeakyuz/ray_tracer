#include "util.hpp"
#include <cmath>

using namespace rtracer;

// Initialize unit vectors
const Vector Vector::UNIT_VEC_U = Vector(1, 0, 0);
const Vector Vector::UNIT_VEC_V = Vector(0, 1, 0);
const Vector Vector::UNIT_VEC_W = Vector(0, 0, 1);

Vector::Vector() : u(0), v(0), w(0) {}

Vector::Vector(double u, double v, double w) : u(u), v(v), w(w) {}

double const Vector::getU(void) const { return u; }

double const Vector::getV(void) const { return v; }

double const Vector::getW(void) const { return w; }

const double Vector::setU(double u) { return this->u = u; }

const double Vector::setV(double v) { return this->v = v; }

const double Vector::setW(double w) { return this->w = w; }

const double Vector::getLength(void) const {
  double sumSquares = (u * u) + (v * v) + (w * w);
  return sqrt(sumSquares);
}

const Point Vector::operator*(const double x) const {
  return Point(u * x, v * x, w * x);
}

const Vector Vector::operator-() const { return Vector(-u, -v, -w); }

namespace rtracer {
ostream &operator<<(ostream &os, const Vector &vec) {
  os << "[" << vec.u << "-> " << vec.v << "-> " << vec.w << "->]";
  return os;
}
} // namespace rtracer

Point::Point() : Vector() {}

Point::Point(double u, double v, double w) : Vector(u, v, w) {}

Ray::Ray() : origin(Point(0, 0, 0)), direction(Vector(0, 0, 0)) {}

Ray::Ray(const Point &from, const Point &to) {
  Vector direction = Vector(to.getU() - from.getU(), to.getV() - from.getV(),
                            to.getW() - from.getW());
  Point origin = from;

  this->origin = origin;
  this->direction = direction;
  normalizeRay();
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

namespace rtracer {
    ostream &operator<<(ostream &os, const Ray &ray) {
        os << "Position: " << ray.origin << " Direction: " << ray.direction;
        return os;
    }
}

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
        os << "[" << point.getU() << " " << point.getV() << " " << point.getW() << "]";
        return os;
    }
}
const Point Ray::calculatePoint(const double x) const {
  return origin + (direction * x);
}
