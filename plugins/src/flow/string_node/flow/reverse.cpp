/* ----------------------------------- Local -------------------------------- */
#include "flow/string_node/flow/reverse.h"
/* -------------------------------------------------------------------------- */

/* ----------------------------------- Qtils -------------------------------- */

namespace {

enum PinIn { Value = 0 };
enum PinOut { Result = 0 };

}  // namespace

namespace flow {

/* ------------------------------ StringNodeReverse ------------------------- */

StringNodeReverse::StringNodeReverse() { setName(QObject::tr("REVERSE")); }

StringNodeReverse::~StringNodeReverse() = default;

std::unique_ptr<Node> StringNodeReverse::clone() const {
  auto node = std::make_unique<StringNodeReverse>();
  node->init(this);
  return std::move(node);
}

void StringNodeReverse::compute() {
  const auto &value_pin = getPin(Pin::Type::In, PinIn::Value);
  auto &result_pin = getPin(Pin::Type::Out, PinOut::Result);

  const auto value = value_pin.getData().toString();

  auto reversed = QString{};
  for (int i = value.length() - 1; i >= 0; --i) {
    reversed += value[i];
  }

  result_pin.setData(reversed);
}

}  // namespace flow