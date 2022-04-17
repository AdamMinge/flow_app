#ifndef FLOW_INTEGER_TO_STRING_CONVERTER_H
#define FLOW_INTEGER_TO_STRING_CONVERTER_H

/* ----------------------------------- Local -------------------------------- */
#include "flow/modules/node/export.h"
#include "flow/modules/node/type_converter.h"
/* -------------------------------------------------------------------------- */

class NODE_API IntegerToStringConverter : public node::TypeConverter
{
public:
  explicit IntegerToStringConverter() = default;
  ~IntegerToStringConverter() = default;

  [[nodiscard]] std::unique_ptr<node::NodeData>
  convert(const node::NodeData &data) override;
};

#endif//FLOW_INTEGER_TO_STRING_CONVERTER_H
