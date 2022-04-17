#ifndef FLOW_LOGIC_NODE_AND_H
#define FLOW_LOGIC_NODE_AND_H

/* ----------------------------------- Local -------------------------------- */
#include "export.h"
#include "logic_node.h"
/* -------------------------------------------------------------------------- */

class LOGIC_NODE_API LogicNodeAnd : public LogicNodeWithTwoParams
{
  Q_OBJECT

public:
  explicit LogicNodeAnd();
  ~LogicNodeAnd() override;

protected:
  void compute() override;
};

#endif//FLOW_LOGIC_NODE_AND_H