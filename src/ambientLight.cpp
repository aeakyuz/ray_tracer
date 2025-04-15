#include "ambientLight.hpp"

using namespace rtracer;


AmbientLight::AmbientLight() : lum(Vector(0,0,0)) {}

AmbientLight::AmbientLight(const Vector &lum) : lum(lum) {}

const Vector AmbientLight::getLum(void) const {
  return lum;
}

const Vector AmbientLight::setLum(const Vector &lum) {
  return this->lum = lum;
}
