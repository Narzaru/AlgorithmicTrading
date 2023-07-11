#ifndef MATH_UTILS_POINTS_H_
#define MATH_UTILS_POINTS_H_

#include <algorithm>
#include <functional>
#include <limits>
#include <vector>

#include "point.h"

class Points {
 public:
  using Vector = std::vector<Point>;
  using SizeType = Vector::size_type;
  using Iterator = Vector::iterator;
  using ConstIterator = Vector::const_iterator;
  using Reference = Vector::reference;
  using ConstReference = Vector::const_reference;
  using DoubleLimits = std::numeric_limits<double>;

  Points &Append(Point point) {
    points_.push_back(point);
    return *this;
  }

  Points &operator+=(Point point) {
    points_.push_back(point);
    return *this;
  }

  Points &operator<<(Point point) {
    points_.push_back(point);
    return *this;
  }

  const Point &operator[](SizeType index) const { return points_[index]; }

  Point &operator[](SizeType index) { return points_[index]; }

  struct XAxis {};
  struct YAxis {};

  template <typename axis>
  std::pair<double, double> MinMax() const {
    if constexpr (std::is_same<axis, XAxis>::value) {
      auto &&[min, max] =
          std::minmax_element(points_.begin(), points_.end(), x_comparator);
      return {min->x, max->x};
    } else if constexpr (std::is_same<axis, YAxis>::value) {
      auto &&[min, max] =
          std::minmax_element(points_.begin(), points_.end(), y_comparator);
      return {min->y, max->y};
    } else {
      return {DoubleLimits::quiet_NaN(), DoubleLimits::quiet_NaN()};
    }
  }

  const Vector &GetRawPoints() const noexcept { return points_; }

  Reference Front() noexcept { return points_.front(); }

  Reference Back() noexcept { return points_.back(); }

  ConstReference Front() const noexcept { return points_.front(); }

  ConstReference Back() const noexcept { return points_.back(); }

  Vector::size_type Size() const noexcept { return points_.size(); }

  Iterator begin() noexcept { return points_.begin(); }

  Iterator end() noexcept { return points_.end(); }

  ConstIterator begin() const noexcept { return points_.cbegin(); }

  ConstIterator end() const noexcept { return points_.cend(); }

  ConstIterator cbegin() const noexcept { return points_.cbegin(); }

  ConstIterator cend() const noexcept { return points_.cend(); }

  void Clear() noexcept { points_.clear(); }

  bool Empty() const noexcept { return points_.empty(); }

 private:
  Vector points_;

  std::function<bool(const Point &, const Point &)> x_comparator =
      [](const Point &a, const Point &b) { return a.x < b.x; };

  std::function<bool(const Point &, const Point &)> y_comparator =
      [](const Point &a, const Point &b) { return a.y < b.y; };
};

#endif  // MATH_UTILS_POINTS_H_
