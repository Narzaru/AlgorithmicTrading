#ifndef PARALLELS_SLE_MODEL_ABSTRACT_SOLVER_H_
#define PARALLELS_SLE_MODEL_ABSTRACT_SOLVER_H_

#include <chrono>

#include "sle.h"

namespace s21 {

class AbstractSolver {
 public:
  using Solution = typename SLE::DataType::DataType;
  using ExecutionTime = std::size_t;
  using SizeType = typename SLE::SizeType;
  using ValueType = typename SLE::ValueType;

  struct TimedSolution {
    ExecutionTime time;
    Solution solution;

    TimedSolution();
    TimedSolution(ExecutionTime&& time, Solution&& solution);
  };

  virtual Solution Solve(SLE& sle) = 0;
  TimedSolution SolveWithTime(const SLE& sle);
};

}  // namespace s21

#endif  // PARALLELS_SLE_MODEL_ABSTRACT_SOLVER_H_
