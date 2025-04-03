#ifndef FACE_HPP_
#define FACE_HPP_

#include "triangle.hpp"

namespace rtracer {

class Face : public Triangle {
private:
  Vector normal;
  Vector textureData;

public:
  Face();
  Face(const Point &, const Point &, const Point &,
       const Vector &, const Vector &);
  const Vector getNormal(void) const;
  const Vector getTexture(void) const;
  const Vector setNormal(const Vector &);
  const Vector setTexture(const Vector &);

};

}

#endif  // FACE_HPP_
