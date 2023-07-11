#include "solver.h"

namespace s21 {

Solver::Solution Solver::Solve(SLE& sle) {
  ForwardMove(sle);
  BackwardMove(sle);

  Solution solution;
  solution.reserve(sle.GetRows());
  for (SizeType i = 0; i < sle.GetRows(); ++i)
    solution.push_back(sle(i, sle.GetRows()));

  return solution;
}

void Solver::ForwardMove(SLE& sle) {
  for (SizeType i = 0; i < sle.GetRows(); ++i) {
    for (SizeType j = i + 1; j < sle.GetRows(); ++j) {
      ValueType coeff = sle(j, i) / sle(i, i);
      for (SizeType k = 0; k < sle.GetColumns(); ++k)
        sle(j, k) -= sle(i, k) * coeff;
    }
  }
}

void Solver::BackwardMove(SLE& sle) {
  for (SizeType i = sle.GetRows(); i > 0; --i) {
    for (SizeType j = i - 1; j > 0; --j) {
      ValueType coeff = sle(j - 1, i - 1) / sle(i - 1, i - 1);
      for (SizeType k = sle.GetRows() + 1; k > 0; --k)
        sle(j - 1, k - 1) -= sle(i - 1, k - 1) * coeff;
    }

    sle(i - 1, sle.GetRows()) /= sle(i - 1, i - 1);
  }
}

}  // namespace s21
