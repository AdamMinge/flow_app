#ifndef FLOW_FLOW_EDITOR_H
#define FLOW_FLOW_EDITOR_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QHash>
#include <QPointer>
#include <QStackedWidget>
/* ----------------------------------- Local -------------------------------- */
#include "flow/editor/document/document_editor.h"
/* -------------------------------------------------------------------------- */

class FlowDocument;

class UndoDock;
class FlowView;

class FlowEditor : public DocumentEditor
{
  Q_OBJECT

private:
  struct Preferences;

public:
  explicit FlowEditor(QObject *parent = nullptr);
  ~FlowEditor() override;

  void setCurrentDocument(api::IDocument *document) override;

  void addDocument(api::IDocument *document) override;
  void removeDocument(api::IDocument *document) override;

  [[nodiscard]] api::IDocument *getCurrentDocument() const override;
  [[nodiscard]] QWidget *getEditorWidget() const override;

  void saveState() override;
  void restoreState() override;

  [[nodiscard]] QList<QDockWidget *> getDockWidgets() const override;
  [[nodiscard]] QList<utils::QtDialogWithToggleView *> getDialogWidgets() const override;

  void performStandardAction(StandardAction standard_action) override;
  [[nodiscard]] StandardActions getEnabledStandardActions() const override;

private:
  FlowDocument *m_current_document;
  QPointer<QMainWindow> m_main_window;

  QStackedWidget *m_scene_stack;
  UndoDock *m_undo_dock;

  QHash<FlowDocument *, FlowView *> m_view_for_document;

  QScopedPointer<Preferences> m_preferences;
};

#endif//FLOW_FLOW_EDITOR_H