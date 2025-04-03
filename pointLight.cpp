#include "pointLight.hpp"

using namespace rtracer;

PointLight::PointLight() : pos(Point()), intensity(Vector()), id(0) {}

PointLight::PointLight(const Point &pos, const Vector &intensity, const size_t id)
: pos(pos), intensity(intensity), id(id) {}

const Point PointLight::getPos(void) const {
  return pos;
}

const Vector PointLight::getIntensity(void) const {
  return intensity;
}

const Point PointLight::setPos(const Point &pos) {
  return this->pos = pos;
}

const Vector PointLight::setIntensity(const Vector &intensity) {
  return this->intensity = intensity;
}

const size_t PointLight::getId(void) const {
  return id;
}

const size_t PointLight::setId(const size_t id) {
  return this->id = id;
}
