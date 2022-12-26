#include "model.h"
#include <fstream>
#include <iostream>
#include <string>

Model::Model(const char *fileName) {
  std::ifstream file;
  file.open(fileName);
  if (file.fail())
    return;
  std::string discard;
  do {
    std::string v_f;
    file >> v_f;
    if (v_f.compare("v") == 0) {
      float x, y, z;
      file >> x >> y >> z;
      vertices.push_back(Vec3<float>(x, y, z));
    } else if (v_f.compare("f") == 0) {
      std::string trash;
      std::vector<int> face;
      for (int i = 0; i < 3; i++) {
        int idx;
        file >> idx >> trash;
        face.push_back(idx - 1);
      }
      faces.push_back(face);
    } else {
      continue;
    }
  } while (std::getline(file, discard));
  std::cout << "vertices: " << vertices.size() << " | faces: " << faces.size()
            << std::endl;
}

int Model::numFaces() { return faces.size(); }

int Model::numVertices() { return vertices.size(); }

Vec3<float> Model::getVertex(int i) { return vertices[i]; }

std::vector<int> Model::getFace(int i) { return faces[i]; }
