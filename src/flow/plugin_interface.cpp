/* ----------------------------------- Local -------------------------------- */
#include "flow/plugin_interface.h"

#include "flow/plugin_manager.h"
/* -------------------------------------------------------------------------- */

namespace flow {

PluginInterface::PluginInterface() = default;

PluginInterface::~PluginInterface() {
  for (auto object : m_added_objects) removeObject(object);
}

void PluginInterface::init() {}

void PluginInterface::addObject(QObject *object) {
  PluginManager::getInstance().addObject(object);
  m_added_objects.append(object);
}

void PluginInterface::removeObject(QObject *object) {
  PluginManager::getInstance().removeObject(object);
  m_added_objects.removeOne(object);
}

}  // namespace flow