/* ----------------------------------- Local -------------------------------- */
#include "flow/plugins/flow_document/command/move_object.h"

#include "flow/plugins/flow_document/event/objects_event.h"
#include "flow/plugins/flow_document/flow_document.h"
#include "flow/plugins/flow_document/node/object.h"
/* -------------------------------------------------------------------------- */

namespace plugin::flow_document {

MoveObject::MoveObject(FlowDocument *document, QList<Object *> objects,
                       const QPointF &move, flow::Command *parent)
    : flow::Command(QLatin1String("MoveObject"), parent),
      m_document(document),
      m_objects(std::move(objects)),
      m_move(move) {
  setText(QObject::tr("Move Item"));
}

MoveObject::~MoveObject() = default;

void MoveObject::redo() { execute(m_move); }

void MoveObject::undo() { execute(-m_move); }

void MoveObject::execute(const QPointF &move) {
  for (auto object : m_objects)
    object->setPosition(object->getPosition() + move);

  Q_EMIT m_document->event(ObjectsChangedPropertiesEvent(
      m_objects, ObjectsChangedPropertiesEvent::Property::PositionProperty));
}

}  // namespace plugin::flow_document