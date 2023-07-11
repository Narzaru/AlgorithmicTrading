#include "parallel_solver.h"

namespace s21 {

ParallelSolver::ParallelSolver(int thread_count) : threads_() {
  threads_.reserve(thread_count > 0 ? thread_count
                                    : std::thread::hardware_concurrency());
}

ParallelSolver::Solution ParallelSolver::Solve(SLE& sle) {
  ForwardMove(sle);
  BackwardMove(sle);

  Solution solution;
  solution.reserve(sle.GetRows());
  for (SizeType i = 0; i < sle.GetRows(); ++i)
    solution.push_back(sle(i, sle.GetRows()) / sle(i, i));

  return solution;
}

typename ParallelSolver::ThreadPool::size_type ParallelSolver::GetThreadCount()
    const noexcept {
  return threads_.capacity();
}

void ParallelSolver::ForwardMove(SLE& sle) {
  for (SizeType i = 1; i < sle.GetRows(); ++i)
    ParallelizeCalculations(sle, i, sle.GetRows() - 1, i - 1);
}

void ParallelSolver::BackwardMove(SLE& sle) {
  for (SizeType i = sle.GetRows() - 1; i > 0; --i)
    ParallelizeCalculations(sle, 0, i - 1, i);
}

void ParallelSolver::ParallelizeCalculations(SLE& sle, SizeType start,
                                             SizeType end, SizeType ref_row) {
  SizeType rows_count = end - start + 1;

  SizeType current_thread_count = std::min(GetThreadCount(), rows_count);
  SizeType step = rows_count / current_thread_count;
  SizeType prev_upper_bound = start;

  for (SizeType j = 0; j < current_thread_count; ++j) {
    SizeType lower_bound = prev_upper_bound;
    SizeType upper_bound = std::min(lower_bound + step, end + 1);
    if (j + 1 == current_thread_count) upper_bound = end + 1;
    prev_upper_bound = upper_bound;

    threads_.emplace_back(&ParallelSolver::CalculatePart, this, std::ref(sle),
                          ref_row, lower_bound, upper_bound);
  }

  for (auto& thread : threads_) thread.join();
  threads_.clear();
}

void ParallelSolver::CalculateRow(SLE& sle, SizeType row, SizeType ref_row) {
  ValueType coeff = sle(row, ref_row) / sle(ref_row, ref_row);
  for (SizeType i = 0; i < sle.GetColumns(); ++i) {
    sle(row, i) -= sle(ref_row, i) * coeff;
  }
}

void ParallelSolver::CalculatePart(SLE& sle, SizeType ref_row,
                                   SizeType lower_bound, SizeType upper_bound) {
  while (lower_bound < upper_bound) {
    CalculateRow(sle, lower_bound, ref_row);
    ++lower_bound;
  }
}

}  // namespace s21
