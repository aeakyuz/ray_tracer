#include "triangle.hpp"

using namespace rtracer;
using std::array;
using std::size_t;

Triangle::Triangle() : vertices{Point(), Point(), Point()} {}

Triangle::Triangle(const Point &v1, const Point &v2, const Point &v3)
    : vertices{v1, v2, v3} {}

const array<Point, 3> &Triangle::getVertices(void) const { return vertices; }

const array<Point, 3> &
Triangle::setVertices(const array<Point, 3> &newVertices) {
  return vertices = newVertices;
}

const Point &Triangle::getVertex(const size_t index) const {
  return vertices.at(index);
}

const Point &Triangle::setVertex(const size_t index, const Point &vertex) {
  return vertices.at(index) = vertex;
}

Point & Triangle::operator[](const size_t index) {
  return vertices.at(index);
}

namespace rtracer {
ostream &operator<<(ostream &os, const Triangle &tri) {
  os << tri.getVertex(0) << ", "
    << tri.getVertex(1) << ", "
    << tri.getVertex(2) << std::endl;
  return os;
}
}
