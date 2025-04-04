#ifndef SCENE_HPP_
#define SCENE_HPP_

#include "ambientLight.hpp"
#include "camera.hpp"
#include "material.hpp"
#include "mesh.hpp"
#include "point.hpp"
#include "pointLight.hpp"
#include "tinyxml2.h"
#include "triangularLight.hpp"
#include "vector.hpp"
#include <cstddef>
#include <string>
#include <vector>

namespace rtracer {

class Scene {
public:
  size_t maxRTDepth;
  Vector bgColor;
  Camera cam;
  AmbientLight ambient;
  std::vector<PointLight> pointLights;
  std::vector<TriangularLight> triangularLights;
  std::vector<Material> materials;
  std::vector<Point> vertices;
  std::vector<Vector> textureData;
  std::vector<Vector> normals;
  std::string textureImage;
  std::vector<Mesh> meshes;
  std::vector<std::vector<Vector>> image;

  Scene();
  Scene(const std::string xmlPath);
  bool loadFromXML(const std::string xmlPath);
  void rayTrace(void);
  const Vector getColor(const Point &x, const Vector &w_o, const Material &mat,
                        const Face &obj) const;
  bool saveToPPM(const std::string &filename) const;
  const Vector findMaxColorValue(void) const;
  void debug() const;
private:
  void parseBasicParameters(tinyxml2::XMLElement *);
  void parseCamera(tinyxml2::XMLElement *);
  void parseLights(tinyxml2::XMLElement *);
  void parseMaterials(tinyxml2::XMLElement *);
  void parseGeometry(tinyxml2::XMLElement *);
  void parseObjects(tinyxml2::XMLElement *);
  void parseFaceVertex(const std::string &, Face &, int);
};

} // namespace rtracer

#endif // SCENE_HPP_
