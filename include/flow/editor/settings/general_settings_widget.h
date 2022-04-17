#ifndef FLOW_GENERAL_SETTINGS_WIDGET_H
#define FLOW_GENERAL_SETTINGS_WIDGET_H

/* ----------------------------------- Local -------------------------------- */
#include "flow/editor/settings/settings_widget.h"
/* -------------------------------------------------------------------------- */

namespace Ui
{
  class GeneralSettingsWidget;
}

class GeneralSettingsWidget : public SettingsWidget
{
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

#endif//FLOW_GENERAL_SETTINGS_WIDGET_H