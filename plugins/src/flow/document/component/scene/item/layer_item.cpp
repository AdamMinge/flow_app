/* ----------------------------------- Local -------------------------------- */
#include "flow/document/component/scene/item/layer_item.h"

#include "flow/document/component/scene/item/connection_item.h"
#include "flow/document/component/scene/item/factory/utils.h"
#include "flow/document/component/scene/item/node_item.h"
#include "flow/document/event/change_event.h"
#include "flow/document/event/connection_change_event.h"
#include "flow/document/event/layer_change_event.h"
#include "flow/document/event/node_change_event.h"
#include "flow/document/flow/group_layer.h"
#include "flow/document/flow/layer.h"
#include "flow/document/flow/node.h"
#include "flow/document/flow/node_layer.h"
/* -------------------------------------------------------------------------- */

namespace flow {

/* --------------------------------- LayerItem ------------------------------ */

LayerItem::LayerItem(Layer* layer, FlowDocument* document,
                     QGraphicsItem* parent)
    : ObjectItem(layer, document, parent) {}

LayerItem::~LayerItem() = default;

void LayerItem::init() {
  setPos(getLayer()->getPosition());
  setVisible(getLayer()->isVisible());
}

Layer* LayerItem::getLayer() const { return static_cast<Layer*>(getObject()); }

void LayerItem::onEvent(const ChangeEvent& event) {
  if (event.getType() == LayersChangeEvent::type) {
    const auto& layer_event = static_cast<const LayersChangeEvent&>(event);
    if (layer_event.contains(getLayer())) onUpdate(layer_event);
  }
}

void LayerItem::onUpdate(const LayersChangeEvent& event) {
  const auto layer = getLayer();
  const auto properties = event.getProperties();

  using enum LayersChangeEvent::Property;
  if (properties & Visible) {
    setVisible(layer->isVisible());
  }
  if (properties & Locked) {
    // TODO
  }
  if (properties & Opacity) {
    setOpacity(layer->getOpacity());
  }
  if (properties & Position) {
    setPos(layer->getPosition());
  }
}

/* -------------------------- GroupLayerItem ------------------------ */

GroupLayerItem::GroupLayerItem(GroupLayer* layer, FlowDocument* document,
                               QGraphicsItem* parent)
    : LayerItem(layer, document, parent) {
  for (auto& layer : *getGroupLayer()) {
    m_layer_items.append(
        createItem<LayerItem>(layer.get(), getDocument(), this));
  }
}

GroupLayerItem::~GroupLayerItem() = default;

void GroupLayerItem::init() {
  LayerItem::init();

  for (auto& layer : *getGroupLayer()) {
    m_layer_items.append(
        createItem<LayerItem>(layer.get(), getDocument(), this));
  }
}

GroupLayer* GroupLayerItem::getGroupLayer() const {
  return static_cast<GroupLayer*>(getObject());
}

void GroupLayerItem::onEvent(const ChangeEvent& event) {
  LayerItem::onEvent(event);

  if (event.getType() == LayerEvent::type) {
    const auto& layer_event = static_cast<const LayerEvent&>(event);
    switch (layer_event.getEvent()) {
      using enum LayerEvent::Event;

      case Added: {
        if (getGroupLayer() == layer_event.getGroupLayer()) {
          auto layer = layer_event.getGroupLayer()->at(layer_event.getIndex());

          m_layer_items.insert(
              layer_event.getIndex(),
              createItem<LayerItem>(layer, getDocument(), this));
        }

        break;
      }

      case Removed: {
        if (getGroupLayer() == layer_event.getGroupLayer()) {
          auto item = m_layer_items.takeAt(layer_event.getIndex());
          item->deleteLater();
        }

        break;
      }
    }
  }
}

/* --------------------------- NodeLayerItem ------------------------ */

NodeLayerItem::NodeLayerItem(NodeLayer* layer, FlowDocument* document,
                             QGraphicsItem* parent)
    : LayerItem(layer, document, parent) {}

NodeLayerItem::~NodeLayerItem() = default;

void NodeLayerItem::init() {
  LayerItem::init();

  auto node_layer = getNodeLayer();

  for (auto i = 0; i < node_layer->nodesCount(); ++i) {
    auto node = node_layer->nodeAt(i);
    m_node_items.append(createItem<NodeItem>(node, getDocument(), this));
  }

  for (auto i = 0; i < node_layer->connectionsCount(); ++i) {
    auto connection = node_layer->connectionAt(i);
    m_connection_items.append(
        createItem<ConnectionItem>(connection, getDocument(), this));
  }
}

NodeLayer* NodeLayerItem::getNodeLayer() const {
  return static_cast<NodeLayer*>(getObject());
}

void NodeLayerItem::onEvent(const ChangeEvent& event) {
  LayerItem::onEvent(event);

  if (event.getType() == NodeEvent::type) {
    const auto& node_event = static_cast<const NodeEvent&>(event);
    switch (node_event.getEvent()) {
      using enum NodeEvent::Event;

      case Added: {
        if (getNodeLayer() == node_event.getNodeLayer()) {
          auto node = node_event.getNodeLayer()->nodeAt(node_event.getIndex());

          m_node_items.insert(node_event.getIndex(),
                              createItem<NodeItem>(node, getDocument(), this));
        }

        break;
      }

      case Removed: {
        if (getNodeLayer() == node_event.getNodeLayer()) {
          auto item = m_node_items.takeAt(node_event.getIndex());
          item->deleteLater();
        }

        break;
      }
    }
  } else if (event.getType() == ConnectionEvent::type) {
    const auto& connection_event = static_cast<const ConnectionEvent&>(event);
    switch (connection_event.getEvent()) {
      using enum ConnectionEvent::Event;

      case Added: {
        if (getNodeLayer() == connection_event.getNodeLayer()) {
          auto node = connection_event.getNodeLayer()->connectionAt(
              connection_event.getIndex());

          m_connection_items.insert(
              connection_event.getIndex(),
              createItem<ConnectionItem>(node, getDocument(), this));
        }

        break;
      }

      case Removed: {
        if (getNodeLayer() == connection_event.getNodeLayer()) {
          auto item = m_connection_items.takeAt(connection_event.getIndex());
          item->deleteLater();
        }

        break;
      }
    }
  }

  auto z = 0;
  for (auto item : m_node_items) item->setZValue(z++);
  for (auto item : m_connection_items) item->setZValue(z++);
}

}  // namespace flow