#ifndef MATH_UTILS_POINT_H_
#define MATH_UTILS_POINT_H_

struct Point {
  Point() : Point(0.0, 0.0) {}
  Point(double x_value, double y_value) : x{x_value}, y{y_value} {}
  Point(const Point &point) = default;
  Point(Point &&point) = default;
  Point &operator=(const Point &) = default;
  Point &operator=(Point &&) = default;

  double x;
  double y;
};

#endif  // MATH_UTILS_POINT_H_
