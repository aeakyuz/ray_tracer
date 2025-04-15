#include "camera.hpp"
#include "point.hpp"
#include "scene.hpp"
#include "vector.hpp"
#include "ray.hpp"

#include <iostream>
#include <thread>

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

  vector<Ray> viewRays = scene.cam.calculateViewRays();
  const int coreCount = std::thread::hardware_concurrency();
  size_t rayPerCore = viewRays.size() / coreCount;
  std::vector<std::thread> threads;

  for (size_t i = 0; i < coreCount; ++i) {
      size_t begin = i * rayPerCore;
      size_t end = (i == coreCount - 1) ? viewRays.size() : begin + rayPerCore;
      threads.emplace_back(&Scene::rayTrace, &scene, std::ref(viewRays), begin, end);
  }

  for (auto& thread : threads) {
      thread.join();
  }
  if (!scene.saveToPPM("output.ppm")) {
    std::cerr << "Failed to save image" << std::endl;
  }

  return 0;
}
