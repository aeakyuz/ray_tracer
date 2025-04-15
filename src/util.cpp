#include "util.hpp"

const bool rtracer::isWithinEps(double x, double y) {
  if (x >= y - EPS && x <= y + EPS) {
    return true;
  } else {
    return false;
  }
}

rtracer::Vector rtracer::barycentric(const Triangle &obj, const Point &p) {
  Vector v0 = obj.getVertices()[1] - obj.getVertices()[0];
  Vector v1 = obj.getVertices()[2] - obj.getVertices()[0];
  Vector v2 = p - obj.getVertices()[0];

  double d00 = v0.dotProduct(v0);
  double d01 = v0.dotProduct(v1);
  double d11 = v1.dotProduct(v1);
  double d20 = v2.dotProduct(v0);
  double d21 = v2.dotProduct(v1);
  double denom = d00 * d11 - d01 * d01;

  double beta = (d11 * d20 - d01 * d21) / denom;
  double gamma = (d00 * d21 - d01 * d20) / denom;
  double alpha = 1.0f - beta - gamma;

  return Vector(alpha, beta, gamma);
}
