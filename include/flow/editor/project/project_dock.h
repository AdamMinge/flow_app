#ifndef FLOW_PROJECT_DOCK_H
#define FLOW_PROJECT_DOCK_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QDockWidget>
#include <QScopedPointer>
/* -------------------------------------------------------------------------- */

class QAbstractFileIconProvider;
class QFileSystemModel;
class QTreeView;

namespace api
{
  class IProject;
}

namespace utils
{
  class FileSystemProxyModel;
}

class ProjectDock : public QDockWidget
{
  Q_OBJECT

public:
  explicit ProjectDock(QWidget *parent = nullptr);
  ~ProjectDock() override;

  void setProject(api::IProject *project);
  [[nodiscard]] api::IProject *getProject() const;

protected:
  void changeEvent(QEvent *event) override;

private:
  void initUi();
  void initConnections();

  void retranslateUi();

private:
  api::IProject *m_current_project;

  QTreeView *m_view;
  QFileSystemModel *m_model;
  utils::FileSystemProxyModel *m_proxy;

  QScopedPointer<QAbstractFileIconProvider> m_icon_provider;
};

#endif//FLOW_PROJECT_DOCK_H
