//
// Copyright (c) 2017 XiaoMi All rights reserved.
//

#ifndef MACE_OPS_MATMUL_H_
#define MACE_OPS_MATMUL_H_

#include "mace/core/operator.h"
#include "mace/kernels/matmul.h"

namespace mace {

template <DeviceType D, class T>
class MatMulOp : public Operator<D, T> {
 public:
  MatMulOp(const OperatorDef &operator_def, Workspace *ws)
      : Operator<D, T>(operator_def, ws) {}

  bool Run(StatsFuture *future) override {
    const Tensor *A = this->Input(0);
    const Tensor *B = this->Input(1);
    Tensor *C = this->Output(0);
    MACE_CHECK(A->dim_size() == 4 && 4 == B->dim_size())
        << "The dimension of A and B should be 4";
    MACE_CHECK(A->dim(0) == B->dim(0)) << "A and B must have same batch size";
    MACE_CHECK(A->dim(2) == B->dim(1))
      << "the number of A's column " << A->dim(2)
      << " must be equal to B's row " << B->dim(1);

    functor_(A, B, C, future);
    return true;
  }

 private:
  kernels::MatMulFunctor<D, T> functor_;
};

}  // namespace mace

#endif  // MACE_OPS_MATMUL_H_