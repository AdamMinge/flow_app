/* ----------------------------------- Local -------------------------------- */
#include "flow_document/command/factory/node_command_factory.h"

#include "flow_document/command/change_node.h"
#include "flow_document/flow/node.h"
/* -------------------------------------------------------------------------- */

namespace flow_document {

/* ----------------------------------- Utils -------------------------------- */

namespace {

QList<Node*> toNodes(QList<Object*>&& objects) {
  QList<Node*> nodes;
  std::transform(std::make_move_iterator(objects.begin()),
                 std::make_move_iterator(objects.end()),
                 std::back_inserter(nodes), [](Object* object) {
                   Q_ASSERT(object->isClassOrChild<Node>());
                   return static_cast<Node*>(object);
                 });
  return nodes;
}

}  // namespace

/* ----------------------------- NodeCommandFactory ------------------------- */

NodeCommandFactory::NodeCommandFactory(QObject* parent)
    : ObjectCommandFactory(parent) {}

NodeCommandFactory::~NodeCommandFactory() = default;

QString NodeCommandFactory::getObjectClassName() const {
  return Node::sGetClassName();
}

egnite::Command* NodeCommandFactory::createSetVisible(FlowDocument* document,
                                                      QList<Object*> objects,
                                                      bool visible) const {
  return new SetNodesVisible(document, toNodes(std::move(objects)), visible);
}

egnite::Command* NodeCommandFactory::createSetName(FlowDocument* document,
                                                   QList<Object*> objects,
                                                   QString name) const {
  return new SetNodesName(document, toNodes(std::move(objects)),
                          std::move(name));
}

egnite::Command* NodeCommandFactory::createSetPosition(FlowDocument* document,
                                                       QList<Object*> objects,
                                                       QPointF position) const {
  return new SetNodesPosition(document, toNodes(std::move(objects)),
                              std::move(position));
}

egnite::Command* NodeCommandFactory::createAddCustomProperties(
    FlowDocument* document, QList<Object*> objects,
    QVariantMap properties) const {
  return new AddNodesProperties(document, toNodes(std::move(objects)),
                                std::move(properties));
}

egnite::Command* NodeCommandFactory::createRemoveCustomProperties(
    FlowDocument* document, QList<Object*> objects,
    QStringList properties) const {
  return new RemoveNodesProperties(document, toNodes(std::move(objects)),
                                   std::move(properties));
}

egnite::Command* NodeCommandFactory::createSetCustomProperty(
    FlowDocument* document, QList<Object*> objects, QString property,
    QVariant value) const {
  return new SetNodesCustomProperty(document, toNodes(std::move(objects)),
                                    std::move(property), std::move(value));
}

}  // namespace flow_document