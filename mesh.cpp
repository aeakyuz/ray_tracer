#include "mesh.hpp"

using namespace rtracer;

Mesh::Mesh() : std::vector<Face>(), id(0), mat(Material()) {}

Mesh::Mesh(const size_t id, const Material &mat)
: std::vector<Face>(), id(id), mat(mat) {}

const size_t Mesh::getId(void) const {
  return id;
}

const size_t Mesh::setId(const size_t id) {
  return this->id = id;
}

const Material Mesh::getMaterial(void) const {
  return mat;
}

const Material Mesh::setMaterial(const Material & mat) {
  return this->mat = mat;
}
