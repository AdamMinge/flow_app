/* ----------------------------------- Local -------------------------------- */
#include "flow_document/component/properties/properties_dock.h"
/* ------------------------------------ Qt ---------------------------------- */
#include <QEvent>
#include <QVBoxLayout>
/* ------------------------------------ Ui ---------------------------------- */
#include "flow_document/ui_properties_dock.h"
/* -------------------------------------------------------------------------- */

namespace flow_document {

PropertiesDock::PropertiesDock(QWidget *parent)
    : QDockWidget(parent), m_document(nullptr), m_ui(new Ui::PropertiesDock()) {
  setObjectName(QLatin1String("Properties"));

  initUi();
  initConnections();

  retranslateUi();
}

PropertiesDock::~PropertiesDock() = default;

void PropertiesDock::setDocument(FlowDocument *document) {
  if (m_document == document) return;

  m_document = document;

  m_ui->m_properties_browser->setDocument(m_document);
}

FlowDocument *PropertiesDock::getDocument() const { return m_document; }

void PropertiesDock::changeEvent(QEvent *event) {
  QDockWidget::changeEvent(event);

  switch (event->type()) {
    case QEvent::LanguageChange:
      retranslateUi();
      break;
    default:
      break;
  }
}

void PropertiesDock::searchProperties(const QString &search) {}

void PropertiesDock::initUi() { m_ui->setupUi(this); }

void PropertiesDock::initConnections() {}

void PropertiesDock::retranslateUi() { m_ui->retranslateUi(this); }

}  // namespace flow_document