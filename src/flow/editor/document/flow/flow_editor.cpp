/* ------------------------------------ Qt ---------------------------------- */
#include <QMainWindow>
/* ----------------------------------- Local -------------------------------- */
#include "flow/editor/console_dock.h"
#include "flow/editor/document/flow/flow_document.h"
#include "flow/editor/document/flow/flow_editor.h"
#include "flow/editor/document/flow/flow_scene.h"
#include "flow/editor/document/flow/flow_view.h"
#include "flow/editor/document/undo_dock.h"
#include "flow/editor/preferences_manager.h"
/* -------------------------------------------------------------------------- */

/* -------------------------------- Preferences ----------------------------- */

struct FlowEditor::Preferences {
  Preference<QSize> editor_size = Preference<QSize>(QString("flow_editor/size"));
  Preference<QByteArray> editor_state = Preference<QByteArray>(QString("flow_editor/state"));
};

/* -------------------------------- SceneEditor ----------------------------- */

FlowEditor::FlowEditor(QObject *parent)
    : DocumentEditor(parent),
      m_current_document(nullptr),
      m_main_window(new QMainWindow()),
      m_scene_stack(new QStackedWidget(m_main_window)),
      m_undo_dock(new UndoDock(m_main_window)),
      m_preferences(new Preferences)
{
  m_main_window->setDockOptions(m_main_window->dockOptions() | QMainWindow::GroupedDragging);
  m_main_window->setDockNestingEnabled(true);

  m_main_window->setCentralWidget(m_scene_stack);

  m_main_window->addDockWidget(Qt::LeftDockWidgetArea, m_undo_dock);
}

FlowEditor::~FlowEditor() = default;

void FlowEditor::setCurrentDocument(api::IDocument *document)
{
  if (m_current_document == document)
    return;

  auto flow_document = qobject_cast<FlowDocument *>(document);
  Q_ASSERT(flow_document || !document);

  m_current_document = flow_document;

  if (m_current_document)
    m_undo_dock->setStack(flow_document->getUndoStack());
}

void FlowEditor::addDocument(api::IDocument *document)
{
  auto flow_document = dynamic_cast<FlowDocument *>(document);
  Q_ASSERT(flow_document);

  auto view = new FlowView(m_scene_stack);
  auto scene = new FlowScene(view);

  view->setScene(scene);

  m_view_for_document.insert(flow_document, view);
  m_scene_stack->addWidget(view);
}

void FlowEditor::removeDocument(api::IDocument *document)
{
  auto flow_document = dynamic_cast<FlowDocument *>(document);
  Q_ASSERT(flow_document);
  Q_ASSERT(m_view_for_document.contains(flow_document));

  if (flow_document == m_current_document)
    setCurrentDocument(nullptr);

  auto view = m_view_for_document.take(flow_document);
  m_scene_stack->removeWidget(view);

  view->deleteLater();
}

api::IDocument *FlowEditor::getCurrentDocument() const
{
  return m_current_document;
}

QWidget *FlowEditor::getEditorWidget() const
{
  return m_main_window;
}

void FlowEditor::saveState()
{
  m_preferences->editor_state = m_main_window->saveState();
  m_preferences->editor_size = m_main_window->size();
}

void FlowEditor::restoreState()
{
  auto editor_size = static_cast<QSize>(m_preferences->editor_size);
  auto editor_state = static_cast<QByteArray>(m_preferences->editor_state);

  if (!editor_size.isNull())
    m_main_window->resize(editor_size);

  if (!editor_state.isNull())
    m_main_window->restoreState(editor_state);
}

QList<QDockWidget *> FlowEditor::getDockWidgets() const
{
  return QList<QDockWidget *>{m_undo_dock};
}

QList<utils::QtDialogWithToggleView *> FlowEditor::getDialogWidgets() const
{
  return QList<utils::QtDialogWithToggleView *>{};
}

void FlowEditor::performStandardAction(StandardAction standard_action)
{
  // TODO : implementation //
}

FlowEditor::StandardActions FlowEditor::getEnabledStandardActions() const
{
  // TODO : implementation //
  FlowEditor::StandardActions standard_actions;
  return standard_actions;
}