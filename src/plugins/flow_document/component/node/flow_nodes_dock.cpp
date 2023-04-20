/* ----------------------------------- Local -------------------------------- */
#include "flow_document/component/node/flow_nodes_dock.h"

#include "flow_document/component/node/flow_nodes_tree_delegate.h"
#include "flow_document/component/node/flow_nodes_tree_model.h"
/* ------------------------------------ Qt ---------------------------------- */
#include <QEvent>
#include <QVBoxLayout>
/* ------------------------------------ Ui ---------------------------------- */
#include "flow_document/ui_flow_nodes_dock.h"
/* -------------------------------------------------------------------------- */

namespace flow_document {

FlowNodesDock::FlowNodesDock(QWidget *parent)
    : QDockWidget(parent),
      m_ui(new Ui::FlowNodesDock()),
      m_nodes_model(new FlowNodesTreeModel),
      m_nodes_filter_model(new QSortFilterProxyModel),
      m_nodes_delegate(new FlowNodesTreeDelegate) {
  setObjectName(QLatin1String("Nodes"));

  initUi();
  initConnections();

  retranslateUi();
}

FlowNodesDock::~FlowNodesDock() = default;

void FlowNodesDock::changeEvent(QEvent *event) {
  QDockWidget::changeEvent(event);

  switch (event->type()) {
    case QEvent::LanguageChange:
      retranslateUi();
      break;
    default:
      break;
  }
}

void FlowNodesDock::initUi() {
  m_ui->setupUi(this);

  m_nodes_filter_model->setSourceModel(m_nodes_model.get());

  m_ui->m_nodes_view->setModel(m_nodes_filter_model.get());
  m_ui->m_nodes_view->setItemDelegate(m_nodes_delegate.get());
}

void FlowNodesDock::initConnections() {}

void FlowNodesDock::retranslateUi() { m_ui->retranslateUi(this); }

}  // namespace flow_document