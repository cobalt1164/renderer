#include "geometry.h"
#include <vector>

class Model {
private:
	std::vector<Vec3<float>> vertices;
	std::vector<std::vector<int>> faces;
public:
	Model(char* fileName);
	std::vector<int> getFace(int i);
	Vec3<float> getVertex(int i);
	int numFaces();
	int numVertices();
};