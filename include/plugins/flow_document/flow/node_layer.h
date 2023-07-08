#ifndef FLOW_DOCUMENT_NODE_LAYER_H
#define FLOW_DOCUMENT_NODE_LAYER_H

/* ----------------------------------- Local -------------------------------- */
#include "flow_document/export.h"
#include "flow_document/flow/layer.h"
#include "flow_document/flow/node.h"
#include "flow_document/flow/serialization/object_unique_ptr.h"
/* -------------------------------------------------------------------------- */

namespace flow_document {

class FLOW_DOCUMENT_API NodeLayer : public Layer {
  FLOW_OBJECT_CLASS(NodeLayer, Layer)

 public:
  using Nodes = std::vector<ObjectUniquePtr<Node>>;

 public:
  explicit NodeLayer();
  ~NodeLayer() override;

  void append(std::unique_ptr<Node> node);
  void insert(qsizetype index, std::unique_ptr<Node> node);
  void remove(qsizetype index);

  [[nodiscard]] std::unique_ptr<Node> take(qsizetype index);
  [[nodiscard]] Node* at(qsizetype index) const;
  [[nodiscard]] qsizetype indexOf(Node* node) const;

  [[nodiscard]] qsizetype size() const;

  Nodes::iterator begin();
  Nodes::iterator end();

  Nodes::const_iterator begin() const;
  Nodes::const_iterator end() const;

  [[nodiscard]] std::unique_ptr<Layer> clone() const override;

  void serialize(utils::OArchive& archive) const override;
  void deserialize(utils::IArchive& archive) override;

 protected:
  void init(const NodeLayer* node_layer);

 private:
  Nodes m_nodes;
};

}  // namespace flow_document

#endif  // FLOW_DOCUMENT_NODE_LAYER_H