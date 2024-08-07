#ifndef MATH_NODE_SQUARE_ROOT_H
#define MATH_NODE_SQUARE_ROOT_H

/* ---------------------------- Plugin Flow Document ------------------------ */
#include <flow/document/flow/node.h>
/* ----------------------------------- Local -------------------------------- */
#include "flow/math_node/export.h"
/* -------------------------------------------------------------------------- */

namespace flow {

class LIB_FLOW_MATH_NODE_API MathNodeSquareRoot : public NodeWithOneParam {
  FLOW_OBJECT_CLASS(MathNodeSquareRoot, NodeWithOneParam)

 public:
  explicit MathNodeSquareRoot();
  ~MathNodeSquareRoot() override;

  [[nodiscard]] std::unique_ptr<Node> clone() const override;

 protected:
  void compute() override;
};

}  // namespace flow

#endif  // MATH_NODE_SQUARE_ROOT_H
