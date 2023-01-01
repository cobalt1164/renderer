#include "model.h"
#include "tgaimage.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

Model::Model(const char *fileName) {
  minVal = std::numeric_limits<float>::infinity();
  maxVal = -std::numeric_limits<float>::infinity();
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
      minVal = std::min(minVal, x);
      minVal = std::min(minVal, y);
      minVal = std::min(minVal, z);
      maxVal = std::max(maxVal, x);
      maxVal = std::max(maxVal, y);
      maxVal = std::max(maxVal, z);
      vertices.push_back(Vec3<float>(x, y, z));
    } else if (v_f.compare("f") == 0) {
      std::string trash;
      char separator;
      std::vector<int> face;
      std::vector<int> face_texture;
      for (int i = 0; i < 3; i++) {
        int idx;
        int texture_idx;
        file >> idx >> separator >> texture_idx >> trash;
        face.push_back(idx - 1);
        face_texture.push_back(texture_idx - 1);
      }
      faces.push_back(face);
      face_textures.push_back(face_texture);
    } else if (v_f.compare("vt") == 0) {
      float u, v, w;
      file >> u >> v >> w;
      texture_coords.push_back(Vec3<float>(u, v, w));
    } else {
      continue;
    }
  } while (std::getline(file, discard));
  loadTexture(fileName, "_diffuse.tga", &diffuse_map);
  std::cout << "vertices: " << vertices.size() << " | faces: " << faces.size()
            << std::endl;
}

void Model::loadTexture(const char *fileName, const char *suffix,
                        TGAImage *ptr) {
  std::string file(fileName);
  size_t dot_index = file.find_last_of(".");
  file = file.substr(0, dot_index) + std::string(suffix);
  std::cout << file << " texture: "
            << (ptr->read_tga_file(file.c_str()) ? "succeeded" : "failed")
            << std::endl;
}

int Model::numFaces() { return faces.size(); }

int Model::numVertices() { return vertices.size(); }

Vec3<float> Model::getVertex(int i) {
  // Normalize for non-scaled obj files
  // AKA convert to normalized world coordinates
  Vec3<float> vertex = vertices[i];
  float newX = (vertex.x - minVal) / (maxVal - minVal);
  float newY = (vertex.y - minVal) / (maxVal - minVal);
  float newZ = (vertex.z - minVal) / (maxVal - minVal);
  return Vec3<float>(newX, newY, newZ);
}

std::vector<int> Model::getFace(int i) { return faces[i]; }

std::vector<int> Model::getFaceTexture(int i) {return face_textures[i]; }
