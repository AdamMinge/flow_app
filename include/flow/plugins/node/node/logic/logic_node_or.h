#ifndef FLOW_LOGIC_NODE_OR_H
#define FLOW_LOGIC_NODE_OR_H

/* ----------------------------------- Local -------------------------------- */
#include "export.h"
#include "logic_node.h"
/* -------------------------------------------------------------------------- */

class LOGIC_NODE_API LogicNodeOr : public LogicNodeWithTwoParams
{
  Q_OBJECT

public:
  explicit LogicNodeOr();
  ~LogicNodeOr() override;

protected:
  void compute() override;
};

#endif//FLOW_LOGIC_NODE_OR_H