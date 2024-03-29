#ifndef FLOW_DOCUMENT_TOOL_FACTORY_H
#define FLOW_DOCUMENT_TOOL_FACTORY_H

/* ----------------------------------- Local -------------------------------- */
#include "flow_document/concept.h"
#include "flow_document/export.h"
/* -------------------------------------------------------------------------- */

namespace flow_document {

class Tool;

/* -------------------------------- ToolFactory ----------------------------- */

class FLOW_DOCUMENT_API ToolFactory : public QObject {
  Q_OBJECT

 public:
  explicit ToolFactory(QObject* parent = nullptr);
  ~ToolFactory() override;

  [[nodiscard]] virtual Tool* create(QObject* parent = nullptr) const = 0;
};

}  // namespace flow_document

Q_DECLARE_INTERFACE(flow_document::ToolFactory, "org.flow.ToolFactory")

namespace flow_document {

/* -------------------------- NodeSelectionToolFactory ---------------------- */

class FLOW_DOCUMENT_API NodeSelectionToolFactory : public ToolFactory {
  Q_OBJECT
  Q_INTERFACES(flow_document::ToolFactory)

 public:
  explicit NodeSelectionToolFactory(QObject* parent = nullptr);
  ~NodeSelectionToolFactory() override;

  [[nodiscard]] Tool* create(QObject* parent = nullptr) const override;
};

/* -------------------------- NodeConnectionToolFactory --------------------- */

class FLOW_DOCUMENT_API NodeConnectionToolFactory : public ToolFactory {
  Q_OBJECT
  Q_INTERFACES(flow_document::ToolFactory)

 public:
  explicit NodeConnectionToolFactory(QObject* parent = nullptr);
  ~NodeConnectionToolFactory() override;

  [[nodiscard]] Tool* create(QObject* parent = nullptr) const override;
};

}  // namespace flow_document

#endif  // FLOW_DOCUMENT_TOOL_FACTORY_H
