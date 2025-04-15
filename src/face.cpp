#include "face.hpp"
#include "util.hpp"

using namespace rtracer;

Face::Face() : Triangle(), normalData{Vector(), Vector(), Vector()},
textureData{Vector(), Vector(), Vector()} {}

Face::Face(const Point &v1, const Point &v2, const Point &v3,
           std::array<Vector, 3> normal, std::array<Vector, 3> texture) :
Triangle(v1, v2, v3), normalData(normal), textureData(texture) {}

const Vector Face::getNormal(const Point &p) const {
  Vector bc = barycentric(*this, p);
  double alpha = bc.getU(), beta = bc.getV(), gamma = bc.getW();

  double u = normalData.at(0).getU() * alpha + normalData.at(1).getU() * beta
    + normalData.at(2).getU() * gamma;
  double v = normalData.at(0).getV() * alpha + normalData.at(1).getV() * beta
    + normalData.at(2).getV() * gamma;
  double w = normalData.at(0).getW() * alpha + normalData.at(1).getW() * beta
    + normalData.at(2).getW() * gamma;
  return Vector(u, v, w).normalize();
}

const Vector Face::getTexture(const Point &p) const {
  Vector bc = barycentric(*this, p);
  double alpha = bc.getU(), beta = bc.getV(), gamma = bc.getW();

  double u = textureData.at(0).getU() * alpha + textureData.at(1).getU() * beta
    + textureData.at(2).getU() * gamma;
  double v = textureData.at(0).getV() * alpha + textureData.at(1).getV() * beta
    + textureData.at(2).getV() * gamma;
  return Vector(u, v, 0);
}

const Vector Face::setNormal(size_t i, const Vector &normal) {
  return normalData.at(i) = normal;
}

const Vector Face::setTexture(size_t i, const Vector &texture) {
  return textureData.at(i) = texture;
}
