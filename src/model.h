#include "geometry.h"
#include "tgaimage.h"
#include <vector>

class Model {
private:
  std::vector<Vec3<float>> texture_coords;
  std::vector<Vec3<float>> vertices;
  std::vector<std::vector<int>> faces;
  TGAImage diffuse_map;

public:
  Model(const char *fileName);
  void loadTexture(const char *fileName, const char *suffix, TGAImage *ptr);
  std::vector<int> getFace(int i);
  Vec3<float> getVertex(int i);
  int numFaces();
  int numVertices();
  float minVal;
  float maxVal;
};
