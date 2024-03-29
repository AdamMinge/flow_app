#ifndef FLOW_DOCUMENT_FLOW_DOCUMENT_FORMAT_H
#define FLOW_DOCUMENT_FLOW_DOCUMENT_FORMAT_H

/* ---------------------------------- Egnite -------------------------------- */
#include <egnite/document/document_format.h>
/* ----------------------------------- Local -------------------------------- */
#include "flow_document/export.h"
/* -------------------------------------------------------------------------- */

namespace flow_document {

class FLOW_DOCUMENT_API FlowDocumentFormat : public egnite::DocumentFormat {
  Q_OBJECT
  Q_INTERFACES(egnite::DocumentFormat)

 public:
  explicit FlowDocumentFormat(QObject *parent = nullptr);
  ~FlowDocumentFormat() override;
};

}  // namespace flow_document

Q_DECLARE_INTERFACE(flow_document::FlowDocumentFormat,
                    "org.flow.FlowDocumentFormat")

#endif  // FLOW_DOCUMENT_FLOW_DOCUMENT_FORMAT_H
