#include "vector.hpp"
#include "point.hpp"
#include <cmath>
#include <stdexcept>

using namespace rtracer;

// Initialize unit vectors
const Vector Vector::UNIT_VEC_U = Vector(1, 0, 0);
const Vector Vector::UNIT_VEC_V = Vector(0, 1, 0);
const Vector Vector::UNIT_VEC_W = Vector(0, 0, 1);
const Vector Vector::ZERO_VECTOR = Vector(0, 0, 0);

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

const Vector Vector::operator+(const Vector &other) const {
  return Vector(u+other.u, v+other.v, w+other.w);
}

const Point Vector::operator*(const double x) const {
  return Point(u * x, v * x, w * x);
}

const Vector Vector::operator-() const { return Vector(-u, -v, -w); }

const Vector Vector::operator-(const Vector &other) const {
  return *this + (-other);
}

double &Vector::operator[](const size_t idx) {
  if (idx == 0) {
    return u;
  } else if (idx == 1) {
    return v;
  } else if (idx == 2) {
    return w;
  } else {
    throw std::out_of_range("Index out of range");
  }
}

const bool Vector::operator==(const Vector &other) const {
  return u == other.getU() && v == other.getV() && w == other.getW();
}

const bool Vector::operator!=(const Vector &other) const {
  return !(*this == other);
}

const double Vector::dotProduct(const Vector &other) const {
  return u * other.u + v * other.v + w * other.w;
}

const Vector Vector::crossProduct(const Vector &other) const {
  const double c_u = v * other.w - w * other.v;
  const double c_v = w * other.u - u * other.w;
  const double c_w = u * other.v - v * other.u;
  return Vector(c_u, c_v, c_w);
}

namespace rtracer {
ostream &operator<<(ostream &os, const Vector &vec) {
  os << "[" << vec.u << "-> " << vec.v << "-> " << vec.w << "->]";
  return os;
}
} // namespace rtracer
