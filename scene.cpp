#include "scene.hpp"
#include "ambientLight.hpp"
#include "tinyxml2.h"
#include "triangle.hpp"
#include "triangularLight.hpp"
#include "util.hpp"
#include "vector.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace rtracer;
using std::cerr;
using std::endl;
using tinyxml2::XML_SUCCESS;
using tinyxml2::XMLDocument;
using tinyxml2::XMLElement;

Scene::Scene()
    : maxRTDepth(0), bgColor(Vector()), cam(Camera()), ambient(AmbientLight()),
      pointLights(std::vector<PointLight>()),
      triangularLights(std::vector<TriangularLight>()),
      materials(std::vector<Material>()), vertices(std::vector<Point>()),
      textureData(std::vector<Vector>()), normals(std::vector<Vector>()),
      textureImage(std::string()), meshes(std::vector<Mesh>()),
      image(std::vector<std::vector<Vector>>()) {}

Scene::Scene(std::string xmlPath) : Scene() {
  if (!loadFromXML(xmlPath)) {
    throw std::exception();
  }
}

bool Scene::loadFromXML(const std::string xmlPath) {
  XMLDocument doc;
  if (doc.LoadFile(xmlPath.c_str()) != XML_SUCCESS) {
    cerr << "Error loading XML file: " << xmlPath << endl;
    return false;
  }

  XMLElement *sceneElement = doc.FirstChildElement("scene");
  if (!sceneElement) {
    cerr << "No <scene> element found" << endl;
    return false;
  }

  // Parse basic scene parameters
  parseBasicParameters(sceneElement);

  // Parse camera
  parseCamera(sceneElement);

  // Parse lights
  parseLights(sceneElement);

  // Parse materials
  parseMaterials(sceneElement);

  // Parse geometry data
  parseGeometry(sceneElement);

  // Parse objects
  parseObjects(sceneElement);

  return true;
}

void Scene::parseBasicParameters(XMLElement *sceneElement) {
  // Max ray trace depth
  XMLElement *maxDepthElem =
      sceneElement->FirstChildElement("maxraytracedepth");
  if (maxDepthElem) {
    maxRTDepth = maxDepthElem->IntText();
  }

  // Background color
  XMLElement *bgColorElem = sceneElement->FirstChildElement("backgroundColor");
  if (bgColorElem) {
    const char *bgColorText = bgColorElem->GetText();
    sscanf(bgColorText, "%lf %lf %lf", &bgColor[0], &bgColor[1], &bgColor[2]);
  }
}

void Scene::parseCamera(XMLElement *sceneElement) {
  XMLElement *cameraElem = sceneElement->FirstChildElement("camera");
  if (!cameraElem)
    return;

  // Camera position
  XMLElement *posElem = cameraElem->FirstChildElement("position");
  if (posElem) {
    const char *posText = posElem->GetText();
    double pos[3];
    sscanf(posText, "%lf %lf %lf", &pos[0], &pos[1], &pos[2]);
    cam.setPosition(Point(pos[0], pos[1], pos[2]));
  }

  // Camera gaze direction
  XMLElement *gazeElem = cameraElem->FirstChildElement("gaze");
  if (gazeElem) {
    const char *gazeText = gazeElem->GetText();
    double gaze[3];
    sscanf(gazeText, "%lf %lf %lf", &gaze[0], &gaze[1], &gaze[2]);
    cam.setGaze(Vector(gaze[0], gaze[1], gaze[2]));
  }

  // Camera up vector
  XMLElement *upElem = cameraElem->FirstChildElement("up");
  if (upElem) {
    const char *upText = upElem->GetText();
    double up[3];
    sscanf(upText, "%lf %lf %lf", &up[0], &up[1], &up[2]);
    cam.setUp(Vector(up[0], up[1], up[2]));
  }

  // Near plane
  double coords[4] = {0.0};
  int nx = 0, ny = 0;
  XMLElement *nearPlaneElem = cameraElem->FirstChildElement("nearPlane");
  if (nearPlaneElem) {
    const char *nearPlaneText = nearPlaneElem->GetText();
    sscanf(nearPlaneText, "%lf %lf %lf %lf", &coords[0], &coords[1], &coords[2],
           &coords[3]);
  }

  // Near distance
  XMLElement *nearDistElem = cameraElem->FirstChildElement("neardistance");
  if (nearDistElem) {
    cam.setDistance(nearDistElem->FloatText());
  }

  // Image resolution
  XMLElement *resElem = cameraElem->FirstChildElement("imageresolution");
  if (resElem) {
    const char *resText = resElem->GetText();
    sscanf(resText, "%d %d", &nx, &ny);
  }
  cam.setImagePlane(
      ImagePlane(coords[0], coords[1], coords[2], coords[3], nx, ny));
  vector<Vector> row = vector<Vector>(nx);
  image = vector<vector<Vector>>(ny, row);
}

void Scene::parseLights(XMLElement *sceneElement) {
  XMLElement *lightsElem = sceneElement->FirstChildElement("lights");
  if (!lightsElem)
    return;

  // Ambient light
  XMLElement *ambientElem = lightsElem->FirstChildElement("ambientlight");
  if (ambientElem) {
    const char *ambientText = ambientElem->GetText();
    double arr[3];
    sscanf(ambientText, "%lf %lf %lf", &arr[0], &arr[1], &arr[2]);
    ambient = AmbientLight(Vector(arr[0], arr[1], arr[2]));
  }

  // Point lights
  XMLElement *pointLightElem = lightsElem->FirstChildElement("pointlight");
  while (pointLightElem) {
    PointLight light = PointLight();
    light.setId(pointLightElem->IntAttribute("id"));

    XMLElement *posElem = pointLightElem->FirstChildElement("position");
    if (posElem) {
      const char *posText = posElem->GetText();
      double arr[3];
      sscanf(posText, "%lf %lf %lf", &arr[0], &arr[1], &arr[2]);
      light.setPos(Point(arr[0], arr[1], arr[2]));
    }

    XMLElement *intensityElem = pointLightElem->FirstChildElement("intensity");
    if (intensityElem) {
      const char *intensityText = intensityElem->GetText();
      double arr[3];
      sscanf(intensityText, "%lf %lf %lf", &arr[0], &arr[1], &arr[2]);
      light.setIntensity(Vector(arr[0], arr[1], arr[2]));
    }

    pointLights.push_back(light);
    pointLightElem = pointLightElem->NextSiblingElement("pointlight");
  }

  // Triangular lights
  XMLElement *triLightElem = lightsElem->FirstChildElement("triangularlight");
  while (triLightElem) {
    TriangularLight light;
    Point vertices[3];
    light.setId(triLightElem->IntAttribute("id"));

    XMLElement *v1Elem = triLightElem->FirstChildElement("vertex1");
    if (v1Elem) {
      const char *v1Text = v1Elem->GetText();
      double arr[3];
      sscanf(v1Text, "%lf %lf %lf", &arr[0], &arr[1], &arr[2]);
      vertices[0] = Point(arr[0], arr[1], arr[2]);
    }

    XMLElement *v2Elem = triLightElem->FirstChildElement("vertex2");
    if (v2Elem) {
      const char *v2Text = v2Elem->GetText();
      double arr[3];
      sscanf(v2Text, "%lf %lf %lf", &arr[0], &arr[1], &arr[2]);
      vertices[1] = Point(arr[0], arr[1], arr[2]);
    }

    XMLElement *v3Elem = triLightElem->FirstChildElement("vertex3");
    if (v3Elem) {
      const char *v3Text = v3Elem->GetText();
      double arr[3];
      sscanf(v3Text, "%lf %lf %lf", &arr[0], &arr[1], &arr[2]);
      vertices[2] = Point(arr[0], arr[1], arr[2]);
    }
    Triangle triangle = Triangle(vertices[0], vertices[1], vertices[2]);
    light.setVertices(triangle);

    XMLElement *intensityElem = triLightElem->FirstChildElement("intensity");
    if (intensityElem) {
      const char *intensityText = intensityElem->GetText();
      double arr[3];
      sscanf(intensityText, "%lf %lf %lf", &arr[0], &arr[1], &arr[2]);
      light.setIntensity(Vector(arr[0], arr[1], arr[2]));
    }

    triangularLights.push_back(light);
    triLightElem = triLightElem->NextSiblingElement("triangularlight");
  }
}

void Scene::parseMaterials(XMLElement *sceneElement) {
  XMLElement *materialsElem = sceneElement->FirstChildElement("materials");
  if (!materialsElem)
    return;

  XMLElement *materialElem = materialsElem->FirstChildElement("material");
  while (materialElem) {
    Material mat;
    mat.setId(materialElem->IntAttribute("id"));

    XMLElement *ambientElem = materialElem->FirstChildElement("ambient");
    if (ambientElem) {
      const char *ambientText = ambientElem->GetText();
      double arr[3];
      sscanf(ambientText, "%lf %lf %lf", &arr[0], &arr[1], &arr[2]);
      mat.setAmbient(Vector(arr[0], arr[1], arr[2]));
    }

    XMLElement *diffuseElem = materialElem->FirstChildElement("diffuse");
    if (diffuseElem) {
      const char *diffuseText = diffuseElem->GetText();
      double arr[3];
      sscanf(diffuseText, "%lf %lf %lf", &arr[0], &arr[1], &arr[2]);
      mat.setDiffuse(Vector(arr[0], arr[1], arr[2]));
    }

    XMLElement *specularElem = materialElem->FirstChildElement("specular");
    if (specularElem) {
      const char *specularText = specularElem->GetText();
      double arr[3];
      sscanf(specularText, "%lf %lf %lf", &arr[0], &arr[1], &arr[2]);
      mat.setSpecular(Vector(arr[0], arr[1], arr[2]));
    }

    XMLElement *mirrorElem =
        materialElem->FirstChildElement("mirrorreflactance");
    if (mirrorElem) {
      const char *mirrorText = mirrorElem->GetText();
      double arr[3];
      sscanf(mirrorText, "%lf %lf %lf", &arr[0], &arr[1], &arr[2]);
      mat.setMirror(Vector(arr[0], arr[1], arr[2]));
    }

    XMLElement *phongElem = materialElem->FirstChildElement("phongexponent");
    if (phongElem) {
      mat.setPhonge(phongElem->FloatText());
    }

    XMLElement *textureElem = materialElem->FirstChildElement("texturefactor");
    if (textureElem) {
      mat.setTextureFactor(textureElem->FloatText());
    }

    materials.push_back(mat);
    materialElem = materialElem->NextSiblingElement("material");
  }
}

void Scene::parseGeometry(XMLElement *sceneElement) {
  // Vertex data
  XMLElement *vertexDataElem = sceneElement->FirstChildElement("vertexdata");
  if (vertexDataElem) {
    const char *vertexText = vertexDataElem->GetText();
    std::stringstream ss(vertexText);
    Point v;
    while (ss >> v[0] >> v[1] >> v[2]) {
      vertices.push_back(v);
    }
  }

  // Texture data
  XMLElement *textureDataElem = sceneElement->FirstChildElement("texturedata");
  if (textureDataElem) {
    const char *textureText = textureDataElem->GetText();
    std::stringstream ss(textureText);
    Vector tc = Vector();
    while (ss >> tc[1] >> tc[0]) {
      textureData.push_back(tc);
    }
  }

  // Normal data
  XMLElement *normalDataElem = sceneElement->FirstChildElement("normaldata");
  if (normalDataElem) {
    const char *normalText = normalDataElem->GetText();
    std::stringstream ss(normalText);
    Vector n;
    while (ss >> n[0] >> n[1] >> n[2]) {
      normals.push_back(n);
    }
  }

  // Texture image
  XMLElement *textureImageElem =
      sceneElement->FirstChildElement("textureimage");
  if (textureImageElem) {
    textureImage = textureImageElem->GetText();
  }
}

void Scene::parseObjects(XMLElement *sceneElement) {
  XMLElement *objectsElem = sceneElement->FirstChildElement("objects");
  if (!objectsElem)
    return;

  XMLElement *meshElem = objectsElem->FirstChildElement("mesh");
  while (meshElem) {
    Mesh mesh;
    mesh.setId(meshElem->IntAttribute("id"));

    XMLElement *materialIdElem = meshElem->FirstChildElement("materialid");
    if (materialIdElem) {
      size_t id = materialIdElem->IntText();
      for (auto mat : materials) {
        if (mat.getId() == id) {
          mesh.setMaterial(mat);
        }
      }
    }

    XMLElement *facesElem = meshElem->FirstChildElement("faces");
    if (facesElem) {
      const char *facesText = facesElem->GetText();
      std::istringstream iss(facesText);
      std::string vertexStr;

      while (iss >> vertexStr) {
        Face face;
        // First vertex
        parseFaceVertex(vertexStr, face, 0);

        // Second vertex (must exist for a valid face)
        if (!(iss >> vertexStr))
          break;
        parseFaceVertex(vertexStr, face, 1);

        // Third vertex (must exist for a valid face)
        if (!(iss >> vertexStr))
          break;
        parseFaceVertex(vertexStr, face, 2);

        mesh.push_back(face);
      }
    }

    meshes.push_back(mesh);
    meshElem = meshElem->NextSiblingElement("mesh");
  }
}

void Scene::parseFaceVertex(const std::string &vertexStr, Face &face,
                            int vertexIndex) {
  size_t slash1 = vertexStr.find('/');
  size_t slash2 = vertexStr.find('/', slash1 + 1);

  int vertexId = stoi(vertexStr.substr(0, slash1)) - 1;
  int textureId = stoi(vertexStr.substr(slash1 + 1, slash2 - slash1 - 1)) - 1;
  int normalId = stoi(vertexStr.substr(slash2 + 1)) - 1;

  if (slash1 != std::string::npos && slash2 != std::string::npos) {
    face.setVertex(vertexIndex, vertices.at(vertexId));
    face.setTexture(textureData.at(textureId));
    face.setNormal(normals.at(normalId));
  } else {
    // Handle error case
    cerr << "Malformed face vertex: " << vertexStr << endl;
  }
}

void Scene::rayTrace(void) {
  const Point M = cam.getPosition() + (cam.getGaze() * cam.getDistance());
  const Point Q = M + (Vector::UNIT_VEC_U * cam.getImagePlane().getL()) +
                  (Vector::UNIT_VEC_V * cam.getImagePlane().getT());

  for (size_t i = 0; i < cam.getImagePlane().getNx(); ++i) {
    for (size_t j = 0; j < cam.getImagePlane().getNy(); ++j) {
      const double Su =
          (cam.getImagePlane().getR() - cam.getImagePlane().getL()) *
          (i + 0.5) / cam.getImagePlane().getNx();
      const double Sv =
          ((cam.getImagePlane().getT() - cam.getImagePlane().getB()) *
           (j + 0.5) / cam.getImagePlane().getNy());
      const Point S =
          Q + (Vector::UNIT_VEC_U * Su) + (-Vector::UNIT_VEC_V * Sv);

      // Create ray from camera position to S
      Ray viewRay = Ray(cam.getPosition(), S).normalizeRay();
      double t_min = -1.0;
      Face obj = Face();
      bool found = false;
      for (auto mesh : meshes) {
        for (auto face : mesh) {
          double t = viewRay.isIntersecting(face);
          if (t > 0) { // if the ray intersects with the obj
            if (t < t_min || t_min == -1.0) {
              t_min = t;
              obj = face;
              found = true;
            }
          }
        }
        if (found) { // if obj is not null
          Point x = viewRay.calculatePoint(t_min);
          // std::cout << "itsct pnt: " << x << endl;
          image[j][i] =
              getColor(x, viewRay.getDirection(), mesh.getMaterial(), obj);
        } else {
          image[j][i] = bgColor;
        }
      }
    }
  }
}

const Vector Scene::getColor(const Point &x, const Vector &w_o,
                             const Material &mat, const Face &obj) const {
  // calculate ambient
  Vector L_a = Vector(ambient.getLum().getU() * mat.getAmbient().getU(),
                      ambient.getLum().getV() * mat.getAmbient().getV(),
                      ambient.getLum().getW() * mat.getAmbient().getW());
  // calculate diffuse
  Vector L_d = Vector();
  for (auto pl : pointLights) {
    const Ray xToLight = Ray(x, pl.getPos());
    // std::cout << "xToLight: " << xToLight.getDirection() << endl;
    double cos = xToLight.getDirection().dotProduct(obj.getNormal());
    // std::cout << "cos: " << cos << endl;
    cos = std::max(0.0, cos);
    double dist = x.calcDistance(pl.getPos());
    Vector v = Vector(
      mat.getDiffuse().getU() * cos * pl.getIntensity().getU() / (dist*dist),
      mat.getDiffuse().getV() * cos * pl.getIntensity().getV() / (dist*dist),
      mat.getDiffuse().getW() * cos * pl.getIntensity().getW() / (dist*dist)
    );
    L_d = L_d + v;
  }

  // calculate specular
  Vector L_s = Vector();
  return L_a + L_d + L_s;
}

bool Scene::saveToPPM(const std::string &filename) const {
  if (image.empty())
    return false;

  // Find maximum color value in the image
  double maxVal = findMaxColorValue();
  // std::cout << "Max val is " << maxVal << endl;
  if (maxVal <= 0)
    maxVal = 1.0; // Avoid division by zero

  std::ofstream file(filename, std::ios::binary | std::ios::trunc);
  if (!file)
    return false;

  file << "P6\n" << image[0].size() << " " << image.size() << "\n255\n";

  for (const auto &row : image) {
    for (const Vector &pixel : row) {
      // // Normalize and apply simple gamma correction
      // Vector normalized(
      //     pow(pixel.getU() / maxVal, 1/2.2f),
      //     pow(pixel.getV() / maxVal, 1/2.2f),
      //     pow(pixel.getW() / maxVal, 1/2.2f)
      // );
      //
      // // Clamp and convert to 8-bit
      // unsigned char r = static_cast<unsigned
      // char>(std::clamp(normalized.getU(), 0.0, 1.0) * 255); unsigned char
      // g = static_cast<unsigned char>(std::clamp(normalized.getV(),
      // 0.0, 1.0) * 255); unsigned char b = static_cast<unsigned
      // char>(std::clamp(normalized.getW(), 0.0, 1.0) * 255);

      // file << r << g << b;
      unsigned char r = static_cast<unsigned char>(pixel.getU());
      unsigned char g = static_cast<unsigned char>(pixel.getV());
      unsigned char b = static_cast<unsigned char>(pixel.getW());
      // printf("r,g,b : %d,%d,%d\n", r, g, b);
      file << r << g << b;
    }
  }
  return true;
}

double Scene::findMaxColorValue() const {
  double maxVal = 0.0;
  for (const auto &row : image) {
    for (const Vector &pixel : row) {
      maxVal = std::max({maxVal, pixel.getU(), pixel.getV(), pixel.getW(),
                         bgColor.getU(), bgColor.getV(), bgColor.getW()});
    }
  }
  return maxVal;
}

void Scene::debug() const {
  for (size_t i = 0; i < image.size(); i = i + 50) {
    for (size_t j = 0; j < image[i].size(); j = j + 50) {
      printf("Vector[%zu][%zu] = ", i, j);
      std::cout << image[i][j] << endl;
    }
  }
}
