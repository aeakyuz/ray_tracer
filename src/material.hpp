#ifndef MATERIAL_HPP_
#define MATERIAL_HPP_

#include "vector.hpp"

namespace rtracer {

class Material {

private:
  Vector ambient;
  Vector diffuse;
  Vector specular;
  Vector mirror;
  double phonge;
  double textureFactor;
  size_t id;

public:
  Material();
  Material(const Vector &, const Vector &, const Vector &, const Vector &,
           const double, const double, const size_t);

  const Vector getAmbient(void) const;
  const Vector getDiffuse(void) const;
  const Vector getSpecular(void) const;
  const Vector getMirror(void) const;
  const double getPhong(void) const;
  const double getTextureFactor(void) const;
  const size_t getId(void) const;

  const Vector setAmbient(const Vector &);
  const Vector setDiffuse(const Vector &);
  const Vector setSpecular(const Vector &);
  const Vector setMirror(const Vector &);
  const double setPhonge(const double);
  const double setTextureFactor(const double);
  const size_t setId(const size_t);

};

}

#endif  // MATERIAL_HPP_
