#ifndef FLOW_DOCUMENT_CONNECTION_PROPERTIES_FACTORY_H
#define FLOW_DOCUMENT_CONNECTION_PROPERTIES_FACTORY_H

/* ----------------------------------- Local -------------------------------- */
#include "flow_document/component/properties/factory/object_properties_factory.h"
/* -------------------------------------------------------------------------- */

namespace flow_document {

/* ------------------------ ConnectionPropertiesFactory --------------------- */

class FLOW_DOCUMENT_API ConnectionPropertiesFactory
    : public ObjectPropertiesFactory {
  Q_OBJECT
  Q_INTERFACES(flow_document::ObjectPropertiesFactory)

 public:
  explicit ConnectionPropertiesFactory(QObject* parent = nullptr);

  [[nodiscard]] ObjectProperties* create(
      QObject* parent = nullptr) const override;

  [[nodiscard]] QString getObjectClassName() const override;
};

}  // namespace flow_document

#endif  // FLOW_DOCUMENT_CONNECTION_PROPERTIES_FACTORY_H