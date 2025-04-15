#ifndef POINTLIGHT_HPP_
#define POINTLIGHT_HPP_

#include "light.hpp"
#include "point.hpp"
#include "vector.hpp"

namespace rtracer {

class PointLight : public Light {
  
private:
  Point pos;
  Vector intensity;
  size_t id;

public:
  PointLight();
  PointLight(const Point &pos, const Vector &intensity, const size_t id);
  const Point getPos(void) const;
  const Vector getIntensity(void) const;
  const Point setPos(const Point &pos);
  const Vector setIntensity(const Vector &intensity);
  const size_t getId(void) const;
  const size_t setId(const size_t id);

};
}

#endif  // POINTLIGHT_HPP_
