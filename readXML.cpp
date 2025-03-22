#include <cstdio>
#include <iostream>
#include "tinyxml2.h"
#include "util.hpp"

using std::cerr;
using std::endl;
using std::string;
using std::cout;

using tinyxml2::XMLDocument;
using tinyxml2::XML_SUCCESS;
using tinyxml2::XMLElement;

using rtracer::Point;
using rtracer::Vector;
using rtracer::Ray;

int main() {
    XMLDocument doc;
    if (doc.LoadFile("complete.xml") != XML_SUCCESS) {
        cerr << "Error loading XML file!" << endl;
        return -1;
    }

    // Access the root element
    XMLElement* scene = doc.FirstChildElement("scene");
    if (!scene) {
        cerr << "No <scene> element found!" << endl;
        return -1;
    }

    // Read maxraytracedepth
    XMLElement* maxDepthElem = scene->FirstChildElement("maxraytracedepth");
    int maxRayTraceDepth = maxDepthElem ? std::stoi(maxDepthElem->GetText()) : 6;

    // Read backgroundColor
    XMLElement* bgColorElem = scene->FirstChildElement("backgroundColor");
    float bgColor[3] = {0, 0, 0};
    if (bgColorElem) {
        sscanf(bgColorElem->GetText(), "%f %f %f", &bgColor[0], &bgColor[1], &bgColor[2]);
    }

    // Read camera
    XMLElement* cameraElem = scene->FirstChildElement("camera");
    if (cameraElem) {
        XMLElement* posElem = cameraElem->FirstChildElement("position");
        float camPos[3] = {0, 0, 0};
        if (posElem) {
            sscanf(posElem->GetText(), "%f %f %f", &camPos[0], &camPos[1], &camPos[2]);
        }

        XMLElement* gazeElem = cameraElem->FirstChildElement("gaze");
        float gaze[3] = {0, 0, -1};
        if (gazeElem) {
            sscanf(gazeElem->GetText(), "%f %f %f", &gaze[0], &gaze[1], &gaze[2]);
        }

        XMLElement* upElem = cameraElem->FirstChildElement("up");
        float up[3] = {0, 1, 0};
        if (upElem) {
            sscanf(upElem->GetText(), "%f %f %f", &up[0], &up[1], &up[2]);
        }

        XMLElement* nearPlaneElem = cameraElem->FirstChildElement("nearPlane");
        float nearPlane[4] = {-1, 1, -1, 1};
        if (nearPlaneElem) {
            sscanf(nearPlaneElem->GetText(), "%f %f %f %f", &nearPlane[0], &nearPlane[1], &nearPlane[2], &nearPlane[3]);
        }

        XMLElement* nearDistElem = cameraElem->FirstChildElement("neardistance");
        float nearDistance = nearDistElem ? std::stof(nearDistElem->GetText()) : 1.0f;

        XMLElement* resElem = cameraElem->FirstChildElement("imageresolution");
        int resolution[2] = {800, 800};
        if (resElem) {
            sscanf(resElem->GetText(), "%d %d", &resolution[0], &resolution[1]);
        }
    }

    // Read lights
    XMLElement* lightsElem = scene->FirstChildElement("lights");
    if (lightsElem) {
        XMLElement* ambientElem = lightsElem->FirstChildElement("ambientlight");
        float ambientLight[3] = {25, 25, 25};
        if (ambientElem) {
            sscanf(ambientElem->GetText(), "%f %f %f", &ambientLight[0], &ambientLight[1], &ambientLight[2]);
        }

        XMLElement* pointLightElem = lightsElem->FirstChildElement("pointlight");
        while (pointLightElem) {
            int id = pointLightElem->IntAttribute("id");
            XMLElement* posElem = pointLightElem->FirstChildElement("position");
            float pos[3] = {0, 0, 0};
            if (posElem) {
                sscanf(posElem->GetText(), "%f %f %f", &pos[0], &pos[1], &pos[2]);
            }

            XMLElement* intensityElem = pointLightElem->FirstChildElement("intensity");
            float intensity[3] = {1000, 1000, 1000};
            if (intensityElem) {
                sscanf(intensityElem->GetText(), "%f %f %f", &intensity[0], &intensity[1], &intensity[2]);
            }

            pointLightElem = pointLightElem->NextSiblingElement("pointlight");
        }

        XMLElement* triLightElem = lightsElem->FirstChildElement("triangularlight");
        while (triLightElem) {
            int id = triLightElem->IntAttribute("id");
            XMLElement* v1Elem = triLightElem->FirstChildElement("vertex1");
            float v1[3] = {0, 0, 0};
            if (v1Elem) {
                sscanf(v1Elem->GetText(), "%f %f %f", &v1[0], &v1[1], &v1[2]);
            }

            XMLElement* v2Elem = triLightElem->FirstChildElement("vertex2");
            float v2[3] = {1.2f, 0.5f, 0.5f};
            if (v2Elem) {
                sscanf(v2Elem->GetText(), "%f %f %f", &v2[0], &v2[1], &v2[2]);
            }

            XMLElement* v3Elem = triLightElem->FirstChildElement("vertex3");
            float v3[3] = {0.5f, 0.5f, 0.5f};
            if (v3Elem) {
                sscanf(v3Elem->GetText(), "%f %f %f", &v3[0], &v3[1], &v3[2]);
            }

            XMLElement* intensityElem = triLightElem->FirstChildElement("intensity");
            float intensity[3] = {800, 800, 800};
            if (intensityElem) {
                sscanf(intensityElem->GetText(), "%f %f %f", &intensity[0], &intensity[1], &intensity[2]);
            }

            triLightElem = triLightElem->NextSiblingElement("triangularlight");
        }
    }

    // Read materials
    XMLElement* materialsElem = scene->FirstChildElement("materials");
    if (materialsElem) {
        XMLElement* materialElem = materialsElem->FirstChildElement("material");
        while (materialElem) {
            int id = materialElem->IntAttribute("id");
            XMLElement* ambientElem = materialElem->FirstChildElement("ambient");
            float ambient[3] = {1, 1, 1};
            if (ambientElem) {
                sscanf(ambientElem->GetText(), "%f %f %f", &ambient[0], &ambient[1], &ambient[2]);
            }

            XMLElement* diffuseElem = materialElem->FirstChildElement("diffuse");
            float diffuse[3] = {1, 1, 1};
            if (diffuseElem) {
                sscanf(diffuseElem->GetText(), "%f %f %f", &diffuse[0], &diffuse[1], &diffuse[2]);
            }

            XMLElement* specularElem = materialElem->FirstChildElement("specular");
            float specular[3] = {1, 1, 1};
            if (specularElem) {
                sscanf(specularElem->GetText(), "%f %f %f", &specular[0], &specular[1], &specular[2]);
            }

            XMLElement* mirrorElem = materialElem->FirstChildElement("mirrorreflactance");
            float mirror[3] = {0, 0, 0};
            if (mirrorElem) {
                sscanf(mirrorElem->GetText(), "%f %f %f", &mirror[0], &mirror[1], &mirror[2]);
            }

            XMLElement* phongElem = materialElem->FirstChildElement("phongexponent");
            int phongExponent = phongElem ? std::stoi(phongElem->GetText()) : 1;

            XMLElement* textureElem = materialElem->FirstChildElement("texturefactor");
            float textureFactor = textureElem ? std::stof(textureElem->GetText()) : 0.5f;

            materialElem = materialElem->NextSiblingElement("material");
        }
    }

    // Read vertex data
    XMLElement* vertexDataElem = scene->FirstChildElement("vertexdata");
    if (vertexDataElem) {
        const char* vertexText = vertexDataElem->GetText();
        // Parse vertexText into a list of floats
        // You can use stringstream or other methods to split the string into floats
    }

    // Read texture data
    XMLElement* textureDataElem = scene->FirstChildElement("texturedata");
    if (textureDataElem) {
        const char* textureText = textureDataElem->GetText();
        // Parse textureText into a list of floats
    }

    // Read texture image
    XMLElement* textureImageElem = scene->FirstChildElement("textureimage");
    string textureImage = textureImageElem ? textureImageElem->GetText() : "";

    // Read objects
    XMLElement* objectsElem = scene->FirstChildElement("objects");
    if (objectsElem) {
        XMLElement* meshElem = objectsElem->FirstChildElement("mesh");
        while (meshElem) {
            int id = meshElem->IntAttribute("id");
            XMLElement* materialIdElem = meshElem->FirstChildElement("materialid");
            int materialId = materialIdElem ? std::stoi(materialIdElem->GetText()) : 1;

            XMLElement* facesElem = meshElem->FirstChildElement("faces");
            if (facesElem) {
                const char* facesText = facesElem->GetText();
                // Parse facesText into a list of integers
            }

            meshElem = meshElem->NextSiblingElement("mesh");
        }
    }

    return 0;
}
