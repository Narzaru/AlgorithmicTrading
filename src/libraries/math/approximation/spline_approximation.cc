#include "spline_approximation.h"

#include "sle/matrix.h"
#include "sle/solver.h"

namespace s21 {
SplineApproximation& SplineApproximation::FromPoints(const Points& points,
                                                     SizeType degree) noexcept {
  auto [min_x, max_x] = points.MinMax<Points::XAxis>();
  min_x_ = min_x;
  max_x_ = max_x;

  std::vector<double> x_sum(degree * 2 + 1);
  x_sum[0] = static_cast<double>(points.Size());
  for (unsigned long i = 1; i < degree * 2 + 1; ++i) {
    for (auto [x, y] : points) {
      x_sum[i] += std::pow(x, i);
    }
  }

  std::vector<double> y_sum(degree + 1);
  for (unsigned long i = 0; i < degree + 1; ++i) {
    for (auto [x, y] : points) {
      y_sum[i] += y * std::pow(x, i);
    }
  }

  // fill matrix of variables
  s21::Matrix sle_matrix(degree + 1, degree + 2, 0.0);
  for (unsigned long i = 0; i < degree + 1; ++i) {
    for (unsigned long j = 0; j < degree + 1; ++j) {
      sle_matrix(i, j) = x_sum[i + j];
    }
  }

  // fill row of answers
  for (unsigned long i = 0; i < degree + 1; ++i) {
    sle_matrix(i, degree + 1) = y_sum[i];
  }

  s21::SLE sle{sle_matrix};
  s21::Solver solver;
  auto res = solver.Solve(sle);

  spline_ = Spline(res);

  return *this;
}

double SplineApproximation::ApproximateAt(double x) const noexcept {
  return spline_.GetValue(x);
}

Points SplineApproximation::UniformApproximationByPoints(
    SizeType number_of_points, double extra_x) const noexcept {
  Points points;
  double step = (extra_x - min_x_) / static_cast<double>(number_of_points - 1);

  for (SizeType i = 0; i < number_of_points; ++i) {
    auto x{min_x_ + static_cast<double>(i) * step};
    points += {x, spline_.GetValue(x)};
  }

  return points;
}
}  // namespace s21
