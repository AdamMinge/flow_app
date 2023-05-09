#ifndef FLOW_DOCUMENT_NODE_FACTORY_H
#define FLOW_DOCUMENT_NODE_FACTORY_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
#include <QString>
/* ----------------------------------- Local -------------------------------- */
#include "flow_document/concept.h"
#include "flow_document/export.h"
#include "flow_document/flow/factory.h"
/* -------------------------------------------------------------------------- */

namespace flow_document {

class Node;

/* -------------------------------- NodeFactory ----------------------------- */

class NodeFactory : public Factory {
  Q_OBJECT

 public:
  explicit NodeFactory(QString name, QString id);
  ~NodeFactory() override;

  [[nodiscard]] virtual std::unique_ptr<Node> create() const = 0;

 private:
  QString m_id;
};

/* ------------------------------ BaseNodeFactory --------------------------- */

template <IsNode NODE>
class BaseNodeFactory : public NodeFactory {
 public:
  explicit BaseNodeFactory(QString name, QString id);

  [[nodiscard]] std::unique_ptr<Node> create() const override;
};

template <IsNode NODE>
BaseNodeFactory<NODE>::BaseNodeFactory(QString name, QString id)
    : NodeFactory(std::move(name), std::move(id)) {}

template <IsNode NODE>
std::unique_ptr<Node> BaseNodeFactory<NODE>::create() const {
  return std::make_unique<NODE>();
}

}  // namespace flow_document

#endif  // FLOW_DOCUMENT_NODE_FACTORY_H