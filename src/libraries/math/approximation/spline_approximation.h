#ifndef MATH_APPROXIMATION_SPLINE_APPROXIMATION_
#define MATH_APPROXIMATION_SPLINE_APPROXIMATION_

#include <cmath>
#include <vector>

#include "utils/point.h"
#include "utils/points.h"

namespace s21 {
class SplineApproximation final {
 public:
  using SizeType = std::size_t;

  SplineApproximation& FromPoints(const Points& points,
                                  SizeType degree) noexcept;

  double ApproximateAt(double x) const noexcept;

  Points UniformApproximationByPoints(SizeType number_of_points,
                                      double extra_x) const noexcept;

 private:
  class Spline final {
   public:
    Spline() = default;

    explicit Spline(std::vector<double> coefficients) noexcept
        : coefficients_(std::move(coefficients)) {}

    double GetValue(double x) const noexcept {
      double y = coefficients_[0];
      SizeType degree = coefficients_.size();
      for (SizeType i = 1; i < degree; ++i) {
        y += coefficients_[i] * std::pow(x, i);
      }

      return y;
    }

   private:
    std::vector<double> coefficients_;
  };

  Spline spline_;
  double min_x_{0.0};
  double max_x_{0.0};
};
}  // namespace s21

#endif  // MATH_APPROXIMATION_SPLINE_APPROXIMATION_
