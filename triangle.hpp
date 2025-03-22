#ifndef TRIANGLE_HPP_
#define TRIANGLE_HPP_

#include "util.hpp"
#include <array>
#include <cstddef>

using std::array;
using std::size_t;

namespace rtracer {

class Triangle {
private:
  std::array<Point, 3> vertices;

public:
  Triangle();
  Triangle(const Point &v1, const Point &v2, const Point &v3);

  const std::array<Point, 3> &getVertices(void) const;
  const std::array<Point, 3> &
  setVertices(const std::array<Point, 3> &newVertices);
  const Point &getVertex(const size_t index) const;
  const Point &setVertex(const size_t index, const Point &vertex);

  const Point &operator[](const size_t index) const;
};

} // namespace rtracer

#endif // TRIANGLE_HPP_
