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
    int new_y = y0 + dy * scale;
    if (transpose) {
      image.set(new_y, i, color); // de-swap
    } else {
      image.set(i, new_y, color);
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
  // draw_line(v0, v1, image, color);
  // draw_line(v1, v2, image, color);
  // draw_line(v2, v0, image, color);

  // from v0 to v1, draw on v0-v1 line from v2-v0 line
  // from v1 to v2, draw on v1-v2 line from v2-v0 line
  for (int y = v0.y; y <= v2.y; y++) {
    Vec2<int> d_left = (v2 - v0);
    Vec2<int> d_right = y > v1.y ? (v1 - v2) : (v0 - v1);
    float scale_left = ((float)(y - v0.y) / (d_left.y == 0 ? 1 : d_left.y));
    float scale_right = ((float)(y - (y > v1.y ? v1.y : v0.y)) / (d_right.y == 0 ? 1 : d_right.y));
    int x_left = v0.x + d_left.x * scale_left;
    int x_right = (y > v1.y ? v1.x : v0.x) + d_right.x * scale_right;

    if (std::abs(x_left - x_right) > 0) { // fix random dot being drawn when distance = 0
      Vec2<int> left_bound = Vec2<int>(x_left, y);
      Vec2<int> right_bound = Vec2<int>(x_right, y);
      draw_line(left_bound, right_bound, image, color);
    }
  }
}

int main(int argc, char **argv) {
  int width = 1000;
  int height = 1000;
  TGAImage image(width, height, TGAImage::RGB);
  Model *m;
  if (argc == 1) {
    m = new Model("obj/african_head.obj");
  } else {
    m = new Model(argv[1]);
  }

  // Shine a light from in front in world coordinates
  Vec3<float> light = Vec3<float>(0.0, 0.0, -1.0);

  for (int i = 0; i < m->numFaces(); i++) {
    std::vector<int> face = m->getFace(i);
    Vec2<int> screenVertices[3];
    Vec3<float> worldVertices[3];
    for (int j = 0; j < 3; j++) {
      Vec3<float> v0 = m->getVertex(face[j]);
      int x = (v0.x) * width;
      int y = (v0.y) * height;
      worldVertices[j] = v0;
      screenVertices[j] = (Vec2<int>(x, y));
    }
    // Find the normal of the face, and find dot product between
    // the normal and the light direction. This will give us
    // light intensity. This is due to the fact that light intensity
    // approaches 0 as the light becomes orthogonal to a surface, same
    // as dot product.
    Vec3<float> side1 = (worldVertices[2] - worldVertices[0]);
    Vec3<float> side2 = (worldVertices[1] - worldVertices[0]);
    Vec3<float> faceNormal = side1 ^ side2;
    faceNormal = faceNormal.normalize();
    float n = faceNormal * light;
    if (n > 0) { // n < 0 on objects behind, so dont render
      draw_triangle(screenVertices[0], screenVertices[1], screenVertices[2], image, TGAColor(255*n, 255*n, 255*n, 255));
    }
  }
  image.flip_vertically();
  image.write_tga_file("output.tga");
  return 0;
}
