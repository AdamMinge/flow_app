/* ----------------------------------- Local -------------------------------- */
#include "flow_document/flow_document.h"

#include "flow_document/event/change_event.h"
#include "flow_document/flow/flow.h"
/* -------------------------------------------------------------------------- */

namespace flow_document {

std::unique_ptr<egnite::Document> FlowDocument::create() {
  auto document = std::unique_ptr<FlowDocument>(new FlowDocument());
  return document;
}

FlowDocument::FlowDocument(QObject *parent)
    : Document(QLatin1String("FlowDocument"), parent),
      m_flow(std::make_unique<Flow>()),
      m_current_layer(nullptr),
      m_current_node(nullptr) {}

FlowDocument::~FlowDocument() = default;

Flow *FlowDocument::getFlow() const { return m_flow.get(); }

Layer *FlowDocument::getCurrentLayer() const { return m_current_layer; }

Node *FlowDocument::getCurrentNode() const { return m_current_node; }

const QList<Layer *> &FlowDocument::getSelectedLayers() const {
  return m_selected_layers;
}

const QList<Node *> &FlowDocument::getSelectedNodes() const {
  return m_selected_nodes;
}

void FlowDocument::setCurrentLayer(Layer *layer) {
  if (m_current_layer == layer) return;

  m_current_layer = layer;
  Q_EMIT currentLayerChanged(m_current_layer);
}

void FlowDocument::setCurrentNode(Node *node) {
  if (m_current_node == node) return;

  m_current_node = node;
  Q_EMIT currentNodeChanged(m_current_node);
}

void FlowDocument::switchCurrentLayer(Layer *layer) {
  setCurrentLayer(layer);
  if (layer && !m_selected_layers.contains(layer)) setSelectedLayers({layer});
}

void FlowDocument::switchCurrentNode(Node *node) {
  setCurrentNode(node);
  if (node && !m_selected_nodes.contains(node)) setSelectedNodes({node});
}

void FlowDocument::setSelectedLayers(const QList<Layer *> &layers) {
  if (m_selected_layers == layers) return;

  m_selected_layers = layers;
  Q_EMIT selectedLayersChanged(m_selected_layers);
}

void FlowDocument::setSelectedNodes(const QList<Node *> &nodes) {
  if (m_selected_nodes == nodes) return;

  m_selected_nodes = nodes;
  Q_EMIT selectedNodesChanged(m_selected_nodes);
}

void FlowDocument::switchSelectedLayers(const QList<Layer *> &layers) {
  setSelectedLayers(layers);

  if (layers.contains(m_current_layer))
    setCurrentLayer(layers.isEmpty() ? nullptr : layers.first());
}

void FlowDocument::switchSelectedNodes(const QList<Node *> &nodes) {
  setSelectedNodes(nodes);

  if (nodes.contains(m_current_node))
    setCurrentNode(nodes.isEmpty() ? nullptr : nodes.first());
}

}  // namespace flow_document