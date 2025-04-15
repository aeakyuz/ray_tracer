#include "material.hpp"

using namespace rtracer;

Material::Material() : ambient(Vector()), diffuse(Vector()),
specular(Vector()), mirror(Vector()), phonge(0.0), textureFactor(0.0), id(0) {}

Material::Material(const Vector &ambient, const Vector &diffuse,
                   const Vector &specular, const Vector &mirror,
                   const double phonge, const double textureFactor,
                   const size_t id):
ambient(ambient), diffuse(diffuse), specular(specular), mirror(mirror),
phonge(phonge), textureFactor(textureFactor), id(id) {}

const Vector Material::getAmbient(void) const {
  return ambient;
}

const Vector Material::getDiffuse(void) const {
  return diffuse;
}

const Vector Material::getSpecular(void) const {
  return specular;
}

const Vector Material::getMirror(void) const {
  return mirror;
}

const double Material::getPhong(void) const {
  return phonge;
}

const double Material::getTextureFactor(void) const {
  return textureFactor;
}

const size_t Material::getId(void) const {
  return id;
}

const Vector Material::setAmbient(const Vector &ambient) {
  return this->ambient = ambient;
}

const Vector Material::setDiffuse(const Vector &diffuse) {
  return this->diffuse = diffuse;
}

const Vector Material::setSpecular(const Vector &specular) {
  return this->specular = specular;
}

const Vector Material::setMirror(const Vector &mirror) {
  return this->mirror = mirror;
}

const double Material::setPhonge(const double phonge) {
  return this->phonge = phonge;
}

const double Material::setTextureFactor(const double tf) {
  return this->textureFactor = tf;
}

const size_t Material::setId(const size_t id) {
  return this->id = id;
}
