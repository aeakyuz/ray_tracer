#include "imagePlane.hpp"

using namespace rtracer;

ImagePlane::ImagePlane() : l(0), r(0), b(0), t(0), nx(0), ny(0) {}

ImagePlane::ImagePlane(int l, int r, int b, int t, int nx, int ny) : l(l), r(r), b(b), t(t), nx(nx), ny(ny) {}

const int ImagePlane::getL(void) const {
    return l;
}

const int ImagePlane::getR(void) const {
    return r;
}

const int ImagePlane::getT(void) const {
    return t;
}

const int ImagePlane::getB(void) const {
    return b;
}

const int ImagePlane::getNx(void) const {
    return nx;
}

const int ImagePlane::getNy(void) const {
    return ny;
}

const int ImagePlane::setL(int l) {
    return this->l = l;
}

const int ImagePlane::setR(int r) {
    return this->r = r;
}

const int ImagePlane::setT(int t) {
    return this->t = t;
}

const int ImagePlane::setB(int b) {
    return this->b = b;
}

const int ImagePlane::setNx(int nx) {
    return this->nx = nx;
}

const int ImagePlane::setNy(int ny) {
    return this->ny = ny;
}
