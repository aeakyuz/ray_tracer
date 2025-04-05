#ifndef INCLUDE_HW1_TRIANGULARLIGHT_HPP_
#define INCLUDE_HW1_TRIANGULARLIGHT_HPP_

#include "light.hpp"
#include "triangle.hpp"

using std::array;

namespace rtracer {

class TriangularLight : public Light {

private:
  Triangle vertices;
  Vector intensity;
  Vector normal;
  size_t id;
  const Vector setNormal();

public:
  TriangularLight();
  TriangularLight(const Triangle &vertices, const Vector &intensity, const size_t id);
  const Triangle getVertices(void) const;
  const Vector getIntensity(void) const;
  const Triangle setVertices(const Triangle &vertices);
  const Vector setIntensity(const Vector &intensity);
  const size_t getId(void) const;
  const size_t setId(const size_t id);
  const Vector getNormal(void) const;
};

}

#endif  // INCLUDE_HW1_TRIANGULARLIGHT_HPP_
