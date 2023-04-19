#ifndef PLUGIN_FLOW_LOGIC_NODE_OR_H
#define PLUGIN_FLOW_LOGIC_NODE_OR_H

/* ----------------------------------- Local -------------------------------- */
#include "flow/plugins/node/node/logic/export.h"
#include "flow/plugins/node/node/logic/logic_node.h"
/* -------------------------------------------------------------------------- */

class LOGIC_NODE_API LogicNodeOr : public LogicNodeWithTwoParams
{
public:
  explicit LogicNodeOr();
  ~LogicNodeOr() override;

protected:
  void compute() override;
};

#endif//PLUGIN_FLOW_LOGIC_NODE_OR_H
