/* ----------------------------------- Local -------------------------------- */
#include "project/project_window.h"

#include "console_dock.h"
#include "document/new_document_dialog.h"
#include "issue_dock.h"
#include "project/new_project_dialog.h"
#include "project/project_dock.h"
/* ------------------------------------ Qt ---------------------------------- */
#include <QCloseEvent>
#include <QMessageBox>
/* ---------------------------------- Pillar -------------------------------- */
#include <pillar/action_manager.h>
#include <pillar/document/document.h>
#include <pillar/document/document_editor.h>
#include <pillar/document/document_format.h>
#include <pillar/document/document_manager.h>
#include <pillar/format_helper.h>
#include <pillar/preferences_manager.h>
#include <pillar/project/project.h>
#include <pillar/project/project_format.h>
#include <pillar/project/project_manager.h>
/* ----------------------------------- Qtils -------------------------------- */
#include <qtils/action/action.h>
#include <qtils/dialog/dialog_with_toggle_view.h>
#include <qtils/dialog/extended_file_dialog.h>
#include <qtils/dpi/dpi.h>
/* ------------------------------------ Ui ---------------------------------- */
#include "project/ui_project_window.h"
/* -------------------------------------------------------------------------- */

/* ------------------------------- adjustMenuSize --------------------------- */

void adjustMenuSize(QObject *object) {
  if (auto menu = qobject_cast<QMenu *>(object); menu) {
    auto max_width = 0;
    auto extra_width = qtils::dpiScaled(55);
    for (auto action : menu->actions()) {
      const auto fontMetrics = QFontMetrics(action->font());
      auto width = fontMetrics.horizontalAdvance(action->text());

      if (auto child_menu = action->menu(); child_menu) {
        adjustMenuSize(child_menu);
        width += qtils::dpiScaled(5);
      }

      if (!action->shortcut().isEmpty()) extra_width = qtils::dpiScaled(120);

      if (max_width < width) max_width = width;
    }

    menu->setMinimumWidth(max_width + extra_width);
  } else {
    for (auto child_menu :
         object->findChildren<QMenu *>(QString{}, Qt::FindDirectChildrenOnly))
      adjustMenuSize(child_menu);
  }
}

/* -------------------------------- Preferences ----------------------------- */

struct ProjectWindow::Preferences {
  pillar::Preference<QByteArray> project_window_geometry =
      pillar::Preference<QByteArray>("project_window/geometry");
  pillar::Preference<QByteArray> project_window_state =
      pillar::Preference<QByteArray>("project_window/state");
};

/* ------------------------------- ProjectWindow ---------------------------- */

ProjectWindow::ProjectWindow(QWidget *parent)
    : QMainWindow(parent),
      m_ui(new Ui::ProjectWindow),
      m_preferences(new Preferences),
      m_project_dock(new ProjectDock(this)),
      m_console_dock(new ConsoleDock(this)),
      m_issue_dock(new IssueDock(this)),
      m_project_menu(new QMenu(this)),
      m_new_project_action(
          qtils::createActionWithShortcut(QKeySequence{}, this)),
      m_open_project_action(
          qtils::createActionWithShortcut(QKeySequence{}, this)),
      m_open_recent_project_menu(new QMenu(this)),
      m_clear_recent_projects_action(
          qtils::createActionWithShortcut(QKeySequence{}, this)),
      m_close_project_action(
          qtils::createActionWithShortcut(QKeySequence{}, this)),
      m_document_menu(new QMenu(this)),
      m_new_document_action(
          qtils::createActionWithShortcut(QKeySequence::New, this)),
      m_open_document_action(
          qtils::createActionWithShortcut(QKeySequence::Open, this)),
      m_close_document_action(
          qtils::createActionWithShortcut(QKeySequence{}, this)),
      m_save_document_action(
          qtils::createActionWithShortcut(QKeySequence::Save, this)),
      m_save_document_as_action(
          qtils::createActionWithShortcut(QKeySequence::SaveAs, this)),
      m_save_all_documents_action(
          qtils::createActionWithShortcut(QKeySequence{}, this)),
      m_edit_menu(new QMenu(this)),
      m_cut_action(qtils::createActionWithShortcut(QKeySequence::Cut, this)),
      m_copy_action(qtils::createActionWithShortcut(QKeySequence::Copy, this)),
      m_paste_action(
          qtils::createActionWithShortcut(QKeySequence::Paste, this)),
      m_delete_action(
          qtils::createActionWithShortcut(QKeySequence::Delete, this)),
      m_view_menu(new QMenu(this)),
      m_views_and_toolbars_menu(new QMenu(this)),
      m_help_menu(new QMenu(this)) {
  auto undoGroup = pillar::DocumentManager::getInstance().getUndoGroup();
  m_undo_action = undoGroup->createUndoAction(this, tr("&Undo"));
  m_undo_action->setShortcut(QKeySequence::Undo);
  m_redo_action = undoGroup->createRedoAction(this, tr("&Redo"));
  m_redo_action->setShortcut(QKeySequence::Redo);

  registerActions();

  initUi();
  initConnections();

  updateActions();
  updateWindowTitle();
  updateRecentProjectFiles();

  retranslateUi();
}

ProjectWindow::~ProjectWindow() = default;

void ProjectWindow::closeEvent(QCloseEvent *event) {
  if (confirmAllSave()) {
    event->accept();
  } else {
    event->ignore();
  }
}

void ProjectWindow::changeEvent(QEvent *event) {
  QMainWindow::changeEvent(event);
  switch (event->type()) {
    case QEvent::LanguageChange:
      retranslateUi();
      break;
    default:
      break;
  }
}

void ProjectWindow::documentChanged(pillar::Document *document) {
  updateActions();
  updateRecentProjectFiles();
}

void ProjectWindow::projectChanged(pillar::Project *project) {
  m_project_dock->setProject(project);
  updateWindowTitle();
}

bool ProjectWindow::confirmSave(pillar::Document *document) {
  if (!document || !document->isModified()) return true;

  pillar::DocumentManager::getInstance().switchToDocument(document);

  auto ret = QMessageBox::warning(
      this, tr("Unsaved Changes"),
      tr("There are unsaved changes. Do you want to save now?"),
      QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

  switch (ret) {
    case QMessageBox::Save:
      return saveDocument(document);
    case QMessageBox::Discard:
      return true;
    case QMessageBox::Cancel:
    default:
      return false;
  }
}

bool ProjectWindow::confirmAllSave() {
  for (const auto &document :
       pillar::DocumentManager::getInstance().getDocuments())
    if (!confirmSave(document.get())) return false;

  return true;
}

void ProjectWindow::updateActions() {
  auto document_editor =
      pillar::DocumentManager::getInstance().getCurrentEditor();
  auto current_document =
      pillar::DocumentManager::getInstance().getCurrentDocument();

  pillar::DocumentEditor::StandardActions standard_actions;
  if (document_editor)
    standard_actions = document_editor->getEnabledStandardActions();

  m_close_document_action->setEnabled(current_document);
  m_save_document_action->setEnabled(current_document);
  m_save_document_as_action->setEnabled(current_document);
  m_save_all_documents_action->setEnabled(current_document);

  m_cut_action->setEnabled(standard_actions &
                           pillar::DocumentEditor::StandardAction::CutAction);
  m_copy_action->setEnabled(standard_actions &
                            pillar::DocumentEditor::StandardAction::CopyAction);
  m_paste_action->setEnabled(
      standard_actions & pillar::DocumentEditor::StandardAction::PasteAction);
  m_delete_action->setEnabled(
      standard_actions & pillar::DocumentEditor::StandardAction::DeleteAction);
}

void ProjectWindow::updateWindowTitle() {
  auto current_project =
      pillar::ProjectManager::getInstance().getCurrentProject();

  const auto project_name =
      current_project ? tr("[*]%1").arg(current_project->getDisplayName())
                      : QString();
  const auto project_file_path =
      current_project ? current_project->getFileName() : QString();

  auto project_is_modified = false;
  for (const auto &document :
       pillar::DocumentManager::getInstance().getDocuments())
    project_is_modified |= document->isModified();

  setWindowTitle(project_name);
  setWindowFilePath(project_file_path);
  setWindowModified(project_is_modified);

  Q_EMIT windowTitleChanged(project_name);
}

void ProjectWindow::updateViewsAndToolbarsMenu() {
  m_views_and_toolbars_menu->clear();

  m_views_and_toolbars_menu->addAction(m_project_dock->toggleViewAction());
  m_views_and_toolbars_menu->addAction(m_console_dock->toggleViewAction());
  m_views_and_toolbars_menu->addAction(m_issue_dock->toggleViewAction());

  if (auto editor = pillar::DocumentManager::getInstance().getCurrentEditor()) {
    m_views_and_toolbars_menu->addSeparator();

    auto dockWidgets = editor->getDockWidgets();
    auto dialogWidgets = editor->getDialogWidgets();

    for (auto &dockWidget : dockWidgets)
      m_views_and_toolbars_menu->addAction(dockWidget->toggleViewAction());

    m_views_and_toolbars_menu->addSeparator();

    for (auto &dialogWidget : dialogWidgets)
      m_views_and_toolbars_menu->addAction(dialogWidget->toggleViewAction());
  }

  adjustMenuSize(m_views_and_toolbars_menu);
}

void ProjectWindow::updateRecentProjectFiles() {
  auto recent_project_files =
      pillar::PreferencesManager::getInstance().getRecentProjectFiles();

  for (auto &action : m_open_recent_project_menu->actions()) {
    if (action != m_clear_recent_projects_action) action->deleteLater();
  }

  for (auto &recent_project_file : recent_project_files) {
    auto fileInfo = QFileInfo(recent_project_file);
    if (!fileInfo.exists()) continue;

    auto open_recent_project_action = qtils::createActionWithShortcut(
        QKeySequence{}, m_open_recent_project_menu);
    open_recent_project_action->setText(fileInfo.fileName());
    open_recent_project_action->setToolTip(fileInfo.filePath());
    open_recent_project_action->setVisible(true);

    m_open_recent_project_menu->insertAction(m_clear_recent_projects_action,
                                             open_recent_project_action);

    connect(
        open_recent_project_action, &QAction::triggered, this,
        [this, recent_project_file]() { openProject(recent_project_file); });
  }

  m_open_recent_project_menu->insertSeparator(m_clear_recent_projects_action);

  adjustMenuSize(m_open_recent_project_menu);
}

void ProjectWindow::newProject() {
  auto new_project_dialog =
      QScopedPointer<NewProjectDialog>(new NewProjectDialog(this));
  if (auto project = new_project_dialog->create(); project)
    switchProject(std::move(project));
}

void ProjectWindow::openProject() {
  const auto recent_project_files =
      pillar::PreferencesManager::getInstance().getRecentProjectFiles();
  const auto project_dir =
      recent_project_files.empty() ? QString{} : recent_project_files.last();
  const auto filter =
      pillar::FormatHelper<pillar::ProjectFormat>{
          pillar::FileFormat::Capability::Read}
          .getFilter();

  const auto file_name = qtils::QtExtendedFileDialog::getOpenFileName(
      this, tr("Open Project"), project_dir, filter);

  if (!file_name.isEmpty()) openProject(file_name);
}

bool ProjectWindow::openProject(const QString &file_name) {
  if (!pillar::ProjectManager::getInstance().switchToProject(file_name)) {
    QString error;
    auto project = pillar::Project::load(file_name, nullptr, &error);
    if (!project) {
      QMessageBox::critical(nullptr, tr("Error Opening File"), error);
      return false;
    }

    return switchProject(std::move(project));
  }

  return true;
}

bool ProjectWindow::closeProject() {
  if (!confirmAllSave()) return false;

  auto project = pillar::ProjectManager::getInstance().getCurrentProject();
  auto project_index =
      pillar::ProjectManager::getInstance().findProject(project);
  Q_ASSERT(project_index >= 0);

  pillar::DocumentManager::getInstance().removeAllDocuments();
  pillar::ProjectManager::getInstance().removeProject(project_index);

  return true;
}

void ProjectWindow::newDocument() {
  auto new_document_dialog =
      QScopedPointer<NewDocumentDialog>(new NewDocumentDialog);
  if (auto document = new_document_dialog->create(); document)
    pillar::DocumentManager::getInstance().addDocument(std::move(document));
}

void ProjectWindow::openDocument() {
  const auto project =
      pillar::ProjectManager::getInstance().getCurrentProject();
  const auto project_dir =
      QFileInfo(project->getFileName()).absoluteDir().absolutePath();
  const auto filter =
      pillar::FormatHelper<pillar::DocumentFormat>{
          pillar::FileFormat::Capability::Read}
          .getFilter();

  const auto file_name = qtils::QtExtendedFileDialog::getOpenFileName(
      this, tr("Open Document"), project_dir, filter);

  if (!file_name.isEmpty())
    pillar::DocumentManager::getInstance().loadDocument(file_name);
}

bool ProjectWindow::closeDocument(int index) {
  auto document = pillar::DocumentManager::getInstance().getDocument(index);
  if (document) {
    if (!confirmSave(document)) return false;

    pillar::DocumentManager::getInstance().removeDocument(index);
  }

  return true;
}

bool ProjectWindow::closeDocument() {
  auto current_document =
      pillar::DocumentManager::getInstance().getCurrentDocument();
  auto current_document_index =
      pillar::DocumentManager::getInstance().findDocument(current_document);

  return closeDocument(current_document_index);
}

bool ProjectWindow::saveDocument(
    pillar::Document
        *document)  // NOLINT(readability-convert-member-functions-to-static)
{
  Q_ASSERT(document);

  pillar::DocumentManager::getInstance().switchToDocument(document);

  if (!document->getFileName().isEmpty())
    return pillar::DocumentManager::getInstance().saveDocument(document);
  else
    return pillar::DocumentManager::getInstance().saveDocumentAs(document);
}

bool ProjectWindow::saveDocumentAs(
    pillar::Document
        *document)  // NOLINT(readability-convert-member-functions-to-static)
{
  Q_ASSERT(document);
  pillar::DocumentManager::getInstance().switchToDocument(document);
  return pillar::DocumentManager::getInstance().saveDocumentAs(document);
}

bool ProjectWindow::saveAllDocuments() {
  for (const auto &document :
       pillar::DocumentManager::getInstance().getDocuments())
    if (!saveDocument(document.get())) return false;

  return true;
}

void ProjectWindow::
    performCut()  // NOLINT(readability-convert-member-functions-to-static)
{
  if (auto editor = pillar::DocumentManager::getInstance().getCurrentEditor())
    editor->performStandardAction(
        pillar::DocumentEditor::StandardAction::CutAction);
}

void ProjectWindow::
    performCopy()  // NOLINT(readability-convert-member-functions-to-static)
{
  if (auto editor = pillar::DocumentManager::getInstance().getCurrentEditor())
    editor->performStandardAction(
        pillar::DocumentEditor::StandardAction::CopyAction);
}

void ProjectWindow::
    performPaste()  // NOLINT(readability-convert-member-functions-to-static)
{
  if (auto editor = pillar::DocumentManager::getInstance().getCurrentEditor())
    editor->performStandardAction(
        pillar::DocumentEditor::StandardAction::PasteAction);
}

void ProjectWindow::
    performDelete()  // NOLINT(readability-convert-member-functions-to-static)
{
  if (auto editor = pillar::DocumentManager::getInstance().getCurrentEditor())
    editor->performStandardAction(
        pillar::DocumentEditor::StandardAction::DeleteAction);
}

void ProjectWindow::writeSettings() {
  m_preferences->project_window_geometry = saveGeometry();
  m_preferences->project_window_state = saveState();

  pillar::DocumentManager::getInstance().saveState();
}

void ProjectWindow::readSettings() {
  auto window_geometry = m_preferences->project_window_geometry.get();
  auto window_state = m_preferences->project_window_state.get();

  if (!window_geometry.isNull()) restoreGeometry(window_geometry);
  if (!window_state.isNull()) restoreState(window_state);

  pillar::DocumentManager::getInstance().restoreState();
}

void ProjectWindow::registerActions() {
  auto &action_manager = pillar::ActionManager::getInstance();

  action_manager.registerAction(m_new_project_action, "new_project");
  action_manager.registerAction(m_open_project_action, "open_project");
  action_manager.registerAction(m_clear_recent_projects_action,
                                "clear_recent_projects");
  action_manager.registerAction(m_close_project_action, "close_project");
  action_manager.registerAction(m_new_document_action, "new_document");
  action_manager.registerAction(m_open_document_action, "open_document");
  action_manager.registerAction(m_close_document_action, "close_document");
  action_manager.registerAction(m_save_document_action, "save_document");
  action_manager.registerAction(m_save_document_as_action, "save_document_as");
  action_manager.registerAction(m_save_all_documents_action,
                                "save_all_documents");
  action_manager.registerAction(m_undo_action, "undo");
  action_manager.registerAction(m_redo_action, "redo");
  action_manager.registerAction(m_cut_action, "cut");
  action_manager.registerAction(m_copy_action, "copy");
  action_manager.registerAction(m_paste_action, "paste");
  action_manager.registerAction(m_delete_action, "delete");
}

void ProjectWindow::initUi() {
  m_ui->setupUi(this);

  setCentralWidget(pillar::DocumentManager::getInstance().getWidget());
  addDockWidget(Qt::LeftDockWidgetArea, m_project_dock);
  addDockWidget(Qt::BottomDockWidgetArea, m_console_dock);
  addDockWidget(Qt::BottomDockWidgetArea, m_issue_dock);
  tabifyDockWidget(m_console_dock, m_issue_dock);
  m_console_dock->raise();

  m_ui->m_menu_bar->addMenu(m_project_menu);
  m_project_menu->addAction(m_new_project_action);
  m_project_menu->addAction(m_open_project_action);
  m_project_menu->addMenu(m_open_recent_project_menu);
  m_open_recent_project_menu->addSeparator();
  m_open_recent_project_menu->addAction(m_clear_recent_projects_action);
  m_project_menu->addAction(m_close_project_action);
  m_project_menu->addSeparator();
  m_project_menu->addAction(
      pillar::ActionManager::getInstance().findAction("settings"));
  m_project_menu->addSeparator();
  m_project_menu->addAction(
      pillar::ActionManager::getInstance().findAction("exit"));

  m_ui->m_menu_bar->addMenu(m_document_menu);
  m_document_menu->addAction(m_new_document_action);
  m_document_menu->addAction(m_open_document_action);
  m_document_menu->addAction(m_close_document_action);
  m_document_menu->addSeparator();
  m_document_menu->addAction(m_save_document_action);
  m_document_menu->addAction(m_save_document_as_action);
  m_document_menu->addAction(m_save_all_documents_action);

  m_ui->m_menu_bar->addMenu(m_edit_menu);
  m_edit_menu->addAction(m_undo_action);
  m_edit_menu->addAction(m_redo_action);
  m_edit_menu->addSeparator();
  m_edit_menu->addAction(m_cut_action);
  m_edit_menu->addAction(m_copy_action);
  m_edit_menu->addAction(m_paste_action);
  m_edit_menu->addAction(m_delete_action);

  m_ui->m_menu_bar->addMenu(m_view_menu);
  m_view_menu->addMenu(m_views_and_toolbars_menu);

  m_ui->m_menu_bar->addMenu(m_help_menu);
  m_help_menu->addAction(
      pillar::ActionManager::getInstance().findAction("about"));

  m_help_menu->setShortcutEnabled(true);
}

void ProjectWindow::initConnections() {
  auto undoGroup = pillar::DocumentManager::getInstance().getUndoGroup();

  connect(undoGroup, &QUndoGroup::cleanChanged, this,
          &ProjectWindow::updateWindowTitle);

  connect(m_new_project_action, &QAction::triggered, this,
          &ProjectWindow::newProject);
  connect(m_open_project_action, &QAction::triggered, this,
          qOverload<>(&ProjectWindow::openProject));
  connect(m_clear_recent_projects_action, &QAction::triggered, this, []() {
    pillar::PreferencesManager::getInstance().clearRecentProjectFiles();
  });
  connect(m_close_project_action, &QAction::triggered, this,
          &ProjectWindow::closeProject);

  connect(m_new_document_action, &QAction::triggered, this,
          &ProjectWindow::newDocument);
  connect(m_open_document_action, &QAction::triggered, this,
          qOverload<>(&ProjectWindow::openDocument));
  connect(m_close_document_action, &QAction::triggered, this,
          qOverload<>(&ProjectWindow::closeDocument));
  connect(m_save_document_action, &QAction::triggered, this, [this]() {
    saveDocument(pillar::DocumentManager::getInstance().getCurrentDocument());
  });
  connect(m_save_document_as_action, &QAction::triggered, this, [this]() {
    saveDocumentAs(pillar::DocumentManager::getInstance().getCurrentDocument());
  });
  connect(m_save_all_documents_action, &QAction::triggered, this,
          &ProjectWindow::saveAllDocuments);

  connect(m_cut_action, &QAction::triggered, this, &ProjectWindow::performCut);
  connect(m_copy_action, &QAction::triggered, this,
          &ProjectWindow::performCopy);
  connect(m_paste_action, &QAction::triggered, this,
          &ProjectWindow::performPaste);
  connect(m_delete_action, &QAction::triggered, this,
          &ProjectWindow::performDelete);

  connect(m_views_and_toolbars_menu, &QMenu::aboutToShow, this,
          &ProjectWindow::updateViewsAndToolbarsMenu);

  connect(&pillar::DocumentManager::getInstance(),
          &pillar::DocumentManager::currentDocumentChanged, this,
          &ProjectWindow::documentChanged);
  connect(&pillar::DocumentManager::getInstance(),
          &pillar::DocumentManager::documentCloseRequested, this,
          qOverload<int>(&ProjectWindow::closeDocument));
  connect(&pillar::DocumentManager::getInstance(),
          &pillar::DocumentManager::enabledStandardActionsChanged, this,
          &ProjectWindow::updateActions);

  connect(&pillar::ProjectManager::getInstance(),
          &pillar::ProjectManager::currentProjectChanged, this,
          &ProjectWindow::projectChanged);

  connect(&pillar::PreferencesManager::getInstance(),
          &pillar::PreferencesManager::recentProjectFilesChanged, this,
          &ProjectWindow::updateRecentProjectFiles);
}

void ProjectWindow::retranslateUi() {
  m_ui->retranslateUi(this);

  m_project_menu->setTitle(tr("&Project"));
  m_new_project_action->setText(tr("&New..."));
  m_new_project_action->setWhatsThis(tr("New Project"));
  m_open_project_action->setText(tr("&Open..."));
  m_open_project_action->setWhatsThis(tr("Open Project"));
  m_open_recent_project_menu->setTitle(tr("Open &Recent"));
  m_clear_recent_projects_action->setText(tr("Clear &Recent"));
  m_clear_recent_projects_action->setWhatsThis(tr("Clear Recent Project"));
  m_close_project_action->setText(tr("&Close Project"));
  m_close_project_action->setWhatsThis(tr("Close Project"));

  m_document_menu->setTitle(tr("&Document"));
  m_new_document_action->setText(tr("&New..."));
  m_new_document_action->setWhatsThis(tr("New Document"));
  m_open_document_action->setText(tr("&Open..."));
  m_open_document_action->setWhatsThis(tr("Open Document"));
  m_close_document_action->setText(tr("&Close Document"));
  m_close_document_action->setWhatsThis(tr("Close Document"));
  m_save_document_action->setText(tr("&Save"));
  m_save_document_action->setWhatsThis(tr("Save Document"));
  m_save_document_as_action->setText(tr("Save &As"));
  m_save_document_as_action->setWhatsThis(tr("Save Document As"));
  m_save_all_documents_action->setText(tr("Save All"));
  m_save_all_documents_action->setWhatsThis(tr("Save All Documents"));

  m_edit_menu->setTitle(tr("&Edit"));
  m_undo_action->setText(tr("&Undo"));
  m_undo_action->setWhatsThis(tr("Undo"));
  m_redo_action->setText(tr("&Redo"));
  m_redo_action->setWhatsThis(tr("Redo"));
  m_cut_action->setText(tr("&Cut"));
  m_cut_action->setWhatsThis(tr("Cut"));
  m_copy_action->setText(tr("&Copy"));
  m_copy_action->setWhatsThis(tr("Copy"));
  m_paste_action->setText(tr("&Paste"));
  m_paste_action->setWhatsThis(tr("Paste"));
  m_delete_action->setText(tr("&Delete"));
  m_delete_action->setWhatsThis(tr("Delete"));

  m_view_menu->setTitle(tr("&View"));
  m_views_and_toolbars_menu->setTitle(tr("Views and &Toolbars"));

  m_help_menu->setTitle(tr("&Help"));

  adjustMenuSize(this);
}

bool ProjectWindow::switchProject(std::unique_ptr<pillar::Project> project) {
  auto ret = QMessageBox::warning(this, tr("Open Project"),
                                  tr("There is currently open project. Do you "
                                     "want to close it and open new one now?"),
                                  QMessageBox::Yes | QMessageBox::Cancel);

  if (ret == QMessageBox::Yes && closeProject()) {
    auto project_ptr = project.get();

    pillar::ProjectManager::getInstance().removeAllProjects();
    pillar::ProjectManager::getInstance().addProject(std::move(project));
    pillar::PreferencesManager::getInstance().addRecentProjectFile(
        project_ptr->getFileName());
    return true;
  }

  return false;
}
