/* ----------------------------------- Local -------------------------------- */
#include "flow/document/event/layer_change_event.h"

#include "flow/document/flow/group_layer.h"
/* -------------------------------------------------------------------------- */

namespace flow {

/* --------------------------------- LayerEvent ----------------------------- */

LayerEvent::LayerEvent(Event event, GroupLayer* group_layer, qsizetype index)
    : ChangeEvent(type),
      m_event(event),
      m_group_layer(group_layer),
      m_index(index) {}

LayerEvent::~LayerEvent() = default;

LayerEvent::Event LayerEvent::getEvent() const { return m_event; }

GroupLayer* LayerEvent::getGroupLayer() const { return m_group_layer; }

qsizetype LayerEvent::getIndex() const { return m_index; }

/* ------------------------------ LayersChangeEvent ------------------------- */

LayersChangeEvent::LayersChangeEvent(QList<Layer*> layers, Property properties)
    : ChangeEvent(type),
      m_layers(std::move(layers)),
      m_properties(properties) {}

LayersChangeEvent::~LayersChangeEvent() = default;

const QList<Layer*>& LayersChangeEvent::getLayers() const { return m_layers; }

LayersChangeEvent::Property LayersChangeEvent::getProperties() const {
  return m_properties;
}

bool LayersChangeEvent::contains(Layer* layer) const {
  return m_layers.contains(layer);
}

}  // namespace flow
