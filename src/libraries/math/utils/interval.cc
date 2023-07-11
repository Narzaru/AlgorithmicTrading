#include "interval.h"

#include <cmath>

Intervals::Intervals(const std::vector<Point> &points) noexcept
    : points_(points) {}

bool DefinitelyGreaterThan(double a, double b, double epsilon) {
  return (a - b) >
         ((std::fabs(a) < std::fabs(b) ? std::fabs(b) : std::fabs(a)) *
          epsilon);
}

bool DefinitelyLessThan(double a, double b, double epsilon) {
  return (b - a) >
         ((std::fabs(a) < std::fabs(b) ? std::fabs(b) : std::fabs(a)) *
          epsilon);
}

int Intervals::GetInterval(double value, double epsilon) const noexcept {
  int index_of_interval{0};
  if (DefinitelyLessThan(value, points_.front().x, epsilon)) {
    // past end interval
    return -1;
  } else if (DefinitelyGreaterThan(value, points_.back().x, epsilon)) {
    // before start interval
    return static_cast<int>(points_.size()) - 1;
  } else {
    for (int i = 0; i < static_cast<int>(points_.size()) - 1; ++i) {
      if (value >= points_[i].x) {
        index_of_interval = i;
      }
    }

    return index_of_interval;
  }
}
