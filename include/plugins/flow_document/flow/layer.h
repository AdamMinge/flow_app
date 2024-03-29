#ifndef FLOW_DOCUMENT_LAYER_H
#define FLOW_DOCUMENT_LAYER_H

/* ----------------------------------- Local -------------------------------- */
#include "flow_document/export.h"
#include "flow_document/flow/object.h"
/* -------------------------------------------------------------------------- */

namespace flow_document {

class GroupLayer;

/* ----------------------------------- Layer -------------------------------- */

class FLOW_DOCUMENT_API Layer : public MoveableObject {
  friend GroupLayer;
  FLOW_OBJECT_CLASS(Layer, MoveableObject)

 public:
  explicit Layer();
  ~Layer() override;

  void setOpacity(qreal opacity);
  [[nodiscard]] qreal getOpacity() const;

  void setVisible(bool visible);
  [[nodiscard]] bool isVisible() const;

  void setLocked(bool locked);
  [[nodiscard]] bool isLocked() const;

  [[nodiscard]] GroupLayer* getParent() const;
  [[nodiscard]] GroupLayer* getRoot() const;

  [[nodiscard]] bool isParent(Layer* layer) const;
  [[nodiscard]] bool isParentOrSelf(Layer* layer) const;

  [[nodiscard]] virtual std::unique_ptr<Layer> clone() const = 0;

  void serialize(utils::OArchive& archive) const override;
  void deserialize(utils::IArchive& archive) override;

 protected:
  void init(const Layer* layer);
  void setParent(GroupLayer* parent);

 private:
  GroupLayer* m_parent;
  qreal m_opacity;
  bool m_visible;
  bool m_locked;
};

/* -------------------------------- Layer Utils ----------------------------- */

[[nodiscard]] qsizetype getLayerHierarchicalId(Layer* layer);
[[nodiscard]] QList<qsizetype> getLayersHierarchicalIds(
    const QList<Layer*>& layers);

[[nodiscard]] Layer* getLayerByHierarchicalId(Layer* root, qsizetype id);
[[nodiscard]] QList<Layer*> getLayersByHierarchicalIds(
    Layer* root, const QList<qsizetype>& ids);

}  // namespace flow_document

#endif  // FLOW_DOCUMENT_LAYER_H