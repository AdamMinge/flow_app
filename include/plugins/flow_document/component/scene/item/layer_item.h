#ifndef FLOW_DOCUMENT_LAYER_ITEM_H
#define FLOW_DOCUMENT_LAYER_ITEM_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QGraphicsObject>
/* ----------------------------------- Local -------------------------------- */
#include "flow_document/component/scene/item/object_item.h"
/* -------------------------------------------------------------------------- */

namespace flow_document {

class Layer;
class GroupLayer;
class NodeLayer;
class LayersChangeEvent;
class NodeItem;

/* --------------------------------- LayerItem ------------------------------ */

class FLOW_DOCUMENT_API LayerItem : public ObjectItem {
  Q_OBJECT

 public:
  explicit LayerItem(Layer* layer, FlowDocument* document,
                     QGraphicsItem* parent = nullptr);
  ~LayerItem() override;

  [[nodiscard]] Layer* getLayer() const;

 protected Q_SLOTS:
  void onEvent(const ChangeEvent& event) override;
  void onUpdate(const LayersChangeEvent& event);
};

/* ------------------------------ GroupLayerItem ---------------------------- */

class FLOW_DOCUMENT_API GroupLayerItem : public LayerItem {
  Q_OBJECT

 public:
  explicit GroupLayerItem(GroupLayer* layer, FlowDocument* document,
                          QGraphicsItem* parent = nullptr);
  ~GroupLayerItem() override;

  [[nodiscard]] GroupLayer* getGroupLayer() const;

 protected Q_SLOTS:
  void onEvent(const ChangeEvent& event) override;

 private:
  QList<LayerItem*> m_layer_items;
};

/* ------------------------------ NodeLayerItem ----------------------------- */

class FLOW_DOCUMENT_API NodeLayerItem : public LayerItem {
  Q_OBJECT

 public:
  explicit NodeLayerItem(NodeLayer* layer, FlowDocument* document,
                         QGraphicsItem* parent = nullptr);
  ~NodeLayerItem() override;

  [[nodiscard]] NodeLayer* getNodeLayer() const;

 protected Q_SLOTS:
  void onEvent(const ChangeEvent& event) override;

 private:
  QList<NodeItem*> m_node_items;
};

}  // namespace flow_document

#endif  // FLOW_DOCUMENT_LAYER_ITEM_H