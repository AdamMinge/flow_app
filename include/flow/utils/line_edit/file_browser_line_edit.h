#ifndef UTILS_LINE_EDIT_FILE_BROWSER_LINE_EDIT_H
#define UTILS_LINE_EDIT_FILE_BROWSER_LINE_EDIT_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QLineEdit>
#include <QStringList>
/* ----------------------------------- Local -------------------------------- */
#include "flow/utils/line_edit/export.h"
/* -------------------------------------------------------------------------- */

namespace utils {

class LINE_EDIT_API QtFileBrowserLineEdit : public QLineEdit {
  Q_OBJECT

 public:
  explicit QtFileBrowserLineEdit(QWidget *parent = nullptr);
  ~QtFileBrowserLineEdit() override;

  void setBrowserCaption(const QString &caption);
  void setBrowserDir(const QString &dir);
  void setBrowserFilter(const QString &filter);

  [[nodiscard]] QString getBrowserCaption() const;
  [[nodiscard]] QString getBrowserDir() const;
  [[nodiscard]] QString getBrowserFilter() const;

 Q_SIGNALS:
  void browserCaptionChanged(const QString &caption);
  void browserDirChanged(const QString &dir);
  void browserFilterChanged(const QString &filter);

 protected Q_SLOTS:
  void browserPressed();

 private:
  QString m_browser_caption;
  QString m_browser_dir;
  QString m_browser_filter;
};

}  // namespace utils

#endif  // UTILS_LINE_EDIT_FILE_BROWSER_LINE_EDIT_H
