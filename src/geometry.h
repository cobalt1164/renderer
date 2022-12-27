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
};
