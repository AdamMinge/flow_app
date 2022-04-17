/* ----------------------------------- Local -------------------------------- */
#include "flow/plugins/node/node/logic/logic_node_not.h"
/* ------------------------------------ Node -------------------------------- */
#include "flow/modules/node/base/boolean_node_data.h"
/* -------------------------------------------------------------------------- */

LogicNodeNot::LogicNodeNot() = default;

LogicNodeNot::~LogicNodeNot() = default;

void LogicNodeNot::compute()
{
  auto &in_pin_0 = getPin(::node::Pin::Type::In, 0);
  auto &out_pin = getPin(::node::Pin::Type::Out, 0);

  auto value_A =
    dynamic_cast<const node::base::BooleanNodeData &>(in_pin_0.getData())
      .getValue();

  const auto out_data = node::base::BooleanNodeData(!value_A);
  out_pin.setData(out_data);
}