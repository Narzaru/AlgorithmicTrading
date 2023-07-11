#ifndef MATH_UTILS_INTERVAL_H_
#define MATH_UTILS_INTERVAL_H_

#include <optional>
#include <vector>

#include "point.h"

class Intervals {
 public:
  explicit Intervals(const std::vector<Point> &points) noexcept;

  int GetInterval(double value, double epsilon = 1e-9) const noexcept;

 private:
  const std::vector<Point> &points_;
};

#endif  // MATH_UTILS_INTERVAL_H_
