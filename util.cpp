#include "util.hpp"

const bool rtracer::isWithinEps(double x, double y) {
  if (x >= y - EPS && x <= y + EPS) {
    return true;
  } else {
    return false;
  }
}
