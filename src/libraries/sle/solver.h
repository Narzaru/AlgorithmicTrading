#ifndef PARALLELS_SLE_MODEL_SOLVER_H_
#define PARALLELS_SLE_MODEL_SOLVER_H_

#include "abstract_solver.h"

namespace s21 {

class Solver : public AbstractSolver {
 public:
  Solution Solve(SLE& sle) override;

 private:
  void ForwardMove(SLE& sle);
  void BackwardMove(SLE& sle);
};

}  // namespace s21

#endif  // PARALLELS_SLE_MODEL_SOLVER_H_
