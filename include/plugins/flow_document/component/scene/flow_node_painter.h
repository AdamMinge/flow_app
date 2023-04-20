#ifndef PLUGIN_FLOW_FLOW_NODE_PAINTER_H
#define PLUGIN_FLOW_FLOW_NODE_PAINTER_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QPainter>
#include <QStyleOptionGraphicsItem>
/* ----------------------------------- Local -------------------------------- */
#include "flow_document/export.h"
/* -------------------------------------------------------------------------- */

namespace flow_document {

class FlowNodeItem;

class FLOW_DOCUMENT_API FlowNodePainter {
 public:
  explicit FlowNodePainter(const FlowNodeItem &node_item);
  virtual ~FlowNodePainter();

  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option);

 private:
  void paintNodeRect(QPainter *painter, const QStyleOptionGraphicsItem *option);
  void paintNodeLabel(QPainter *painter,
                      const QStyleOptionGraphicsItem *option);
  void paintNodePins(QPainter *painter, const QStyleOptionGraphicsItem *option);
  void paintNodePinLabels(QPainter *painter,
                          const QStyleOptionGraphicsItem *option);

 private:
  const FlowNodeItem &m_node_item;
};

}  // namespace flow_document

#endif  // PLUGIN_FLOW_FLOW_NODE_PAINTER_H
