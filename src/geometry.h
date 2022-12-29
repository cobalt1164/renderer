#include <cmath>
#include <iostream>


template <class T> struct Vec3 {
  T x, y, z;
  Vec3() {
    x = 0;
    y = 0;
    z = 0;
  }
  Vec3(T _x, T _y, T _z) {
    x = _x;
    y = _y;
    z = _z;
  };
  inline Vec3<T> operator+(Vec3<T> b) {
      return Vec3<T> (x + b.x, y + b.y, z + b.z);
  }
  inline Vec3<T> operator-(Vec3<T> b) {
    return Vec3<T> (x - b.x, y - b.y, z - b.z);
  }
  inline T operator*(Vec3<T> b) {
    return x * b.x + y * b.y + z * b.z;
  };
  inline Vec3<T> operator*(T b) {
    return Vec3<T>(x * b, y * b, z * b);
  }
  inline Vec3<T> operator[](int i) {
    return (i == 0 ? x : (i == 1 ? y : z));
  }
  inline Vec3<T> operator^(Vec3<T> b) {
    return Vec3<T>(y * b.z - z * b.y,
        z * b.x - x * b.z, x * b.y - y * b.x);
  }; // cross product
  float length() { return std::sqrt(x*x+y*y+z*z); }
  Vec3<T> normalize() {
    return *this * (1/length());
  };
};

template <class T> struct Vec2 {
  T x, y;
  Vec2() {
    x = 0;
    y = 0;
  }
  Vec2(T _x, T _y) {
    x = _x;
    y = _y;
  };

  inline Vec2<T> operator+(Vec2<T> o) { return Vec2<T>(x + o.x, y + o.y); };
  inline Vec2<T> operator-(Vec2<T> o) { return Vec2<T>(x - o.x, y - o.y); };
  inline Vec2<T> operator*(float o) { return Vec2<T>(x * o, y * o); };
};
