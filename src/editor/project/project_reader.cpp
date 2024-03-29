/* ----------------------------------- Local -------------------------------- */
#include "project/project_reader.h"
/* ------------------------------------ Qt ---------------------------------- */
#include <QFile>
/* ---------------------------------- Egnite -------------------------------- */
#include <egnite/project/project.h>
/* ----------------------------------- Utils -------------------------------- */
#include <utils/pointer_cast/unique_ptr_cast.h>
#include <utils/serializer/json_archive.h>
/* -------------------------------------------------------------------------- */

/* ----------------------------- ProjectReaderImpl -------------------------- */

class ProjectReader::ProjectReaderImpl {
 public:
  explicit ProjectReaderImpl() = default;
  ~ProjectReaderImpl() = default;

  std::unique_ptr<egnite::Project> readProject(QIODevice &device);
  bool isValid(QIODevice &device);
};

std::unique_ptr<egnite::Project> ProjectReader::ProjectReaderImpl::readProject(
    QIODevice &device) {
  auto project =
      utils::cast_unique_ptr<egnite::Project>(egnite::Project::create());

  utils::IJsonArchive archive(device);
  archive >> utils::ArchiveProperty("project", project);

  return project;
}

bool ProjectReader::ProjectReaderImpl::isValid(QIODevice &device) {
  return true;
}

/* ------------------------------- ProjectReader ---------------------------- */

ProjectReader::ProjectReader()
    : m_impl(std::make_unique<ProjectReaderImpl>()) {}

ProjectReader::~ProjectReader() = default;

std::unique_ptr<egnite::Project> ProjectReader::read(QIODevice &device,
                                                     QString *error) {
  auto project = m_impl->readProject(device);
  if (!project && error) *error = QObject::tr("Failed to load project");

  return project;
}

std::unique_ptr<egnite::Project> ProjectReader::read(const QString &file_name,
                                                     QString *error) {
  QFile file(file_name);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    if (error) *error = QObject::tr("Failed to open file = %1").arg(file_name);
    return nullptr;
  }

  auto project = m_impl->readProject(file);
  if (!project && error) *error = QObject::tr("Failed to load project");

  return project;
}

bool ProjectReader::isValid(const QString &file_name) {
  QFile file(file_name);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
  return m_impl->isValid(file);
}

bool ProjectReader::isValid(QIODevice &device) {
  return m_impl->isValid(device);
}
