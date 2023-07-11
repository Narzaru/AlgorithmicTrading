#include "abstract_solver.h"

namespace s21 {

AbstractSolver::TimedSolution::TimedSolution() : time(), solution() {}

AbstractSolver::TimedSolution::TimedSolution(ExecutionTime&& t, Solution&& s)
    : time(std::move(t)), solution(std::move(s)) {}

AbstractSolver::TimedSolution AbstractSolver::SolveWithTime(const SLE& sle) {
  SLE copy(sle);
  auto start = std::chrono::high_resolution_clock::now();
  Solution solution = Solve(copy);
  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  return TimedSolution(duration.count(), std::move(solution));
}

}  // namespace s21
