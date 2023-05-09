#ifndef LOGIC_NODE_NOT_H
#define LOGIC_NODE_NOT_H

/* ----------------------------------- Local -------------------------------- */
#include "export.h"
#include "logic_node.h"
/* -------------------------------------------------------------------------- */

class LOGIC_NODE_API LogicNodeNot : public LogicNodeWithOneParam {
 public:
  explicit LogicNodeNot();
  ~LogicNodeNot() override;

 protected:
  void compute() override;
};

#endif  // LOGIC_NODE_NOT_H