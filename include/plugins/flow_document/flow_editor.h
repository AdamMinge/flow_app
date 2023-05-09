#ifndef FLOW_DOCUMENT_FLOW_EDITOR_H
#define FLOW_DOCUMENT_FLOW_EDITOR_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QHash>
#include <QPointer>
#include <QStackedWidget>
#include <QToolBar>
/* ---------------------------------- Egnite -------------------------------- */
#include <egnite/document/document_editor.h>
/* ----------------------------------- Local -------------------------------- */
#include "flow_document/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite {
class UndoDock;
}

namespace flow_document {

class FlowDocument;
class FlowView;

class NodesDock;
class AbstractTool;
class ToolsBar;

class FactoriesDock;
class PropertiesDock;
class ObjectsDock;

class FLOW_DOCUMENT_API FlowEditor : public egnite::DocumentEditor {
  Q_OBJECT
  Q_INTERFACES(egnite::DocumentEditor)

 private:
  struct Preferences;

 public:
  explicit FlowEditor(QObject *parent = nullptr);
  ~FlowEditor() override;

  void setCurrentDocument(egnite::Document *document) override;

  void addDocument(egnite::Document *document) override;
  void removeDocument(egnite::Document *document) override;

  [[nodiscard]] egnite::Document *getCurrentDocument() const override;
  [[nodiscard]] QWidget *getEditorWidget() const override;

  void saveState() override;
  void restoreState() override;

  [[nodiscard]] QList<QDockWidget *> getDockWidgets() const override;
  [[nodiscard]] QList<utils::QtDialogWithToggleView *> getDialogWidgets()
      const override;

  void performStandardAction(StandardAction standard_action) override;
  [[nodiscard]] StandardActions getEnabledStandardActions() const override;

  [[nodiscard]] QString getDocumentId() const override;

 private Q_SLOTS:
  void toolSelected(AbstractTool *tool);
  void cursorChanged(const QCursor &cursor);

 private:
  void initUi();
  void initConnections();

 private:
  FlowDocument *m_current_document;
  QPointer<QMainWindow> m_main_window;

  ToolsBar *m_tools_bar;
  QStackedWidget *m_scene_stack;
  egnite::UndoDock *m_undo_dock;

  FactoriesDock *m_factories_dock;
  PropertiesDock *m_properties_dock;
  ObjectsDock *m_objects_dock;

  QHash<FlowDocument *, FlowView *> m_view_for_document;

  QScopedPointer<Preferences> m_preferences;
};

}  // namespace flow_document

#endif  // FLOW_DOCUMENT_FLOW_EDITOR_H