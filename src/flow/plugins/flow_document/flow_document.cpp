/* ----------------------------------- Local -------------------------------- */
#include "flow/plugins/flow_document/flow_document.h"

#include "flow/plugins/flow_document/event/objects_event.h"
#include "flow/plugins/flow_document/node/node.h"
/* -------------------------------------------------------------------------- */

namespace plugin::flow_document {

std::unique_ptr<flow::Document> FlowDocument::create() {
  auto document = std::unique_ptr<FlowDocument>(new FlowDocument());
  return document;
}

FlowDocument::FlowDocument(QObject *parent)
    : Document(QLatin1String("FlowDocument"), parent) {}

FlowDocument::~FlowDocument() { qDeleteAll(m_nodes); }

void FlowDocument::addNode(Node *node) {
  m_nodes.append(node);
  Q_EMIT event(ObjectsAddedEvent(node, ObjectsAddedEvent::ObjectsType::Node));
}

void FlowDocument::removeNode(Node *node) {
  m_nodes.removeOne(node);
  Q_EMIT event(ObjectsRemovedEvent(node, ObjectsAddedEvent::ObjectsType::Node));
}

}  // namespace plugin::flow_document