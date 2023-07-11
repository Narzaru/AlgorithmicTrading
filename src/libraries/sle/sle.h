#ifndef PARALLELS_SLE_MODEL_SLE_H_
#define PARALLELS_SLE_MODEL_SLE_H_

#include "matrix.h"

namespace s21 {

class SLE {
 public:
  using DataType = Matrix;
  using ValueType = typename DataType::ValueType;
  using SizeType = typename DataType::SizeType;
  using Reference = typename DataType::Reference;
  using ConstReference = typename DataType::ConstReference;
  using Solution = typename DataType::DataType;

  SLE();
  explicit SLE(SizeType size);
  explicit SLE(const DataType& matrix);

  Reference operator()(SizeType row, SizeType column);
  ConstReference operator()(SizeType row, SizeType column) const;

  SizeType GetRows() const noexcept;
  SizeType GetColumns() const noexcept;

 private:
  DataType data_;
};

}  // namespace s21

#endif  // PARALLELS_SLE_MODEL_SLE_H_
