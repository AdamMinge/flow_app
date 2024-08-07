/* ----------------------------------- Local -------------------------------- */
#include "flow/document/editor.h"

#include "flow/document/action_handler.h"
#include "flow/document/component/dock_widget.h"
#include "flow/document/component/dock_widget_factory.h"
#include "flow/document/component/scene/scene.h"
#include "flow/document/component/scene/tool/tool.h"
#include "flow/document/component/scene/tool/tools_bar.h"
#include "flow/document/component/scene/view.h"
#include "flow/document/document.h"
/* ------------------------------------ Qt ---------------------------------- */
#include <QMainWindow>
/* ---------------------------------- Pillar -------------------------------- */
#include <pillar/document/undo_dock.h>
#include <pillar/preferences_manager.h>
/* -------------------------------------------------------------------------- */

namespace flow {

/* -------------------------------- Preferences ----------------------------- */

struct FlowEditor::Preferences {
  pillar::Preference<QSize> editor_size =
      pillar::Preference<QSize>(QString("flow_editor/size"));
  pillar::Preference<QByteArray> editor_state =
      pillar::Preference<QByteArray>(QString("flow_editor/state"));
};

/* --------------------------------- FlowEditor ----------------------------- */

FlowEditor::FlowEditor(QObject *parent)
    : pillar::DocumentEditor(parent),
      m_current_document(nullptr),
      m_main_window(new QMainWindow()),
      m_tools_bar(new ToolsBar(m_main_window.get())),
      m_scene_stack(new QStackedWidget(m_main_window.get())),
      m_undo_dock(new pillar::UndoDock(m_main_window.get())),
      m_action_handler(ActionHandler::getInstance()),
      m_preferences(new Preferences) {
  initUi();
  initConnections();
  loadObjects();
}

FlowEditor::~FlowEditor() { ActionHandler::deleteInstance(); }

void FlowEditor::setCurrentDocument(pillar::Document *document) {
  if (m_current_document == document) return;

  auto flow_document = qobject_cast<FlowDocument *>(document);
  Q_ASSERT(flow_document || !document);

  m_current_document = flow_document;

  m_action_handler.setDocument(m_current_document);
  m_tools_bar->setDocument(m_current_document);
  m_undo_dock->setStack(flow_document ? flow_document->getUndoStack()
                                      : nullptr);

  for (auto dock_widget : m_dock_widget_for_factory) {
    dock_widget->setDocument(m_current_document);
  }

  if (auto flow_view = m_view_for_document[flow_document]; flow_view) {
    m_scene_stack->setCurrentWidget(flow_view);
    toolSelected(m_tools_bar->getSelectedTool());
  }
}

void FlowEditor::addDocument(pillar::Document *document) {
  auto flow_document = dynamic_cast<FlowDocument *>(document);
  Q_ASSERT(flow_document);

  auto view = new FlowView(m_scene_stack);
  auto scene = new FlowScene(view);

  scene->setDocument(flow_document);
  view->setScene(scene);
  view->setAcceptDrops(true);

  m_view_for_document.insert(flow_document, view);

  m_scene_stack->addWidget(view);
}

void FlowEditor::removeDocument(pillar::Document *document) {
  auto flow_document = dynamic_cast<FlowDocument *>(document);
  Q_ASSERT(flow_document);
  Q_ASSERT(m_view_for_document.contains(flow_document));

  if (flow_document == m_current_document) setCurrentDocument(nullptr);

  auto view = m_view_for_document.take(flow_document);
  auto scene = view->scene();
  Q_ASSERT(scene);

  m_scene_stack->removeWidget(view);
  scene->clear();

  view->deleteLater();
}

pillar::Document *FlowEditor::getCurrentDocument() const {
  return m_current_document;
}

QWidget *FlowEditor::getEditorWidget() const { return m_main_window.get(); }

void FlowEditor::saveState() {
  m_preferences->editor_state = m_main_window->saveState();
  m_preferences->editor_size = m_main_window->size();
}

void FlowEditor::restoreState() {
  auto editor_size = static_cast<QSize>(m_preferences->editor_size);
  auto editor_state = static_cast<QByteArray>(m_preferences->editor_state);

  if (!editor_size.isNull()) m_main_window->resize(editor_size);
  if (!editor_state.isNull()) m_main_window->restoreState(editor_state);
}

QList<QDockWidget *> FlowEditor::getDockWidgets() const {
  auto dock_widgets = QList<QDockWidget *>{m_undo_dock};

  for (auto dock_widget : m_dock_widget_for_factory) {
    dock_widgets.append(dock_widget);
  }

  return dock_widgets;
}

QList<qtils::QtDialogWithToggleView *> FlowEditor::getDialogWidgets() const {
  return QList<qtils::QtDialogWithToggleView *>{};
}

void FlowEditor::performStandardAction(StandardAction standard_action) {
  // TODO : implementation //
}

FlowEditor::StandardActions FlowEditor::getEnabledStandardActions() const {
  // TODO : implementation //
  FlowEditor::StandardActions standard_actions;
  return standard_actions;
}

QString FlowEditor::getDocumentId() const {
  return QLatin1String("FlowDocument");
}

void FlowEditor::addedObject(FlowDockWidgetFactory *factory) {
  auto widget = factory->create(m_main_window.get());
  auto area = factory->getDockWidgetArea();

  m_dock_widget_for_factory[factory] = widget;
  addDockWidget(widget, area);
}

void FlowEditor::removedObject(FlowDockWidgetFactory *factory) {
  if (m_dock_widget_for_factory.contains(factory)) {
    m_dock_widget_for_factory.remove(factory);
  }
}

void FlowEditor::toolSelected(Tool *tool) {
  auto flow_view = m_view_for_document[m_current_document];
  if (!flow_view) return;

  auto flow_scene = flow_view->getScene();
  auto prev_tool = flow_scene->getTool();

  if (prev_tool) {
    disconnect(prev_tool, &Tool::cursorChanged, this,
               &FlowEditor::cursorChanged);
  }

  flow_scene->setTool(tool);
  flow_view->viewport()->setCursor(tool ? tool->getCursor() : Qt::ArrowCursor);

  if (tool) {
    connect(tool, &Tool::cursorChanged, this, &FlowEditor::cursorChanged);
  }
}

void FlowEditor::cursorChanged(const QCursor &cursor) {
  auto flow_view = m_view_for_document[m_current_document];
  flow_view->viewport()->setCursor(cursor);
}

void FlowEditor::initUi() {
  m_main_window->setDockOptions(m_main_window->dockOptions() |
                                QMainWindow::GroupedDragging);
  m_main_window->setDockNestingEnabled(true);
  m_main_window->setCentralWidget(m_scene_stack);
  m_main_window->addToolBar(m_tools_bar);

  addDockWidget(m_undo_dock, Qt::LeftDockWidgetArea);
}

void FlowEditor::initConnections() {
  connect(m_tools_bar, &ToolsBar::toolSelected, this,
          &FlowEditor::toolSelected);
}

void FlowEditor::addDockWidget(QDockWidget *dock_widget,
                               Qt::DockWidgetArea area) {
  if (m_main_window->dockWidgetArea(dock_widget) != Qt::NoDockWidgetArea)
    return;

  m_main_window->addDockWidget(area, dock_widget);

  const auto dock_widgets = m_main_window->findChildren<QDockWidget *>();
  auto same_area_iter =
      std::find_if(dock_widgets.begin(), dock_widgets.end(),
                   [this, dock_widget, area](auto widget) {
                     return m_main_window->dockWidgetArea(widget) == area &&
                            dock_widget != widget;
                   });

  if (same_area_iter != dock_widgets.end()) {
    m_main_window->tabifyDockWidget(*same_area_iter, dock_widget);
  }
}

}  // namespace flow