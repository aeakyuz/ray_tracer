#ifndef IMAGEPLANE_HPP_
#define IMAGEPLANE_HPP_

namespace rtracer {

class ImagePlane {
private:
  int l, r, b, t;
  int nx, ny;

public:
  ImagePlane();
  ImagePlane(int l, int r, int b, int t, int nx, int ny);
  const int getL(void) const;
  const int getR(void) const;
  const int getT(void) const;
  const int getB(void) const;
  const int getNx(void) const;
  const int getNy(void) const;

  const int setL(int);
  const int setR(int);
  const int setT(int);
  const int setB(int);
  const int setNx(int);
  const int setNy(int);
};
} // namespace rtracer

#endif // IMAGEPLANE_HPP_
