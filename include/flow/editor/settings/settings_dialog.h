#ifndef FLOW_SETTINGS_DIALOG_H
#define FLOW_SETTINGS_DIALOG_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QSortFilterProxyModel>
/* ----------------------------------- Utils -------------------------------- */
#include <flow/utils/dialog/dialog_with_url_links.h>
/* -------------------------------------------------------------------------- */

namespace Ui {
class SettingsDialog;
}

namespace utils {
class QtStackedLayout;
}

class SettingsWidgetTreeModel;

class SettingsDialog : public utils::QtDialogWithUrlLinks<SettingsDialog> {
  Q_OBJECT

 public:
  explicit SettingsDialog(QWidget *parent = nullptr);
  ~SettingsDialog() override;

 public Q_SLOTS:
  void setUrl(const QUrl &url) override;

 protected:
  void closeEvent(QCloseEvent *event) override;
  void changeEvent(QEvent *event) override;

 private Q_SLOTS:
  void filterSettings(const QString &filter);

  void ok();
  void apply();
  void cancel();

  void currentChanged(QWidget *widget);

 private:
  void initUi();
  void initConnections();

  void retranslateUi();

 private:
  QScopedPointer<Ui::SettingsDialog> m_ui;

  QScopedPointer<SettingsWidgetTreeModel> m_settings_widget_model;
  QScopedPointer<QSortFilterProxyModel> m_settings_widget_filter_model;
};

#endif  // FLOW_SETTINGS_DIALOG_H
