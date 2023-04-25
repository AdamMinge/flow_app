/* ----------------------------------- Local -------------------------------- */
#include "egnite/document/document_manager.h"

#include "egnite/document/document.h"
#include "egnite/document/document_editor.h"
#include "egnite/document/document_format.h"
#include "egnite/format_helper.h"
/* ------------------------------------ Qt ---------------------------------- */
#include <QCoreApplication>
#include <QMessageBox>
/* ----------------------------------- Utils -------------------------------- */
#include <utils/dialog/extended_file_dialog.h>
#include <utils/file_system/file_system_watcher.h>
/* -------------------------------------------------------------------------- */

namespace egnite {

QScopedPointer<DocumentManager> DocumentManager::m_instance =
    QScopedPointer<DocumentManager>(nullptr);

DocumentManager &DocumentManager::getInstance() {
  if (m_instance.isNull()) m_instance.reset(new DocumentManager);

  return *m_instance;
}

void DocumentManager::deleteInstance() { m_instance.reset(nullptr); }

DocumentManager::DocumentManager()
    : m_widget(new QWidget()),
      m_tab_bar(new QTabBar(m_widget.data())),
      m_editor_stack(new QStackedLayout()),
      m_no_document_widget(new QWidget(m_widget)),
      m_file_system_watcher(new utils::QtFileSystemWatcher()),
      m_undo_group(new QUndoGroup(this)) {
  m_tab_bar->setExpanding(false);
  m_tab_bar->setDocumentMode(true);
  m_tab_bar->setTabsClosable(true);
  m_tab_bar->setMovable(true);
  m_tab_bar->setContextMenuPolicy(Qt::CustomContextMenu);

  auto vertical_layout = new QVBoxLayout(m_widget.data());
  vertical_layout->addWidget(m_tab_bar);
  vertical_layout->setContentsMargins(0, 0, 0, 0);
  vertical_layout->setSpacing(0);
  vertical_layout->addLayout(m_editor_stack);

  m_editor_stack->addWidget(m_no_document_widget);

  connect(m_tab_bar, &QTabBar::currentChanged, this,
          &DocumentManager::currentIndexChanged);
  connect(m_tab_bar, &QTabBar::tabMoved, this,
          &DocumentManager::documentTabMoved);
  connect(m_tab_bar, &QTabBar::tabCloseRequested, this,
          &DocumentManager::documentCloseRequested);

  connect(m_file_system_watcher.get(),
          &utils::QtFileSystemWatcher::pathsChanged, this,
          &DocumentManager::filesChanged);

  loadObjects();
}

DocumentManager::~DocumentManager() = default;

QWidget *DocumentManager::getWidget() const { return m_widget.data(); }

void DocumentManager::addEditor(DocumentEditor *editor) {
  Q_ASSERT(!m_editor_for_document_id.contains(editor->getDocumentId()));
  m_editor_stack->addWidget(editor->getEditorWidget());
  m_editor_for_document_id.insert(
      std::make_pair(editor->getDocumentId(), editor));
}

void DocumentManager::removeEditor(const QString &document_id) {
  auto documents_to_remove = std::vector<Document *>{};
  std::transform(m_documents.begin(), m_documents.end(),
                 std::back_inserter(documents_to_remove),
                 [](auto &ptr_document) { return ptr_document.get(); });

  for (auto document_to_remove : documents_to_remove) {
    if (document_to_remove->getId() == document_id)
      removeDocument(findDocument(document_to_remove));
  }

  Q_ASSERT(m_editor_for_document_id.contains(document_id));
  m_editor_for_document_id.erase(document_id);
}

void DocumentManager::removeAllEditors() {
  while (!m_editor_for_document_id.empty())
    removeEditor(m_editor_for_document_id.begin()->first);
}

DocumentEditor *DocumentManager::getEditor(const QString &document_id) const {
  if (m_editor_for_document_id.contains(document_id))
    return m_editor_for_document_id.at(document_id);

  return nullptr;
}

DocumentEditor *DocumentManager::getCurrentEditor() const {
  auto current_document = getCurrentDocument();
  return current_document ? getEditor(current_document->getId()) : nullptr;
}

void DocumentManager::addDocument(std::unique_ptr<Document> document) {
  insertDocument(static_cast<int>(m_documents.size()), std::move(document));
}

void DocumentManager::insertDocument(int index,
                                     std::unique_ptr<Document> document) {
  Q_ASSERT(document);
  auto document_ptr = document.get();
  m_documents.insert(m_documents.begin() + index, std::move(document));
  m_undo_group->addStack(document_ptr->getUndoStack());

  if (!document_ptr->getFileName().isEmpty())
    m_file_system_watcher->addPath(document_ptr->getFileName());

  if (auto editor = getEditor(document_ptr->getId()); editor)
    editor->addDocument(document_ptr);

  auto tab_text = document_ptr->getDisplayName();
  if (document_ptr->isModified()) tab_text.prepend(QLatin1Char('*'));

  auto document_index = m_tab_bar->insertTab(index, tab_text);
  m_tab_bar->setTabToolTip(document_index, document_ptr->getFileName());

  connect(document_ptr, &Document::fileNameChanged, this,
          &DocumentManager::fileNameChanged);
  connect(document_ptr, &Document::modifiedChanged, this,
          [this, document_ptr]() { updateDocumentTab(document_ptr); });

  switchToDocument(document_index);
}

bool DocumentManager::reloadDocumentAt(int index) {
  const auto &document = m_documents.at(index);

  QString error;
  auto new_document = Document::load(document->getFileName(), nullptr, &error);
  if (!new_document) {
    QMessageBox::critical(m_widget.get(), tr("Error Reloading File"), error);

    document->setFileName(QString{});
    return false;
  }

  removeDocument(index);
  insertDocument(index, std::move(new_document));

  return true;
}

bool DocumentManager::loadDocument(const QString &file_name) {
  if (switchToDocument(file_name)) return true;

  QString error;
  auto document = Document::load(file_name, nullptr, &error);
  if (!document) {
    QMessageBox::critical(m_widget.get(), tr("Error Opening File"), error);
    return false;
  }

  addDocument(std::move(document));
  return true;
}

void DocumentManager::removeDocument(int index) {
  auto document_to_remove = std::move(m_documents[index]);
  Q_ASSERT(document_to_remove);

  if (auto editor = getEditor(document_to_remove->getId()); editor)
    editor->removeDocument(document_to_remove.get());

  if (!document_to_remove->getFileName().isEmpty())
    m_file_system_watcher->removePath(document_to_remove->getFileName());

  std::erase(m_documents, nullptr);
  m_undo_group->removeStack(document_to_remove->getUndoStack());

  m_tab_bar->removeTab(index);
}

void DocumentManager::removeAllDocuments() {
  while (!m_documents.empty()) removeDocument(m_tab_bar->currentIndex());
}

Document *DocumentManager::getDocument(int index) const {
  if (index < m_documents.size() && index >= 0)
    return m_documents.at(index).get();

  return nullptr;
}

Document *DocumentManager::getCurrentDocument() const {
  auto index = m_tab_bar->currentIndex();
  return getDocument(index);
}

int DocumentManager::findDocument(Document *document) const {
  auto found = std::find_if(m_documents.begin(), m_documents.end(),
                            [document](auto &current_document) {
                              return current_document.get() == document;
                            });

  if (found == m_documents.end()) return -1;

  return static_cast<int>(std::distance(m_documents.begin(), found));
}

int DocumentManager::findDocument(const QString &file_name) const {
  auto found =
      std::find_if(m_documents.begin(), m_documents.end(),
                   [&file_name](auto &current_document) {
                     return current_document->getFileName() == file_name;
                   });

  if (found == m_documents.end()) return -1;

  return static_cast<int>(std::distance(m_documents.begin(), found));
}

void DocumentManager::switchToDocument(int index) {
  m_tab_bar->setCurrentIndex(index);
}

void DocumentManager::switchToDocument(Document *document) {
  auto found_iter = std::find_if(m_documents.begin(), m_documents.end(),
                                 [&document](auto &&current_document) {
                                   return current_document.get() == document;
                                 });

  Q_ASSERT(found_iter != m_documents.end());
  auto index = std::distance(m_documents.begin(), found_iter);
  switchToDocument(static_cast<int>(index));
}

bool DocumentManager::switchToDocument(const QString &file_name) {
  auto found_iter =
      std::find_if(m_documents.begin(), m_documents.end(),
                   [&file_name](auto &&current_document) {
                     return current_document->getFileName() == file_name;
                   });

  if (found_iter == m_documents.end()) return false;

  auto index = std::distance(m_documents.begin(), found_iter);
  switchToDocument(static_cast<int>(index));
  return true;
}

QUndoGroup *DocumentManager::getUndoGroup() const { return m_undo_group; }

void DocumentManager::saveState() {
  for (auto &[document_type, editor] : m_editor_for_document_id)
    editor->saveState();
}

void DocumentManager::restoreState() {
  for (auto &[document_type, editor] : m_editor_for_document_id)
    editor->restoreState();
}

bool DocumentManager::saveDocument(Document *document) {
  Q_ASSERT(document);

  QString error;
  if (!document->save(document->getFileName(), &error)) {
    switchToDocument(document);
    QMessageBox::critical(m_widget->window(), tr("Error Saving File"), error);
    return false;
  }

  return true;
}

bool DocumentManager::saveDocumentAs(Document *document) {
  Q_ASSERT(document);

  auto selectedFilter = QString{};
  if (auto format = document->getWriterFormat(); format)
    selectedFilter = format->getNameFilter();

  auto format_helper =
      FormatHelper<DocumentFormat>{FileFormat::Capability::Write};
  const auto file_name = utils::QtExtendedFileDialog::getSaveFileName(
      m_widget->window(), tr("Save Document As"), document->getFileName(),
      format_helper.getFilter(), &selectedFilter);

  if (file_name.isEmpty()) return false;

  auto format = format_helper.findFormatByNameFilter(selectedFilter);
  Q_ASSERT(format);

  document->setWriterFormat(format);
  document->setWriterFormat(format);

  QString error;
  if (!document->save(file_name, &error)) {
    switchToDocument(document);
    QMessageBox::critical(m_widget->window(), tr("Error Saving File"), error);
    return false;
  }

  return true;
}

const std::vector<std::unique_ptr<Document>> &DocumentManager::getDocuments()
    const {
  return m_documents;
}

void DocumentManager::addedObject(DocumentEditor *object) { addEditor(object); }

void DocumentManager::removedObject(DocumentEditor *object) {
  removeEditor(object->getDocumentId());
}

void DocumentManager::currentIndexChanged() {
  auto document = getCurrentDocument();
  auto editor = getCurrentEditor();

  if (document) m_undo_group->setActiveStack(document->getUndoStack());

  if (editor) {
    editor->setCurrentDocument(document);
    m_editor_stack->setCurrentWidget(editor->getEditorWidget());

    disconnect(this, SIGNAL(enabledStandardActionsChanged()));

    connect(editor, &DocumentEditor::enabledStandardActionsChanged, this,
            &DocumentManager::enabledStandardActionsChanged);

  } else {
    m_editor_stack->setCurrentWidget(m_no_document_widget);
  }

  Q_EMIT currentDocumentChanged(document);
}

void DocumentManager::documentTabMoved(int from, int to) {
  if (from > to)
    std::rotate(m_documents.rend() - from - 1, m_documents.rend() - from,
                m_documents.rend() - to);
  else
    std::rotate(m_documents.begin() + from, m_documents.begin() + from + 1,
                m_documents.begin() + to + 1);
}

void DocumentManager::filesChanged(const QStringList &file_names) {
  for (const auto &file_name : file_names) {
    const auto index = findDocument(file_name);
    if (index == -1) return;

    const auto &document = m_documents.at(index);
    if (QFileInfo(file_name).lastModified() == document->getLastModified())
      continue;

    if (!document->isModified()) reloadDocumentAt(index);
  }
}

void DocumentManager::fileNameChanged(const QString &new_file_name,
                                      const QString &old_file_name) {
  if (!new_file_name.isEmpty()) m_file_system_watcher->addPath(new_file_name);
  if (!old_file_name.isEmpty())
    m_file_system_watcher->removePath(old_file_name);

  auto document = dynamic_cast<Document *>(sender());
  Q_ASSERT(document);

  updateDocumentTab(document);
}

void DocumentManager::updateDocumentTab(Document *document) {
  const auto index = findDocument(document);
  Q_ASSERT(document && index != -1);

  auto tab_text = document->getDisplayName();
  if (document->isModified()) tab_text.prepend(QLatin1Char('*'));

  m_tab_bar->setTabText(index, tab_text);
  m_tab_bar->setTabToolTip(index, document->getFileName());
}

}  // namespace egnite
