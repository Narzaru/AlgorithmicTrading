#include "sle.h"

namespace s21 {

SLE::SLE() : SLE(1) {}
SLE::SLE(SizeType size) : data_(size, size + 1) {}
SLE::SLE(const DataType& matrix) : data_(matrix) {}

SLE::Reference SLE::operator()(SizeType row, SizeType column) {
  return data_(row, column);
}

SLE::ConstReference SLE::operator()(SizeType row, SizeType column) const {
  return data_(row, column);
}

SLE::SizeType SLE::GetRows() const noexcept { return data_.GetRows(); }
SLE::SizeType SLE::GetColumns() const noexcept { return data_.GetColumns(); }

}  // namespace s21
