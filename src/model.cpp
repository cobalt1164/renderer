#include "model.h"
#include <fstream>
#include <string>

Model::Model(char* fileName) {
	std::ifstream file;
	file.open(fileName);
	if (file.fail()) return;
	while (!file.eof()) {
		std::string v_f;
		file >> v_f;
		if (v_f.compare("v") == 0) {
			float x, y, z;
			file >> x >> y >> z;
			vertices.push_back(Vec3<float>(x, y, z));
		}
		else if (v_f.compare("f") == 0) {
			std::string discard;
			for (int i = 0; i < 3; i++) {
				std::vector<int> face;
				int idx1, idx2, idx3;
				file >> idx1 >> discard >> idx2 >> discard >> idx3;
				idx1--;
				idx2--;
				idx3--;
				face.push_back(idx1);
				face.push_back(idx2);
				face.push_back(idx3);
				faces.push_back(face);
			}
		}
		else {
			std::string discard;
			std::getline(file, discard);
			continue;
		}
	}
}
