/* ----------------------------------- Local -------------------------------- */
#include "settings/general_settings_widget.h"
/* ------------------------------------ Qt ---------------------------------- */
#include <QEvent>
/* ----------------------------------- Qtils -------------------------------- */
#include <qtils/dpi/dpi.h>
/* ------------------------------------ Ui ---------------------------------- */
#include "settings/ui_general_settings_widget.h"
/* -------------------------------------------------------------------------- */

/* --------------------------- GeneralSettingsWidget ------------------------ */

GeneralSettingsWidget::GeneralSettingsWidget(QWidget *parent)
    : SettingsWidget(parent), m_ui(new Ui::GeneralSettingsWidget()) {
  initUi();
  initConnections();

  retranslateUi();
}

GeneralSettingsWidget::~GeneralSettingsWidget() = default;

void GeneralSettingsWidget::changeEvent(QEvent *event) {
  QWidget::changeEvent(event);

  switch (event->type()) {
    case QEvent::LanguageChange:
      retranslateUi();
      break;
    default:
      break;
  }
}

void GeneralSettingsWidget::initUi() { m_ui->setupUi(this); }

void GeneralSettingsWidget::initConnections() {}

void GeneralSettingsWidget::retranslateUi() {
  m_ui->retranslateUi(this);

  const auto link_color = palette().color(QPalette::Link).name(QColor::HexRgb);
  const auto link_text =
      QString(
          "<li style=\" margin-bottom:5px \"><a href=\"%2\"><span "
          "style=\" text-decoration: none; "
          "color:%1;\">%3</span></a></li>")
          .arg(link_color);

  auto label_text =
      QString(
          "<html>"
          "<head/>"
          "<body>"
          "Customize appearance and behavior: change themes and change "
          "language."
          "<ul style=\" list-style-type:none\">%2</ul>"
          "</body>"
          "</html>")
          .arg(QString{link_text.arg("settings:AppearanceSettingsWidget",
                                     "Appearance")});

  m_ui->m_general_label->setText(label_text);
}

/* ------------------------ GeneralSettingsWidgetFactory -------------------- */

GeneralSettingsWidgetFactory::GeneralSettingsWidgetFactory(QObject *parent)
    : pillar::SettingsWidgetFactory(parent) {}

GeneralSettingsWidgetFactory::~GeneralSettingsWidgetFactory() = default;

std::unique_ptr<pillar::SettingsWidget> GeneralSettingsWidgetFactory::create()
    const {
  return std::make_unique<GeneralSettingsWidget>();
}
