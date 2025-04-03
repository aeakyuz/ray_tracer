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
  const Point M = position + (gaze * distance);
  const Point Q = M + (Vector::UNIT_VEC_U * imagePlane.getL()) +
                  (Vector::UNIT_VEC_V * imagePlane.getT());
  for (size_t i = 0; i < imagePlane.getNx(); ++i) {
    for (size_t j = 0; j < imagePlane.getNy(); ++j) {
      const double Su = (imagePlane.getR() - imagePlane.getL())
        * (i + 0.5) / imagePlane.getNx();
      const double Sv = ((imagePlane.getT() - imagePlane.getB())
        * (j + 0.5) / imagePlane.getNy());
      const Point S =
          Q + (Vector::UNIT_VEC_U * Su) + (-Vector::UNIT_VEC_V * Sv);

      // Create ray from camera position to S
      rays.push_back(Ray(position, S).normalizeRay());
    }
  }
  return rays;
}
