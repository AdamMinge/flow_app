/* ----------------------------------- Local -------------------------------- */
#include "flow/document/flow/layer.h"

#include "flow/document/flow/group_layer.h"
#include "flow/document/flow/layer_iterator.h"
/* ----------------------------------- Qtils -------------------------------- */
#include <qtils/serializer/archive.h>
#include <qtils/serializer/archive_property.h>
/* -------------------------------------------------------------------------- */

namespace flow {

/* ----------------------------------- Layer -------------------------------- */

Layer::Layer()
    : m_parent(nullptr), m_opacity(1), m_visible(true), m_locked(false) {}

Layer::~Layer() = default;

qreal Layer::getOpacity() const { return m_opacity; }

void Layer::setOpacity(qreal opacity) { m_opacity = opacity; }

void Layer::setVisible(bool visible) { m_visible = visible; }

bool Layer::isVisible() const { return m_visible; }

void Layer::setLocked(bool locked) { m_locked = locked; }

bool Layer::isLocked() const { return m_locked; }

GroupLayer* Layer::getParent() const { return m_parent; }

GroupLayer* Layer::getRoot() const {
  auto root = getParent();
  while (root && root->getParent()) {
    root = root->getParent();
  }

  return root;
}

bool Layer::isParent(Layer* layer) const {
  auto root = getParent();
  while (root && root->getParent()) {
    if (root == layer) return true;
    root = root->getParent();
  }

  return false;
}

bool Layer::isParentOrSelf(Layer* layer) const {
  return layer == this || isParent(layer);
}

void Layer::init(const Layer* layer) {
  MoveableObject::init(layer);

  m_opacity = layer->m_opacity;
  m_visible = layer->m_visible;
  m_locked = layer->m_locked;
}

void Layer::setParent(GroupLayer* parent) { m_parent = parent; }

void Layer::serialize(qtils::OArchive& archive) const {
  MoveableObject::serialize(archive);

  archive << qtils::ArchiveProperty("opacity", m_opacity);
  archive << qtils::ArchiveProperty("visible", m_visible);
  archive << qtils::ArchiveProperty("locked", m_locked);
}

void Layer::deserialize(qtils::IArchive& archive) {
  MoveableObject::deserialize(archive);

  archive >> qtils::ArchiveProperty("opacity", m_opacity);
  archive >> qtils::ArchiveProperty("visible", m_visible);
  archive >> qtils::ArchiveProperty("locked", m_locked);
}

/* -------------------------------- Layer Qtils ----------------------------- */

qsizetype getLayerHierarchicalId(Layer* layer) {
  auto root = layer->getRoot();
  auto id = 0;
  if (root) {
    auto iter = LayerPreOrderIterator(root);
    while (iter.hasNext() && iter.next() != layer) id += 1;
  }

  return id;
}

QList<qsizetype> getLayersHierarchicalIds(const QList<Layer*>& layers) {
  if (layers.empty()) return {};

  auto ids = QList<qsizetype>(layers.size());
  auto root = layers.at(0)->getRoot();
  auto remaining = ids.size();
  auto id = 0;

  if (root) {
    auto iter = LayerPreOrderIterator(root);
    while (iter.hasNext() && remaining > 0) {
      auto layer = iter.next();
      auto index = layers.indexOf(layer);

      if (index >= 0) {
        ids[index] = id;
        remaining -= 1;
      }

      id += 1;
    }
  }

  return remaining == 0 ? ids : QList<qsizetype>{};
}

Layer* getLayerByHierarchicalId(Layer* root, qsizetype id) {
  if (root) {
    Q_ASSERT(!root->getRoot());
    auto layer = root;
    auto iter = LayerPreOrderIterator(layer);

    id += 1;
    while (iter.hasNext() && id > 0) {
      layer = iter.next();
      id -= 1;
    }

    if (id == 0) return layer;
  }

  return nullptr;
}

QList<Layer*> getLayersByHierarchicalIds(Layer* root,
                                         const QList<qsizetype>& ids) {
  if (!root) return {};

  Q_ASSERT(!root->getRoot());
  auto layers = QList<Layer*>(ids.size());
  auto iter = LayerPreOrderIterator(root);
  auto remaining = ids.size();
  auto id = 0;

  while (iter.hasNext() && remaining > 0) {
    auto layer = iter.next();
    auto index = ids.indexOf(id);

    if (index >= 0) {
      layers[index] = layer;
      remaining -= 1;
    }

    id += 1;
  }

  return remaining == 0 ? layers : QList<Layer*>{};
}

}  // namespace flow
