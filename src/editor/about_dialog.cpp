/* ----------------------------------- Local -------------------------------- */
#include "about_dialog.h"

#include "../config.h"
/* ------------------------------------ Ui ---------------------------------- */
#include "ui_about_dialog.h"
/* -------------------------------------------------------------------------- */

AboutDialog::AboutDialog(QWidget *parent)
    : utils::QtDialogWithUrlLinks<AboutDialog>(parent),
      m_ui(new Ui::AboutDialog()) {
  m_ui->setupUi(this);

  retranslateUi();
}

AboutDialog::~AboutDialog() = default;

void AboutDialog::changeEvent(QEvent *event) {
  QDialog::changeEvent(event);

  switch (event->type()) {
    case QEvent::LanguageChange:
      retranslateUi();
      break;
    default:
      break;
  }
}

void AboutDialog::retranslateUi() {
  m_ui->retranslateUi(this);

  setWindowTitle(tr("About"));

  auto about_text =
      QString{
          "<html>"
          "<head/>"
          "<body>"
          "<p align=\"center\"><span style=\" font-size:48pt; "
          "font-weight:700;\">Egnite</span></p>"
          "<p align=\"center\"><span style=\" color:#555753;\">%1</span></p>"
          "<p align=\"center\"><span style=\" font-size:12pt;\">%2</span></p>"
          "<p align=\"center\"><span style=\" font-size:12pt;\">%3 </span><a "
          "href=\"%4\"><span style=\" font-size:12pt; "
          "text-decoration: underline; color:#0000ff;\">%5</span></a> <span "
          "style=\" font-size:12pt;\"><br/> %6</span></p>"
          "<p align=\"center\"><span style=\" "
          "color:#555753;\">%7</span><br/></p>"
          "</body>"
          "</html>"}
          .arg(tr("Version %1").arg(EGNITE_VERSION_STR),
               tr("This Project is open source and <br/> contributions are "
                  "welcomed."),
               tr("Visit"), tr("https://github.com/AdamMinge/flow_app"),
               tr("GitHub/AdamMinge/flow_app"),
               tr("for more information or to report a bug <br> or to suggest "
                  "a new "
                  "feature."),
               tr("Copyright © 2023 Adam Minge. All rights reserved."));

  m_ui->m_about_label->setText(about_text);
}