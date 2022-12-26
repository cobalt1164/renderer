#include "model.h"
#include "tgaimage.h"
#include <cmath>
#include <iostream>
#include <type_traits>
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 128);
const TGAColor green = TGAColor(0, 255, 0, 128);

void draw_line(int x0, int y0, int x1, int y1, TGAImage &image,
               TGAColor color) {
  int dx = x1 - x0;
  int dy = y1 - y0;
  bool transpose = false;

  // TODO: If y is steeper than x, swap the values around
  // Since the loop scales from the x axis, we want to swap
  // x and y so it will actually scale by y axis. This will avoid holes
  if (std::abs(dy) > std::abs(dx)) {
    std::swap(x0, y0);
    std::swap(x1, y1);
    transpose = true;
  }

  // This will normally make the for loop not run at all
  // So just swap them
  if (x1 < x0) {
    std::swap(x1, x0);
    std::swap(y1, y0);
  }

  // Reupdate deltas after switch
  dx = x1 - x0;
  dy = y1 - y0;

  // Iterate for each point between the two points (Using x-axis to count)
  for (int i = x0; i <= x1; i++) {
    // Make a scale from 0 to 1 and apply to dx and dy
    // This tells us how far in each direction to plot the next point
    float scale = ((float)(i - x0) / (x1 - x0));
    int new_x = x0 + dx * scale;
    int new_y = y0 + dy * scale;
    if (transpose) {
      image.set(new_y, new_x, color); // de-swap
    } else {
      image.set(new_x, new_y, color);
    }
  }
}

int main(int argc, char **argv) {
  int width = 700;
  int height = 700;
  TGAImage image(width, height, TGAImage::RGB);
  Model* m;
  if (argc == 1) {
    m = new Model("obj/african_head.obj");
  } else {
    m = new Model(argv[1]);
  }

  for (int i = 0; i < m->numFaces(); i++) {
    std::vector<int> face = m->getFace(i);
    for (int j = 0; j < 3; j++) {
      Vec3<float> v0 = m->getVertex(face[j]);
      Vec3<float> v1 = m->getVertex(face[(j + 1) % 3]);

      // The coordinates go from -1 to 1. Change that to 0 to 2 then scale
      // accordingly
      int x0 = (v0.x + 1.) * width / 2.;
      int y0 = (v0.y + 1.) * height / 2.;
      int x1 = (v1.x + 1.) * width / 2.;
      int y1 = (v1.y + 1.) * height / 2.;
      draw_line(x0, y0, x1, y1, image, white);
    }
  }
  image.flip_vertically();
  image.write_tga_file("output.tga");
  return 0;
}
