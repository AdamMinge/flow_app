#ifndef FLOW_DOCUMENT_FLOW_DOCUMENT_PLUGIN_H
#define FLOW_DOCUMENT_FLOW_DOCUMENT_PLUGIN_H

/* ---------------------------------- Egnite -------------------------------- */
#include <egnite/plugin_interface.h>
/* ----------------------------------- Local -------------------------------- */
#include "flow_document/export.h"
/* -------------------------------------------------------------------------- */

namespace flow_document {

class FLOW_DOCUMENT_API FlowDocumentPlugin : public egnite::PluginInterface {
  Q_OBJECT
  Q_INTERFACES(egnite::PluginInterface)
  Q_PLUGIN_METADATA(IID "org.flow.PluginInterface" FILE "plugin.json")

 public:
  explicit FlowDocumentPlugin();
  ~FlowDocumentPlugin() override;

  void init() override;
};

}  // namespace flow_document

#endif  // FLOW_DOCUMENT_FLOW_DOCUMENT_PLUGIN_H
