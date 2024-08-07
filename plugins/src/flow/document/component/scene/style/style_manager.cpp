/* ----------------------------------- Local -------------------------------- */
#include "flow/document/component/scene/style/style_manager.h"

#include "flow/document/component/scene/style/style.h"
#include "flow/document/component/scene/style/style_reader.h"
#include "flow/document/component/scene/style/style_writer.h"
#include "flow/document/resources.h"
/* -------------------------------------------------------------------------- */

namespace flow {

std::unique_ptr<StyleManager> StyleManager::m_instance =
    std::unique_ptr<StyleManager>(nullptr);

StyleManager &StyleManager::getInstance() {
  if (!m_instance) m_instance.reset(new StyleManager);

  return *m_instance;
}

void StyleManager::deleteInstance() { m_instance.reset(nullptr); }

StyleManager::StyleManager() { resetStyle(); }

StyleManager::~StyleManager() = default;

void StyleManager::setStyle(const FlowStyle &style) { *m_style = style; }

void StyleManager::resetStyle() { loadStyle(styles::Default); }

bool StyleManager::loadStyle(const QString &file_name) {
  auto reader = FlowStyleReader{};
  auto loaded_style = reader.read(file_name);
  if (!loaded_style) return false;

  m_style = std::move(loaded_style);
  return true;
}

bool StyleManager::saveStyle(const QString &file_name) {
  auto reader = FlowStyleWriter{};
  return reader.write(*m_style, file_name);
}

const FlowStyle &StyleManager::getStyle() const { return *m_style; }

}  // namespace flow
