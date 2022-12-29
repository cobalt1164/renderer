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

bool barycentric(Vec2<int> a, Vec2<int> b, Vec2<int> c, Vec2<int> p) {
  Vec3<float> A(a.x, a.y, 0);
  Vec3<float> B(b.x, b.y, 0);
  Vec3<float> C(c.x, c.y, 0);
  Vec3<float> P(p.x, p.y, 0);

  // Use Cramer's rule to solve 
  // vA + wB = C
  // This method is from the book "Realtime collision Detection"
  Vec3<float> v0 = B - A;
  Vec3<float> v1 = C - A;
  Vec3<float> v2 = P - A;

  float d00 = v0 * v0;
  float d01 = v0 * v1;
  float d11 = v1 * v1;
  float d20 = v2 * v0;
  float d21 = v2 * v1;
  float denom = d00 * d11 - d01 * d01;
  float v = (d11 * d20 - d01 * d21) / denom;
  float w = (d00 * d21 - d01 * d20) / denom;
  float u = 1.0 - v - w;
  return v >= 0.0 && w >= 0.0 && u >= 0.0;
}

void draw_triangle(Vec2<int> v0, Vec2<int> v1, Vec2<int> v2, TGAImage &image, TGAColor color) {
  // Find bounding box of the triangle
  // Then iterate through every pixel in the bounding box and draw pixels that are in the triangle
  // We know if the pixel is in the triangle because it will have all non-negative barycentric coordinates.

  // find toppest, bottomest, and rightest, and leftest
  if (v0.y > v1.y) std::swap(v0, v1);
  if (v1.y > v2.y) std::swap(v1, v2);
  if (v0.y > v1.y) std::swap(v0, v1);
  int topOfBox = v2.y;
  int bottomOfBox = v0.y;

  if (v0.x > v1.x) std::swap(v0, v1);
  if (v1.x > v2.x) std::swap(v1, v2);
  if (v0.x > v1.x) std::swap(v0, v1);
  int leftOfBox = v0.x;
  int rightOfBox = v2.x;

  for (int i = leftOfBox; i <= rightOfBox; i++) {
    for (int j = bottomOfBox; j <= topOfBox; j++) {
      if (barycentric(v0, v1, v2, Vec2<int>(i, j)))
        image.set(i, j, color);
    }
  }
}

// Old linesweep method
// void draw_triangle(Vec2<int> v0, Vec2<int> v1, Vec2<int> v2, TGAImage &image,
//                    TGAColor color) {
//   // Sort vertices by y-axis
//   // Then rasterize based on upper/lower part
//   if (v0.y > v1.y) std::swap(v0, v1);
//   if (v1.y > v2.y) std::swap(v1, v2);
//   if (v0.y > v1.y) std::swap(v0, v1);
//
//
//   // from v0 to v1, draw on v0-v1 line from v2-v0 line
//   // from v1 to v2, draw on v1-v2 line from v2-v0 line
//   for (int y = v0.y; y <= v2.y; y++) {
//     Vec2<int> d_left = (v2 - v0);
//     Vec2<int> d_right = y > v1.y ? (v1 - v2) : (v0 - v1);
//     float scale_left = ((float)(y - v0.y) / (d_left.y == 0 ? 1 : d_left.y));
//     float scale_right = ((float)(y - (y > v1.y ? v1.y : v0.y)) / (d_right.y == 0 ? 1 : d_right.y));
//     int x_left = v0.x + d_left.x * scale_left;
//     int x_right = (y > v1.y ? v1.x : v0.x) + d_right.x * scale_right;
//
//     if (std::abs(x_left - x_right) > 0) { // fix random dot being drawn when distance = 0
//       Vec2<int> left_bound = Vec2<int>(x_left, y);
//       Vec2<int> right_bound = Vec2<int>(x_right, y);
//       draw_line(left_bound, right_bound, image, color);
//     }
//   }
// }

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

  // Vec2<int> t0[3] = {Vec2<int>(10, 70),   Vec2<int>(60, 70),  Vec2<int>(60, 140)}; 
  // Vec2<int> t1[3] = {Vec2<int>(60, 70),  Vec2<int>(110, 70),   Vec2<int>(60, 140)}; 
  // draw_triangle(t1[0], t1[1], t1[2], image, white);
  // draw_triangle(t0[0], t0[1], t0[2], image, green);

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
