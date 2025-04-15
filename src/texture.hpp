#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_

#include "vector.hpp"
#include "face.hpp"
#include <vector>
#include <string>

using std::vector;
using std::string;

namespace rtracer {

class Texture {
private:
  vector<vector<Vector>> data;
  int width;
  int height;

public:
  Texture();
  Texture(const vector<vector<Vector>> &data);
  Texture(const string &filename);

  Vector getColor(double u, double v) const;
};

}

#endif  // TEXTURE_HPP_
