#ifndef AMBIENTLIGHT_HPP_
#define AMBIENTLIGHT_HPP_

#include "light.hpp"
#include "vector.hpp"

namespace rtracer {

class AmbientLight : public Light {

private:
  Vector lum;

public:
  AmbientLight();
  AmbientLight(const Vector &lum);
  const Vector getLum(void) const;
  const Vector setLum(const Vector &lum);

};

} // namespace rtracer

#endif // AMBIENTLIGHT_HPP_
