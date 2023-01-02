#include "geometry.h"
#include "tgaimage.h"
#include <vector>

class Model {
private:
  std::vector<Vec3<float>> texture_coords;
  std::vector<Vec3<float>> vertices;
  std::vector<std::vector<int>> faces;
  std::vector<std::vector<int>> face_textures;

public:
  TGAImage diffuse_map;
  Model(const char *fileName);
  void loadTexture(const char *fileName, const char *suffix, TGAImage *ptr);
  std::vector<int> getFace(int i);
  std::vector<int> getFaceTexture(int i);
  Vec3<float> getTextureVertex(int i);
  Vec3<float> getVertex(int i);
  int numFaces();
  int numVertices();
  float minVal;
  float maxVal;
};
