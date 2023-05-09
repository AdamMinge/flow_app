/* ----------------------------------- Local -------------------------------- */
#include "logic_node_or.h"
/* -------------------------------------------------------------------------- */

LogicNodeOr::LogicNodeOr() { setName(QObject::tr("OR")); }

LogicNodeOr::~LogicNodeOr() = default;

void LogicNodeOr::compute() {
  const auto &in_pin_0 = getPin(flow_document::Pin::Type::In, 0);
  const auto &in_pin_1 = getPin(flow_document::Pin::Type::In, 1);
  auto &out_pin = getPin(flow_document::Pin::Type::Out, 0);

  const auto value_A = in_pin_0.getData().toBool();
  const auto value_B = in_pin_1.getData().toBool();

  out_pin.setData(value_A | value_B);
}