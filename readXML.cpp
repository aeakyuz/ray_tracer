#include "tinyxml2.h"
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace tinyxml2;
using namespace std;

// Data structures to hold scene information
struct Color {
  float r, g, b;
};

struct Vector3 {
  float x, y, z;
};

struct Vertex {
  float x, y, z;
};

struct TextureCoord {
  float u, v;
};

struct Normal {
  float x, y, z;
};

struct Material {
  int id;
  Color ambient;
  Color diffuse;
  Color specular;
  Color mirrorReflectance;
  float phongExponent;
  float textureFactor;
};

struct PointLight {
  int id;
  Vector3 position;
  Color intensity;
};

struct TriangularLight {
  int id;
  Vector3 vertex1, vertex2, vertex3;
  Color intensity;
};

struct Face {
  int vertexIndices[3];
  int textureIndices[3];
  int normalIndices[3];
};

struct Mesh {
  int id;
  int materialId;
  vector<Face> faces;
};

class Scene {
public:
  int maxRayTraceDepth;
  Color backgroundColor;

  // Camera parameters
  Vector3 cameraPosition;
  Vector3 cameraGaze;
  Vector3 cameraUp;
  float nearPlane[4]; // left, right, bottom, top
  float nearDistance;
  int imageResolution[2]; // nx, ny

  // Lighting
  Color ambientLight;
  vector<PointLight> pointLights;
  vector<TriangularLight> triangularLights;

  // Materials
  vector<Material> materials;

  // Geometry
  vector<Vertex> vertices;
  vector<TextureCoord> textureCoords;
  vector<Normal> normals;
  string textureImage;
  vector<Mesh> meshes;

  bool loadFromXML(const string &filename) {
    XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
      cerr << "Error loading XML file: " << filename << endl;
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

private:
  void parseFaceVertex(const string &vertexStr, Face &face, int vertexIndex) {
    size_t slash1 = vertexStr.find('/');
    size_t slash2 = vertexStr.find('/', slash1 + 1);

    if (slash1 != string::npos && slash2 != string::npos) {
      face.vertexIndices[vertexIndex] = stoi(vertexStr.substr(0, slash1)) - 1;
      face.textureIndices[vertexIndex] =
          stoi(vertexStr.substr(slash1 + 1, slash2 - slash1 - 1)) - 1;
      face.normalIndices[vertexIndex] = stoi(vertexStr.substr(slash2 + 1)) - 1;
    } else {
      // Handle error case
      cerr << "Malformed face vertex: " << vertexStr << endl;
    }
  }
  void parseBasicParameters(XMLElement *sceneElement) {
    // Max ray trace depth
    XMLElement *maxDepthElem =
        sceneElement->FirstChildElement("maxraytracedepth");
    if (maxDepthElem) {
      maxRayTraceDepth = maxDepthElem->IntText();
    }

    // Background color
    XMLElement *bgColorElem =
        sceneElement->FirstChildElement("backgroundColor");
    if (bgColorElem) {
      const char *bgColorText = bgColorElem->GetText();
      sscanf(bgColorText, "%f %f %f", &backgroundColor.r, &backgroundColor.g,
             &backgroundColor.b);
    }
  }

  void parseCamera(XMLElement *sceneElement) {
    XMLElement *cameraElem = sceneElement->FirstChildElement("camera");
    if (!cameraElem)
      return;

    // Camera position
    XMLElement *posElem = cameraElem->FirstChildElement("position");
    if (posElem) {
      const char *posText = posElem->GetText();
      sscanf(posText, "%f %f %f", &cameraPosition.x, &cameraPosition.y,
             &cameraPosition.z);
    }

    // Camera gaze direction
    XMLElement *gazeElem = cameraElem->FirstChildElement("gaze");
    if (gazeElem) {
      const char *gazeText = gazeElem->GetText();
      sscanf(gazeText, "%f %f %f", &cameraGaze.x, &cameraGaze.y, &cameraGaze.z);
    }

    // Camera up vector
    XMLElement *upElem = cameraElem->FirstChildElement("up");
    if (upElem) {
      const char *upText = upElem->GetText();
      sscanf(upText, "%f %f %f", &cameraUp.x, &cameraUp.y, &cameraUp.z);
    }

    // Near plane
    XMLElement *nearPlaneElem = cameraElem->FirstChildElement("nearPlane");
    if (nearPlaneElem) {
      const char *nearPlaneText = nearPlaneElem->GetText();
      sscanf(nearPlaneText, "%f %f %f %f", &nearPlane[0], &nearPlane[1],
             &nearPlane[2], &nearPlane[3]);
    }

    // Near distance
    XMLElement *nearDistElem = cameraElem->FirstChildElement("neardistance");
    if (nearDistElem) {
      nearDistance = nearDistElem->FloatText();
    }

    // Image resolution
    XMLElement *resElem = cameraElem->FirstChildElement("imageresolution");
    if (resElem) {
      const char *resText = resElem->GetText();
      sscanf(resText, "%d %d", &imageResolution[0], &imageResolution[1]);
    }
  }

  void parseLights(XMLElement *sceneElement) {
    XMLElement *lightsElem = sceneElement->FirstChildElement("lights");
    if (!lightsElem)
      return;

    // Ambient light
    XMLElement *ambientElem = lightsElem->FirstChildElement("ambientlight");
    if (ambientElem) {
      const char *ambientText = ambientElem->GetText();
      sscanf(ambientText, "%f %f %f", &ambientLight.r, &ambientLight.g,
             &ambientLight.b);
    }

    // Point lights
    XMLElement *pointLightElem = lightsElem->FirstChildElement("pointlight");
    while (pointLightElem) {
      PointLight light;
      light.id = pointLightElem->IntAttribute("id");

      XMLElement *posElem = pointLightElem->FirstChildElement("position");
      if (posElem) {
        const char *posText = posElem->GetText();
        sscanf(posText, "%f %f %f", &light.position.x, &light.position.y,
               &light.position.z);
      }

      XMLElement *intensityElem =
          pointLightElem->FirstChildElement("intensity");
      if (intensityElem) {
        const char *intensityText = intensityElem->GetText();
        sscanf(intensityText, "%f %f %f", &light.intensity.r,
               &light.intensity.g, &light.intensity.b);
      }

      pointLights.push_back(light);
      pointLightElem = pointLightElem->NextSiblingElement("pointlight");
    }

    // Triangular lights
    XMLElement *triLightElem = lightsElem->FirstChildElement("triangularlight");
    while (triLightElem) {
      TriangularLight light;
      light.id = triLightElem->IntAttribute("id");

      XMLElement *v1Elem = triLightElem->FirstChildElement("vertex1");
      if (v1Elem) {
        const char *v1Text = v1Elem->GetText();
        sscanf(v1Text, "%f %f %f", &light.vertex1.x, &light.vertex1.y,
               &light.vertex1.z);
      }

      XMLElement *v2Elem = triLightElem->FirstChildElement("vertex2");
      if (v2Elem) {
        const char *v2Text = v2Elem->GetText();
        sscanf(v2Text, "%f %f %f", &light.vertex2.x, &light.vertex2.y,
               &light.vertex2.z);
      }

      XMLElement *v3Elem = triLightElem->FirstChildElement("vertex3");
      if (v3Elem) {
        const char *v3Text = v3Elem->GetText();
        sscanf(v3Text, "%f %f %f", &light.vertex3.x, &light.vertex3.y,
               &light.vertex3.z);
      }

      XMLElement *intensityElem = triLightElem->FirstChildElement("intensity");
      if (intensityElem) {
        const char *intensityText = intensityElem->GetText();
        sscanf(intensityText, "%f %f %f", &light.intensity.r,
               &light.intensity.g, &light.intensity.b);
      }

      triangularLights.push_back(light);
      triLightElem = triLightElem->NextSiblingElement("triangularlight");
    }
  }

  void parseMaterials(XMLElement *sceneElement) {
    XMLElement *materialsElem = sceneElement->FirstChildElement("materials");
    if (!materialsElem)
      return;

    XMLElement *materialElem = materialsElem->FirstChildElement("material");
    while (materialElem) {
      Material mat;
      mat.id = materialElem->IntAttribute("id");

      XMLElement *ambientElem = materialElem->FirstChildElement("ambient");
      if (ambientElem) {
        const char *ambientText = ambientElem->GetText();
        sscanf(ambientText, "%f %f %f", &mat.ambient.r, &mat.ambient.g,
               &mat.ambient.b);
      }

      XMLElement *diffuseElem = materialElem->FirstChildElement("diffuse");
      if (diffuseElem) {
        const char *diffuseText = diffuseElem->GetText();
        sscanf(diffuseText, "%f %f %f", &mat.diffuse.r, &mat.diffuse.g,
               &mat.diffuse.b);
      }

      XMLElement *specularElem = materialElem->FirstChildElement("specular");
      if (specularElem) {
        const char *specularText = specularElem->GetText();
        sscanf(specularText, "%f %f %f", &mat.specular.r, &mat.specular.g,
               &mat.specular.b);
      }

      XMLElement *mirrorElem =
          materialElem->FirstChildElement("mirrorreflactance");
      if (mirrorElem) {
        const char *mirrorText = mirrorElem->GetText();
        sscanf(mirrorText, "%f %f %f", &mat.mirrorReflectance.r,
               &mat.mirrorReflectance.g, &mat.mirrorReflectance.b);
      }

      XMLElement *phongElem = materialElem->FirstChildElement("phongexponent");
      if (phongElem) {
        mat.phongExponent = phongElem->FloatText();
      }

      XMLElement *textureElem =
          materialElem->FirstChildElement("texturefactor");
      if (textureElem) {
        mat.textureFactor = textureElem->FloatText();
      }

      materials.push_back(mat);
      materialElem = materialElem->NextSiblingElement("material");
    }
  }

  void parseGeometry(XMLElement *sceneElement) {
    // Vertex data
    XMLElement *vertexDataElem = sceneElement->FirstChildElement("vertexdata");
    if (vertexDataElem) {
      const char *vertexText = vertexDataElem->GetText();
      stringstream ss(vertexText);
      Vertex v;
      while (ss >> v.x >> v.y >> v.z) {
        vertices.push_back(v);
      }
    }

    // Texture data
    XMLElement *textureDataElem =
        sceneElement->FirstChildElement("texturedata");
    if (textureDataElem) {
      const char *textureText = textureDataElem->GetText();
      stringstream ss(textureText);
      TextureCoord tc;
      while (ss >> tc.u >> tc.v) {
        textureCoords.push_back(tc);
      }
    }

    // Normal data
    XMLElement *normalDataElem = sceneElement->FirstChildElement("normaldata");
    if (normalDataElem) {
      const char *normalText = normalDataElem->GetText();
      stringstream ss(normalText);
      Normal n;
      while (ss >> n.x >> n.y >> n.z) {
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

  void parseObjects(XMLElement *sceneElement) {
    XMLElement *objectsElem = sceneElement->FirstChildElement("objects");
    if (!objectsElem)
      return;

    XMLElement *meshElem = objectsElem->FirstChildElement("mesh");
    while (meshElem) {
      Mesh mesh;
      mesh.id = meshElem->IntAttribute("id");

      XMLElement *materialIdElem = meshElem->FirstChildElement("materialid");
      if (materialIdElem) {
        mesh.materialId = materialIdElem->IntText();
      }

      XMLElement *facesElem = meshElem->FirstChildElement("faces");
      if (facesElem) {
        const char *facesText = facesElem->GetText();
        istringstream iss(facesText);
        string vertexStr;

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

          mesh.faces.push_back(face);
        }
      }

      meshes.push_back(mesh);
      meshElem = meshElem->NextSiblingElement("mesh");
    }
  }
};

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <scene.xml>" << endl;
    return 1;
  }

  Scene scene;
  if (!scene.loadFromXML(argv[1])) {
    cerr << "Failed to load scene file" << endl;
    return 1;
  }

  // Print some information to verify parsing
  cout << "Scene loaded successfully:" << endl;
  cout << "  Max ray trace depth: " << scene.maxRayTraceDepth << endl;
  cout << "  Background color: (" << scene.backgroundColor.r << ", "
       << scene.backgroundColor.g << ", " << scene.backgroundColor.b << ")"
       << endl;
  cout << "  Camera position: (" << scene.cameraPosition.x << ", "
       << scene.cameraPosition.y << ", " << scene.cameraPosition.z << ")"
       << endl;
  cout << "  Image resolution: " << scene.imageResolution[0] << "x"
       << scene.imageResolution[1] << endl;
  cout << "  Number of vertices: " << scene.vertices.size() << endl;
  cout << "  Number of meshes: " << scene.meshes.size() << endl;

  // Print face information for the first mesh
  if (!scene.meshes.empty()) {
    cout << "Number of faces parsed: " << scene.meshes[0].faces.size() << endl;
    // Modified debug output to show original indices (1-based)
    for (size_t i = 0; i < scene.meshes.front().faces.size(); i++) {
      const Face &f = scene.meshes.front().faces[i];
      cout << "Face " << i + 1 << ":\n";
      cout << "  Vertex indices: " << f.vertexIndices[0] + 1 << ", "
           << f.vertexIndices[1] + 1 << ", " << f.vertexIndices[2] + 1 << "\n";
      cout << "  Texture indices: " << f.textureIndices[0] + 1 << ", "
           << f.textureIndices[1] + 1 << ", " << f.textureIndices[2] + 1
           << "\n";
      cout << "  Normal indices: " << f.normalIndices[0] + 1 << ", "
           << f.normalIndices[1] + 1 << ", " << f.normalIndices[2] + 1 << "\n";
    }
  }
  return 0;
}
