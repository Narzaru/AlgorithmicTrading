#ifndef MATH_INTERPOLATION_SPLINE_INTERPOLATION_H_
#define MATH_INTERPOLATION_SPLINE_INTERPOLATION_H_

#include <vector>

#include "utils/points.h"

namespace s21 {
class SplineInterpolation final {
 public:
  SplineInterpolation &SetPoints(Points points) noexcept;

  double InterpolateAt(double x) const noexcept;

  Points UniformInterpolationByPoints(int number_of_points) const noexcept;

 private:
  struct SplineCoefficient {
    double a;
    double b;
    double c;
    double d;
  };

  void FindSplineCoefficients();

  Points points_;
  std::vector<SplineCoefficient> coefficients_;
};
}  // namespace s21

#endif  // MATH_INTERPOLATION_SPLINE_INTERPOLATION_H_
