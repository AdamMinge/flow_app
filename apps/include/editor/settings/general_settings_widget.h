#ifndef GENERAL_SETTINGS_WIDGET_H
#define GENERAL_SETTINGS_WIDGET_H

/* ---------------------------------- Pillar -------------------------------- */
#include <pillar/settings/settings_widget.h>
#include <pillar/settings/settings_widget_factory.h>
/* -------------------------------------------------------------------------- */

namespace Ui {
class GeneralSettingsWidget;
}

class GeneralSettingsWidget : public pillar::SettingsWidget {
  Q_OBJECT

 public:
  explicit GeneralSettingsWidget(QWidget *parent = nullptr);
  ~GeneralSettingsWidget() override;

 protected:
  void changeEvent(QEvent *event) override;

 private:
  void initUi();
  void initConnections();

  void retranslateUi();

 private:
  QScopedPointer<Ui::GeneralSettingsWidget> m_ui;
};

class GeneralSettingsWidgetFactory : public pillar::SettingsWidgetFactory {
  Q_OBJECT
  Q_INTERFACES(pillar::SettingsWidgetFactory)

 public:
  explicit GeneralSettingsWidgetFactory(QObject *parent = nullptr);
  ~GeneralSettingsWidgetFactory() override;

  [[nodiscard]] std::unique_ptr<pillar::SettingsWidget> create() const override;
};

#endif  // GENERAL_SETTINGS_WIDGET_H
