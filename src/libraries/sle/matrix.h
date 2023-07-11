#ifndef PARALLELS_SLE_MODEL_MATRIX_H_
#define PARALLELS_SLE_MODEL_MATRIX_H_

#include <stdexcept>
#include <vector>

namespace s21 {

class Matrix {
 public:
  using ValueType = double;
  using DataType = std::vector<ValueType>;
  using SizeType = typename DataType::size_type;
  using Reference = typename DataType::reference;
  using ConstReference = typename DataType::const_reference;

  Matrix();
  Matrix(SizeType rows, SizeType columns, ConstReference value = ValueType());

  Reference operator()(SizeType row, SizeType column);
  ConstReference operator()(SizeType row, SizeType column) const;

  SizeType GetRows() const noexcept { return rows_; }
  SizeType GetColumns() const noexcept { return columns_; }

 private:
  SizeType rows_, columns_;
  DataType data_;
};

}  // namespace s21

#endif  // PARALLELS_SLE_MODEL_MATRIX_H_
