#ifndef UTILS_DIALOG_EXTENDED_FILE_DIALOG_H
#define UTILS_DIALOG_EXTENDED_FILE_DIALOG_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QFileDialog>
/* ----------------------------------- Local -------------------------------- */
#include "utils/dialog/export.h"
/* -------------------------------------------------------------------------- */

namespace utils {

class DIALOG_API QtExtendedFileDialog : public QFileDialog {
  Q_OBJECT

 public:
  explicit QtExtendedFileDialog(QWidget *parent, Qt::WindowFlags flags);
  explicit QtExtendedFileDialog(QWidget *parent = nullptr,
                                const QString &caption = QString(),
                                const QString &directory = QString(),
                                const QString &filter = QString());
  ~QtExtendedFileDialog() override;

  static QString getExistingDirectory(
      QWidget *parent = nullptr, const QString &caption = QString(),
      const QString &dir = QString(),
      QFileDialog::Options options = ShowDirsOnly,
      const QString &top_dir = QString());

  static QUrl getExistingDirectoryUrl(
      QWidget *parent = nullptr, const QString &caption = QString(),
      const QUrl &dir = QUrl(), QFileDialog::Options options = ShowDirsOnly,
      const QStringList &supportedSchemes = QStringList(),
      const QString &top_dir = QString());

  void setTopDir(const QString &path);
  [[nodiscard]] QString getTopDir() const;

 public:
  bool eventFilter(QObject *object, QEvent *event) override;

 protected:
  explicit QtExtendedFileDialog(const QFileDialogArgs &args);
  void init();

 private:
  [[nodiscard]] bool pathFits(const QString &path, bool exact = false) const;

 private Q_SLOTS:
  void checkHistory();
  void checkGoToParentAndBack();
  void checkLineEdit(const QString &text);
  void checkComboBox(int index);

 private:
  QString m_top_dir;
};

}  // namespace utils

#endif  // UTILS_DIALOG_EXTENDED_FILE_DIALOG_H
