#include "spline_interpolation.h"

#include "utils/interval.h"

namespace s21 {
SplineInterpolation& SplineInterpolation::SetPoints(Points points) noexcept {
  points_ = std::move(points);
  FindSplineCoefficients();
  return *this;
}

double SplineInterpolation::InterpolateAt(double x) const noexcept {
  Intervals intervals{points_.GetRawPoints()};
  auto interval_number = intervals.GetInterval(x);

  auto [a, b, c, d] = coefficients_[interval_number];
  double dx = x - points_[interval_number].x;

  return a + b * dx + c * dx * dx + d * dx * dx * dx;
}

Points SplineInterpolation::UniformInterpolationByPoints(
    int number_of_points) const noexcept {
  Points interpolated_points;
  auto [min_x, max_x] = points_.MinMax<Points::XAxis>();
  double step = (max_x - min_x) / (number_of_points - 1);

  for (int i = 0; i < number_of_points; ++i) {
    double x = min_x + i * step;
    double y = InterpolateAt(x);
    interpolated_points += {x, y};
  }

  return interpolated_points;
}

void SplineInterpolation::FindSplineCoefficients() {
  using size_t = Points::SizeType;

  auto n = points_.Size();
  std::vector<double> alpha(n);
  std::vector<double> beta(n);
  std::vector<double> h(n - 1);
  std::vector<double> u(n);
  std::vector<double> v(n);

  for (size_t i = 0; i < n - 1; ++i) h[i] = points_[i + 1].x - points_[i].x;

  alpha[0] = 0.0;
  beta[0] = 0.0;

  for (size_t i = 1; i < n - 1; ++i)
    alpha[i] = 3.0 / h[i] * (points_[i + 1].y - points_[i].y) -
               3.0 / h[i - 1] * (points_[i].y - points_[i - 1].y);

  u[0] = 1.0;
  v[0] = 0.0;

  for (size_t i = 1; i < n - 1; ++i) {
    u[i] = 2.0 * (h[i - 1] + h[i]) - h[i - 1] * h[i - 1] / u[i - 1];
    v[i] = alpha[i] - h[i - 1] * v[i - 1] / u[i - 1];
  }

  u[n - 1] = 1.0;
  v[n - 1] = 0.0;

  std::vector<double> z(n + 1, 0.0);
  coefficients_.resize(n + 1);

  for (int i = static_cast<int>(n) - 2; i >= 0; --i)
    z[i] = (v[i] - h[i] * z[i + 1]) / u[i];

  for (size_t i = 0; i < n - 1; ++i) {
    coefficients_[i].a = points_[i].y;
    coefficients_[i].b = (points_[i + 1].y - points_[i].y) / h[i] -
                         h[i] * (z[i + 1] + 2.0 * z[i]) / 3.0;
    coefficients_[i].c = z[i];
    coefficients_[i].d = (z[i + 1] - z[i]) / (3.0 * h[i]);
  }

  coefficients_[n - 1].a = points_[n - 1].y;
  coefficients_[n - 1].b = 0.0;
  coefficients_[n - 1].c = z[n - 1];
  coefficients_[n - 1].d = 0.0;
}
}  // namespace s21
