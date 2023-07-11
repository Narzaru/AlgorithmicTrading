#include "tabulated_function.h"

#include "csv/reader.h"
#include "math/approximation/spline_approximation.h"
#include "math/interpolation/spline_interpolation.h"

TabulatedFunction::TabulatedFunction()
    : spline_approximation_(new s21::SplineApproximation),
      spline_interpolation_(new s21::SplineInterpolation) {}

TabulatedFunction::~TabulatedFunction() = default;

bool TabulatedFunction::LoadFromCSVFile(csv::File file, FunctionType x_fun,
                                        FunctionType y_fun) {
  points_.Clear();
  csv::Reader reader(std::move(file), true);

  while (!reader.EndOfFile()) {
    auto [x, y] = reader.ReadLineWithFunctors<2>(x_fun, y_fun);
    points_ += {x, y};
  }

  if (!ValidPoints()) {
    points_.Clear();
    return false;
  }
  SortPoints();
  Normalize();
  return true;
}

bool TabulatedFunction::FromPoints(Points points) {
  points_ = std::move(points);
  if (!ValidPoints()) {
    points_.Clear();
    return false;
  }
  SortPoints();
  Normalize();
  return true;
}

bool TabulatedFunction::ArePointsCorrect() const noexcept {
  return !points_.Empty();
}

Points TabulatedFunction::GetValues() { return Denormalize(points_); }

Points TabulatedFunction::GetInterpolatedValues(SizeType new_number_of_points) {
  return Denormalize(
      spline_interpolation_->SetPoints(points_).UniformInterpolationByPoints(
          static_cast<int>(new_number_of_points)));
}

Points TabulatedFunction::GetApproximatedValues(SizeType number_of_points,
                                                SizeType spline_power,
                                                double extended_x) {
  auto min_x = normalization_coefficients_.min_x;
  auto max_x = normalization_coefficients_.max_x;
  double end_x = (extended_x - min_x) / max_x;
  return Denormalize(
      spline_approximation_->FromPoints(points_, spline_power)
          .UniformApproximationByPoints(number_of_points, end_x));
}

void TabulatedFunction::SortPoints() {
  std::sort(points_.begin(), points_.end(),
            [](auto& a, auto& b) { return a.x < b.x; });
}

bool TabulatedFunction::ValidPoints() {
  auto IsFinitePoint = [](Point& point) {
    auto [x, y] = point;
    return std::isfinite(x) && std::isfinite(y);
  };
  return std::all_of(points_.begin(), points_.end(), IsFinitePoint);
}

void TabulatedFunction::Normalize() {
  auto [min_x, max_x] = points_.MinMax<Points::XAxis>();
  auto [min_y, max_y] = points_.MinMax<Points::YAxis>();

  normalization_coefficients_.min_x = min_x;
  normalization_coefficients_.min_y = min_y;
  normalization_coefficients_.max_x = max_x;
  normalization_coefficients_.max_y = max_y;

  for (auto& point : points_) {
    point.x -= min_x;
    point.x /= min_x;
    point.y -= min_y;
    point.y /= min_y;
  }
}

Points TabulatedFunction::Denormalize(const Points& points) const {
  Points denormalized_points;
  auto [min_x, max_x, min_y, max_y] = normalization_coefficients_;

  for (auto point : points) {
    double x = point.x * max_x + min_x;
    double y = point.y * max_y + min_y;
    denormalized_points += {x, y};
  }

  return denormalized_points;
}

Point TabulatedFunction::InterpolatedPointAt(double x) {
  spline_interpolation_->SetPoints(points_);

  auto [min_x, max_x, min_y, max_y] = normalization_coefficients_;
  double x_normalized = (x - min_x) / max_x;

  double y = spline_interpolation_->InterpolateAt(x_normalized);

  return {x, y * max_y + min_y};
}

Point TabulatedFunction::ApproximatePointAt(double x, int degree) {
  spline_approximation_->FromPoints(points_, degree);

  auto [min_x, max_x, min_y, max_y] = normalization_coefficients_;
  double x_normalized = (x - min_x) / max_x;

  double y = spline_approximation_->ApproximateAt(x_normalized);

  return {x, y * max_y + min_y};
}
