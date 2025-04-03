#ifndef MESH_HPP_
#define MESH_HPP_

#include "face.hpp"
#include "material.hpp"
#include <cstddef>
#include <vector>

namespace rtracer {

class Mesh : public std::vector<Face> {
private:
  size_t id;
  Material mat;


public:
  Mesh();
  Mesh(const size_t id, const Material &);
  const size_t getId(void) const;
  const size_t setId(const size_t);
  const Material getMaterial(void) const;
  const Material setMaterial(const Material &);

};

}

#endif  // MESH_HPP_
