#ifndef FLOW_FLOW_DOCUMENT_H
#define FLOW_FLOW_DOCUMENT_H

/* ----------------------------------- Local -------------------------------- */
#include "flow/editor/document/document.h"
/* -------------------------------------------------------------------------- */

class FlowDocument : public Document
{
  Q_OBJECT

public:
  static std::unique_ptr<FlowDocument> create();

public:
  ~FlowDocument() override;

protected:
  explicit FlowDocument(QObject *parent = nullptr);
};

#endif//FLOW_FLOW_DOCUMENT_H