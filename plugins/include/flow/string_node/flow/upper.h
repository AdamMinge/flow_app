#ifndef STRING_NODE_UPPER_H
#define STRING_NODE_UPPER_H

/* ---------------------------- Plugin Flow Document ------------------------ */
#include <flow/document/flow/node.h>
/* ----------------------------------- Local -------------------------------- */
#include "flow/string_node/export.h"
/* -------------------------------------------------------------------------- */

namespace flow {

class LIB_FLOW_STRING_NODE_API StringNodeUpper : public NodeWithOneParam {
  FLOW_OBJECT_CLASS(StringNodeUpper, NodeWithOneParam)

 public:
  explicit StringNodeUpper();
  ~StringNodeUpper() override;

  [[nodiscard]] std::unique_ptr<Node> clone() const override;

 protected:
  void compute() override;
};

}  // namespace flow

#endif  // STRING_NODE_UPPER_H
