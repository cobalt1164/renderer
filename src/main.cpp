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

void draw_line(Vec2<int> v0, Vec2<int> v1, TGAImage &image, TGAColor color) {
  draw_line(v0.x, v0.y, v1.x, v1.y, image, color);
}

void draw_triangle(Vec2<int> v0, Vec2<int> v1, Vec2<int> v2, TGAImage &image,
                   TGAColor color) {
  // Sort vertices by y-axis
  // Then rasterize based on upper/lower part
  if (v0.y > v1.y) std::swap(v0, v1);
  if (v1.y > v2.y) std::swap(v1, v2);
  if (v0.y > v1.y) std::swap(v0, v1);

  // v0-v1, v1-v2, v2-v0
  // draw outline
  draw_line(v0, v1, image, color);
  draw_line(v1, v2, image, color);
  draw_line(v2, v0, image, color);

  // from v0 to v1, draw on v0-v1 line from v2-v0 line
  // (if v0 is on the left, otherwise go other direction)
  // for (int i = 0; i < (v1.y - v0.y); i++) {
  //   Vec2<int> left_bound = Vec2<int>();
  //   Vec2<int> right_bound = Vec2<int>();
  //   int left_dx = v2.x - v0.x;
  //   int left_dy = v2.y - v0.y;
  //   float scale = ((float)(i - v0.x) / (v2.x - v0.x));
  //   int new_x = v0.x + left_dx * scale;
  //   int new_y = v0.y + left_dy * scale;
  //   left_bound.x = new_x;
  //   left_bound.y = new_y;
  //   right_bound = left_bound;
  //   right_bound.x += 100;
  //   std::cout << left_bound.x << " " << left_bound.y << std::endl;
  //   draw_line(left_bound, right_bound, image, color);
  // }
}

int main(int argc, char **argv) {
  int width = 500;
  int height = 500;
  TGAImage image(width, height, TGAImage::RGB);
  Model *m;
  if (argc == 1) {
    m = new Model("obj/african_head.obj");
  } else {
    m = new Model(argv[1]);
  }
  Vec2<int> t0[3] = {Vec2<int>(10, 70), Vec2<int>(50, 160), Vec2<int>(70, 80)};
  Vec2<int> t1[3] = {Vec2<int>(180, 50), Vec2<int>(150, 1), Vec2<int>(70, 180)};
  Vec2<int> t2[3] = {Vec2<int>(180, 150), Vec2<int>(120, 160),
                     Vec2<int>(130, 180)};
  draw_triangle(t0[0], t0[1], t0[2], image, red);
  draw_triangle(t1[0], t1[1], t1[2], image, white);
  draw_triangle(t2[0], t2[1], t2[2], image, green);
  // for (int i = 0; i < m->numFaces(); i++) {
  //   std::vector<int> face = m->getFace(i);
  //   for (int j = 0; j < 3; j++) {
  //     Vec3<float> v0 = m->getVertex(face[j]);
  //     Vec3<float> v1 = m->getVertex(face[(j + 1) % 3]);
  //
  //     // The coordinates go from -1 to 1. Change that to 0 to 2 then scale
  //     // accordingly
  //     int x0 = (v0.x) * width;
  //     int y0 = (v0.y) * height;
  //     int x1 = (v1.x) * width;
  //     int y1 = (v1.y) * height;
  //     draw_line(x0, y0, x1, y1, image, white);
  //   }
  // }
  image.flip_vertically();
  image.write_tga_file("output.tga");
  return 0;
}
