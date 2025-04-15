#include "matrix_3x3.hpp"
#include <cstddef>

using namespace rtracer;
using std::cout;
using std::endl;

Matrix_3x3::Matrix_3x3() : data{ Vector(),Vector(),Vector() } {}

Matrix_3x3::Matrix_3x3(const Vector &vec)
: data{ vec, vec, vec } {}

Matrix_3x3::Matrix_3x3(const array<Vector, 3> arr) : data(arr) {}

const double Matrix_3x3::determinant(void) const {
  double a = data[0].getU();
  double b = data[0].getV();
  double c = data[0].getW();

  double d = data[1].getU();
  double e = data[1].getV();
  double f = data[1].getW();

  double g = data[2].getU();
  double h = data[2].getV();
  double i = data[2].getW();

  return a * (e*i - h*f) + b * (g*f - d*i)
  + c * (d*h - e*g);
}

Vector Matrix_3x3::operator[](const size_t idx) const {
  return data.at(idx);
}

namespace rtracer {
ostream &operator<<(ostream &os, const Matrix_3x3 &matrix) {
  for (size_t i = 0; i < 3; ++i) {
    cout << "[";
    for (size_t j = 0; j < 2; ++j) {
      cout << matrix[j][i] << " ";
    }
    cout << matrix[2][i] << "]";
    if (i != 2) cout << endl;
  }
  return os;
}
}
