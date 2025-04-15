#ifndef FACE_HPP_
#define FACE_HPP_

#include "triangle.hpp"

namespace rtracer {

class Face : public Triangle {
private:
  std::array<Vector, 3> normalData;
  std::array<Vector, 3> textureData;

public:
  Face();
  Face(const Point &, const Point &, const Point &,
       std::array<Vector, 3>, std::array<Vector, 3>);
  const Vector getNormal(const Point &) const;
  const Vector getTexture(const Point &) const;
  const Vector getNormal(size_t) const;
  const Vector getTexture(size_t) const;
  const Vector setNormal(size_t, const Vector &);
  const Vector setTexture(size_t, const Vector &);

};

}

#endif  // FACE_HPP_
