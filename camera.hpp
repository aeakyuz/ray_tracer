#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include "imagePlane.hpp"
#include "util.hpp"
#include <vector>

using std::vector;

namespace rtracer {

class Camera {
private:
  Point position;
  Vector gaze;
  Vector up;
  ImagePlane imagePlane;
  int distance;

public:
  Camera();
  Camera(const Point &, const Vector &, const Vector &, const ImagePlane &, const int);

  const Point getPosition(void) const;
  const Vector getGaze(void) const;
  const Vector getUp(void) const;
  const ImagePlane getImagePlane(void) const;
  const int getDistance(void) const;

  const Point setPosition(const Point & pos);
  const Vector setGaze(const Vector & gaze);
  const Vector setUp(const Vector & up);
  const ImagePlane setImagePlane(const ImagePlane & ip);
  const int setDistance(const int dist);

  const vector<Ray> calculateViewRays(void) const;
};

} // namespace rtracer

#endif // CAMERA_HPP_
