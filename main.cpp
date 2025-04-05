// TODO: try to write into result into ppm file
#include "camera.hpp"
#include "matrix_3x3.hpp"
#include "point.hpp"
#include "ray.hpp"
#include "scene.hpp"
#include "triangle.hpp"
#include "triangularLight.hpp"
#include "util.hpp"
#include "vector.hpp"

#include <iostream>

using namespace rtracer;
using std::cerr;
using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <scene.xml>" << endl;
    return 1;
  }

  Scene scene = Scene(argv[1]);

  // Print some information to verify parsing
  cout << "Scene loaded successfully:" << endl;
  cout << "  Max ray trace depth: " << scene.maxRTDepth << endl;
  cout << "  Background color: (" << scene.bgColor[0] << ", "
       << scene.bgColor[1] << ", " << scene.bgColor[2] << ")" << endl;
  cout << "  Camera position: (" << scene.cam.getPosition().getU() << ", "
       << scene.cam.getPosition().getV() << ", "
       << scene.cam.getPosition().getW() << ")" << endl;
  cout << "  Image resolution: " << scene.cam.getImagePlane().getNx() << "x"
       << scene.cam.getImagePlane().getNy() << endl;
  cout << "  Number of vertices: " << scene.vertices.size() << endl;
  cout << "  Number of meshes: " << scene.meshes.size() << endl;

  // // Print face information for the first mesh
  // if (!scene.meshes.empty()) {
  //   cout << "Number of faces parsed: " << scene.meshes[0].size() << endl;
  //   // Modified debug output to show original indices (1-based)
  //   for (size_t i = 0; i < scene.meshes.front().size(); i++) {
  //     const Face &f = scene.meshes.front()[i];
  //     cout << "Face " << i + 1 << ":\n";
  //     cout << "  Vertices: " << f;
  //     cout << "  Texture indices: " << f.getTexture().getU() << ", "
  //          << f.getTexture().getV() << ", " << f.getTexture().getW()
  //          << "\n";
  //     cout << "  Normal indices: " << f.getNormal().getU() << ", "
  //          << f.getNormal().getV() << ", " << f.getNormal().getW() << "\n";
  //   }
  // }

  // Triangle tri = Triangle(
  //   Point(0.9, 0.0, -2.0),
  //   Point(-0.9, 0.0, -2.0),
  //   Point(0.0, 0.9, -2.0)
  // );
  // Matrix_3x3 m = Matrix_3x3({
  //     Vector(0.13877,0.01138,0.81727),
  //     Vector(0.96600,0.65327,0.26777),
  //     Vector(0.63917,0.55456,0.16161)
  //   });
  // cout << "Det: " << m.determinant() << endl;
  // vector<Ray> rays = scene.cam.calculateViewRays();
  // for (auto ray : rays) {
  //   cout << "Ray: " << ray << endl;
  //   double res = ray.isIntersecting(tri);
  //   // cout << "res is " << res << endl;
  //   if (res > 0) {
  //     cout << "Intersection\n";
  //   }
  // }

  scene.rayTrace();
  if (!scene.saveToPPM("output.ppm")) {
    std::cerr << "Failed to save image" << std::endl;
  }

  // TriangularLight tl = TriangularLight(
  //   Triangle(Point(0.5,0.5,0.5),
  //            Point(1.2,0.5,0.5),
  //            Point(0,0,0)),
  //   Vector(0,0,0),
  //   1
  // );
  // cout << "normal: " << tl.getNormal() << endl;

  return 0;
}

// int main(int argc, char *argv[]) {
//     const Camera CAM = Camera(Point(0.0, 0.0, 0.0),
//                               Vector(0.0, 0.0, -1.0),
//                               Vector(0.0, 1.0, 0.0),
//                               ImagePlane(-1, 1, -1, 1, 800, 800),
//                               1);
//     cout << "Calculating rays...\n";
//     const vector<Ray> RAYS = CAM.calculateViewRays();
//     // for(auto ray : RAYS) {
//     //     cout << ray << endl;
//     // }
//     Triangle triangle = Triangle(Point(0,0,0), Point(1,0,0), Point(0,1,0));
//     cout << "Vertices: " << triangle[0] << " | " << triangle[1]
//         << " | " << triangle[2] << endl;
//     cout << Vector::UNIT_VEC_U.crossProduct(Vector::UNIT_VEC_V) << endl;
//     cout << Vector::UNIT_VEC_U.dotProduct(Vector::UNIT_VEC_V) << endl;
//
//     Ray ray = Ray(Point(1,0,0), Point(0,0,0));
//     double t = ray.isIntersecting(triangle);
//     cout << "t is " << t << endl;
//     if (t < 0) {
//         cout << "Not intersecting" << endl;
//     } else {
//         cout << "Intersecting" << endl;
//     }
//     Scene scene = Scene("test");
//
//     return 0;
// }
