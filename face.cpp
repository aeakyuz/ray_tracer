#include "face.hpp"

using namespace rtracer;

Face::Face() : Triangle(), normal(Vector()),
textureData(Vector()) {}

Face::Face(const Point &v1, const Point &v2, const Point &v3,
           const Vector &normal, const Vector &texture) :
Triangle(v1, v2, v3), normal(normal), textureData(texture) {}

const Vector Face::getNormal(void) const {
  return normal;
}

const Vector Face::getTexture(void) const {
  return textureData;
}

const Vector Face::setNormal(const Vector &normal) {
  return this->normal = normal;
}

const Vector Face::setTexture(const Vector &texture) {
  return this->textureData = texture;
}
