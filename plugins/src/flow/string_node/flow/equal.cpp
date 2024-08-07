/* ----------------------------------- Local -------------------------------- */
#include "flow/string_node/flow/equal.h"
/* -------------------------------------------------------------------------- */

/* ----------------------------------- Qtils -------------------------------- */

namespace {

enum PinIn { Lhs = 0, Rhs = 1 };
enum PinOut { Result = 0 };

}  // namespace

namespace flow {

/* ------------------------------- StringNodeEqual -------------------------- */

StringNodeEqual::StringNodeEqual() { setName(QObject::tr("EQUAL")); }

StringNodeEqual::~StringNodeEqual() = default;

std::unique_ptr<Node> StringNodeEqual::clone() const {
  auto node = std::make_unique<StringNodeEqual>();
  node->init(this);
  return std::move(node);
}

void StringNodeEqual::compute() {
  auto &lhs_pin = getPin(Pin::Type::In, PinIn::Lhs);
  auto &rhs_pin = getPin(Pin::Type::In, PinIn::Rhs);
  auto &result_pin = getPin(Pin::Type::Out, PinOut::Result);

  const auto lhs = lhs_pin.getData().toString();
  const auto rhs = rhs_pin.getData().toString();

  result_pin.setData(lhs == rhs);
}

}  // namespace flow