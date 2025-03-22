#include "camera.hpp"
#include "triangle.hpp"
#include <iostream>

using namespace rtracer;
using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
    const Camera CAM = Camera(Point(0.0, 0.0, 0.0),
                              Vector(0.0, 0.0, -1.0),
                              Vector(0.0, 1.0, 0.0),
                              ImagePlane(-1, 1, -1, 1, 800, 800),
                              1);
    cout << "Calculating rays...\n";
    const vector<Ray> RAYS = CAM.calculateViewRays();
    // for(auto ray : RAYS) {
    //     cout << ray << endl;
    // }
    Triangle triangle = Triangle(Point(0,0,0), Point(1,0,0), Point(0,1,0));
    cout << "Vertices: " << triangle[0] << " | " << triangle[1]
        << " | " << triangle[2] << endl;
    return 0;
}
