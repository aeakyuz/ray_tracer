#include "camera.hpp"
#include <cstddef>

using std::size_t;

using namespace rtracer;

Camera::Camera()
    : position(Point()), gaze(Vector()), up(Vector()), imagePlane(ImagePlane()),
      distance(0) {}

Camera::Camera(const Point &pos, const Vector &gaze, const Vector &up,
               const ImagePlane &ip, const int dist)
    : position(pos), gaze(gaze), up(up), imagePlane(ip), distance(dist) {}

const Point Camera::getPosition(void) const { return position; }
const Vector Camera::getGaze(void) const { return gaze; }
const Vector Camera::getUp(void) const { return up; }
const ImagePlane Camera::getImagePlane(void) const { return imagePlane; }
const int Camera::getDistance(void) const { return distance; }

const Point Camera::setPosition(const Point &pos) {
  return this->position = pos;
}
const Vector Camera::setGaze(const Vector &gaze) { return this->gaze = gaze; }
const Vector Camera::setUp(const Vector &up) { return this->up = up; }
const ImagePlane Camera::setImagePlane(const ImagePlane &ip) {
  return imagePlane = ip;
}
const int Camera::setDistance(const int dist) { return distance = dist; }

const vector<Ray> Camera::calculateViewRays(void) const {
  vector<Ray> rays = vector<Ray>();
  const Point M = ((Vector)getPosition() + (getGaze() * getDistance())).getPoint();
  const Point Q = ((Vector)M + (Vector::UNIT_VEC_U * getImagePlane().getL()) +
    (Vector::UNIT_VEC_V * getImagePlane().getT())).getPoint();

  for (size_t i = 0; i < getImagePlane().getNx(); ++i) {
    for (size_t j = 0; j < getImagePlane().getNy(); ++j) {
      const double Su =
          (getImagePlane().getR() - getImagePlane().getL()) *
          (i + 0.5) / getImagePlane().getNx();
      const double Sv =
          ((getImagePlane().getT() - getImagePlane().getB()) *
           (j + 0.5) / getImagePlane().getNy());
      const Point S =
          ((Vector)Q + (Vector::UNIT_VEC_U * Su) + (-Vector::UNIT_VEC_V * Sv)).getPoint();

      // Create ray from camera position to S
      rays.push_back(Ray(position, S).normalizeRay());
    }
  }
  return rays;
}
