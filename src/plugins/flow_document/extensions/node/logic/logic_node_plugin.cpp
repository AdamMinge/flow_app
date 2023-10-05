/* ----------------------------------- Local -------------------------------- */
#include "logic_node_plugin.h"
/* -------------------------------------------------------------------------- */

/* ------------------------------ LogicNodePlugin --------------------------- */

LogicNodePlugin::LogicNodePlugin() = default;

LogicNodePlugin::~LogicNodePlugin() = default;

void LogicNodePlugin::init() {
  addObject(new LogicNodeAndFactory(tr("And"), tr("Logic"), this));
  addObject(new LogicNodeNotFactory(tr("Not"), tr("Logic"), this));
  addObject(new LogicNodeOrFactory(tr("Or"), tr("Logic"), this));
  addObject(
      new LogicNodeBoolEmitterFactory(tr("Bool Emitter"), tr("Logic"), this));
  addObject(
      new LogicNodeBoolReceiverFactory(tr("Bool Receiver"), tr("Logic"), this));
}
