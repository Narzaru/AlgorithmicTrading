#ifndef A8_ALGORITHMIC_TRADING_MODELS_FUNCTION_TABULATED_FUNCTION_H_
#define A8_ALGORITHMIC_TRADING_MODELS_FUNCTION_TABULATED_FUNCTION_H_

#include <memory>

#include "csv/file.h"
#include "forward_declarations.h"
#include "utils/points.h"

class TabulatedFunction {
 public:
  TabulatedFunction();
  ~TabulatedFunction();

  using SizeType = Points::SizeType;
  using FunctionType = const std::function<double(std::string_view)> &;

  bool LoadFromCSVFile(csv::File file, FunctionType x_fun, FunctionType y_fun);
  bool FromPoints(Points points);

  bool ArePointsCorrect() const noexcept;

  Points GetValues();

  Points GetInterpolatedValues(SizeType new_number_of_points);
  Points GetApproximatedValues(SizeType number_of_points, SizeType spline_power,
                               double extended_x);

  Point InterpolatedPointAt(double x);
  Point ApproximatePointAt(double x, int degree);

 private:
  bool ValidPoints();
  void SortPoints();
  void Normalize();
  Points Denormalize(const Points &points) const;

  struct NormalizationCoefficients {
    double min_x{};
    double max_x{};
    double min_y{};
    double max_y{};
  };

  Points points_;
  NormalizationCoefficients normalization_coefficients_;

  std::unique_ptr<SplineApproximationNative> spline_approximation_;
  std::unique_ptr<SplineInterpolationNative> spline_interpolation_;
};

#endif  // A8_ALGORITHMIC_TRADING_MODELS_FUNCTION_TABULATED_FUNCTION_H_
