#ifndef MATRIX_3x3_HPP_
#define MATRIX_3x3_HPP_

#include "vector.hpp"
#include <array>
#include <iostream>

using std::array;
using std::ostream;

namespace rtracer {


class Matrix_3x3 {

private:
  // Each Vector is a column of the matrix
  array<Vector, 3> data;

public:
  Matrix_3x3();
  Matrix_3x3(const Vector &vec);
  Matrix_3x3(const array<Vector, 3> arr);

  const double determinant(void) const;

  Vector operator[](const size_t idx) const;

  friend ostream &operator<<(ostream &os, const Matrix_3x3 &);

};

}

#endif  // MATRIX_3x3_HPP_
