#ifndef FLOW_DOCUMENT_CONNECTION_H
#define FLOW_DOCUMENT_CONNECTION_H

/* ----------------------------------- Local -------------------------------- */
#include "flow/document/export.h"
#include "flow/document/flow/object.h"
/* -------------------------------------------------------------------------- */

namespace flow {

class NodeLayer;
class Node;

/* ------------------------------ ConnectionSide ---------------------------- */

class LIB_FLOW_DOCUMENT_API ConnectionSide {
 public:
  ConnectionSide(QUuid node_id = {}, size_t pin_id = 0);
  ~ConnectionSide();

  [[nodiscard]] bool isNull() const;

  void setNodeId(const QUuid &id);
  [[nodiscard]] QUuid getNodeId() const;

  void setPinId(size_t id);
  [[nodiscard]] size_t getPinId() const;

  void serialize(qtils::OArchive &archive) const;
  void deserialize(qtils::IArchive &archive);

  [[nodiscard]] bool operator==(const ConnectionSide &other) const;
  [[nodiscard]] bool operator!=(const ConnectionSide &other) const;

 private:
  QUuid m_node_id;
  size_t m_pin_id;
};

/* --------------------------------- Connection ----------------------------- */

class LIB_FLOW_DOCUMENT_API Connection : public Object {
  friend NodeLayer;

  FLOW_OBJECT_CLASS(Connection, Object)

 public:
  explicit Connection();
  ~Connection() override;

  void setOutputSide(ConnectionSide connection_side);
  void setInputSide(ConnectionSide connection_side);

  const ConnectionSide &getOutputSide() const;
  const ConnectionSide &getInputSide() const;

  void setVisible(bool visible);
  [[nodiscard]] bool isVisible() const;

  [[nodiscard]] NodeLayer *getParent() const;

  [[nodiscard]] std::unique_ptr<Connection> clone() const;

  void serialize(qtils::OArchive &archive) const override;
  void deserialize(qtils::IArchive &archive) override;

 protected:
  void init(const Connection *connection);
  void setParent(NodeLayer *parent);

 private:
  NodeLayer *m_parent;
  bool m_visible;

  ConnectionSide m_output_side;
  ConnectionSide m_input_side;
};

}  // namespace flow

#endif  // FLOW_DOCUMENT_CONNECTION_H