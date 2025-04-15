#define STB_IMAGE_IMPLEMENTATION
#include "texture.hpp"
#include "stb_image.h"
#include <algorithm>
#include <cmath>

using namespace rtracer;

Texture::Texture() : data(vector<vector<Vector>>()), width(0), height(0) {}

Texture::Texture(const vector<vector<Vector>> &data)
    : data(data), width(data.empty() ? 0 : data[0].size()),
      height(data.size()) {}

Texture::Texture(const string &filename) {
  // Load image with STB
  int channels;
  unsigned char *imgData =
      stbi_load(filename.c_str(), &width, &height, &channels, 3);

  if (!imgData) {
    throw std::runtime_error("Failed to load texture: " + filename);
  }

  // Reserve space for texture data
  data.resize(height, vector<Vector>(width));

  // Convert to Vector format
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      unsigned char *pixel = imgData + (y * width + x) * 3;
      data[y][x] = Vector((double)pixel[0] / 255.0, // R
                          (double)pixel[1] / 255.0, // G
                          (double)pixel[2] / 255.0  // B
      );
    }
  }

  stbi_image_free(imgData);
}

Vector Texture::getColor(double u, double v) const {
  // Clamp texture coordinates to [0, 1] range
  u = std::clamp(u, 0.0, 1.0);
  v = std::clamp(v, 0.0, 1.0);

  // Convert to texture space coordinates
  double x = u * (width - 1);
  double y = v * (height - 1);

  // Get the four surrounding texels
  int x0 = static_cast<int>(std::floor(x));
  int y0 = static_cast<int>(std::floor(y));
  int x1 = std::min(x0 + 1, width - 1);
  int y1 = std::min(y0 + 1, height - 1);

  // Calculate fractional parts
  double dx = x - x0;
  double dy = y - y0;

  // Get the four texel colors
  const Vector &c00 = data[y0][x0];
  const Vector &c01 = data[y0][x1];
  const Vector &c10 = data[y1][x0];
  const Vector &c11 = data[y1][x1];

  // Bilinear interpolation
  Vector color = c00 * (1 - dx) * (1 - dy) + c01 * dx * (1 - dy) +
                 c10 * (1 - dx) * dy + c11 * dx * dy;

  return color;
}
