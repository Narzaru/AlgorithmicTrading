#ifndef PARALLELS_SLE_MODEL_PARALLEL_SOLVER_H_
#define PARALLELS_SLE_MODEL_PARALLEL_SOLVER_H_

#include <future>
#include <thread>
#include <vector>

#include "abstract_solver.h"

namespace s21 {

class ParallelSolver : public AbstractSolver {
 public:
  using ThreadPool = std::vector<std::thread>;

  ParallelSolver(int thread_count = std::thread::hardware_concurrency() / 2);

  Solution Solve(SLE& sle) override;

  ThreadPool::size_type GetThreadCount() const noexcept;

 private:
  void ForwardMove(SLE& sle);
  void BackwardMove(SLE& sle);

  void ParallelizeCalculations(SLE& sle, SizeType start, SizeType end,
                               SizeType ref_row);

  void CalculateRow(SLE& sle, SizeType row, SizeType ref_row);
  void CalculatePart(SLE& sle, SizeType ref_row, SizeType lower_bound,
                     SizeType upper_bound);

  ThreadPool threads_;
};

}  // namespace s21

#endif  // PARALLELS_SLE_MODEL_PARALLEL_SOLVER_H_
