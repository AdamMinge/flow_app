#ifndef PLUGIN_FLOW_LOGIC_NODE_AND_H
#define PLUGIN_FLOW_LOGIC_NODE_AND_H

/* ----------------------------------- Local -------------------------------- */
#include "flow/plugins/node/node/logic/export.h"
#include "flow/plugins/node/node/logic/logic_node.h"
/* -------------------------------------------------------------------------- */

class LOGIC_NODE_API LogicNodeAnd : public LogicNodeWithTwoParams
{
public:
  explicit LogicNodeAnd();
  ~LogicNodeAnd() override;

protected:
  void compute() override;
};

#endif//PLUGIN_FLOW_LOGIC_NODE_AND_H
