#include "triangularLight.hpp"

using namespace rtracer;

TriangularLight::TriangularLight() : vertices(Triangle()), intensity(Vector()), id(0), normal(Vector()) {}

TriangularLight::TriangularLight(const Triangle &vertices, const Vector &intensity, const size_t id)
: vertices(vertices), intensity(intensity), id(id) {
  setNormal();
}

const Triangle TriangularLight::getVertices(void) const {
  return vertices;
}

const Vector TriangularLight::getIntensity(void) const {
  return intensity;
}

const Triangle TriangularLight::setVertices(const Triangle &vertices) {
  this->vertices = vertices;
  setNormal();
  return vertices;
}

const Vector TriangularLight::setIntensity(const Vector &intensity) {
  return this->intensity = intensity;
}

const size_t TriangularLight::getId(void) const {
  return id;
}

const size_t TriangularLight::setId(const size_t id) {
  return this->id = id;
}

const Vector TriangularLight::getNormal(void) const {
  return normal;
}

const Vector TriangularLight::setNormal() {
  Vector x = vertices.getVertex(0) - vertices.getVertex(1);
  Vector y = vertices.getVertex(0) - vertices.getVertex(2);
  normal = x.crossProduct(y);
  normal.normalize();
  return normal;
}
