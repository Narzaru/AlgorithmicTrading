#include "matrix.h"

namespace s21 {

Matrix::Matrix() : Matrix(3, 3) {}

Matrix::Matrix(SizeType rows, SizeType columns, ConstReference value)
    : rows_(rows), columns_(columns) {
  if (!rows_ || !columns_)
    throw std::invalid_argument("Matrix sizes can't be zeroed");
  data_.reserve(rows_ * columns_);
  for (SizeType i = 0; i < rows_ * columns_; ++i) data_.push_back(value);
}

Matrix::Reference Matrix::operator()(SizeType row, SizeType column) {
  return data_.at(row * columns_ + column);
}

Matrix::ConstReference Matrix::operator()(SizeType row, SizeType column) const {
  return data_.at(row * columns_ + column);
}

}  // namespace s21
