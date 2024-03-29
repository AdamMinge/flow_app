/* ----------------------------------- Local -------------------------------- */
#include "flow_document/component/nodes/nodes_dock.h"

#include "flow_document/component/nodes/nodes_tree_model.h"
#include "flow_document/flow_document_action_handler.h"
/* ------------------------------------ Qt ---------------------------------- */
#include <QEvent>
#include <QToolBar>
#include <QVBoxLayout>
/* ------------------------------------ Ui ---------------------------------- */
#include "flow_document/ui_nodes_dock.h"
/* -------------------------------------------------------------------------- */

namespace flow_document {

NodesDock::NodesDock(QWidget *parent)
    : FlowDockWidget(parent),
      m_ui(new Ui::NodesDock()),
      m_nodes_model(new NodesTreeModel),
      m_filter_model(new OnlyNodesFilterProxyModel) {
  setObjectName(QLatin1String("Nodes"));

  initUi();
  initConnections();

  retranslateUi();
}

NodesDock::~NodesDock() = default;

void NodesDock::onDocumentChanged(FlowDocument *from, FlowDocument *to) {
  if (to) {
    m_ui->m_nodes_view->header()->setSectionResizeMode(
        NodesTreeModel::Column::NameColumn, QHeaderView::Stretch);
    m_ui->m_nodes_view->header()->setSectionResizeMode(
        NodesTreeModel::Column::VisibleColumn, QHeaderView::Fixed);
  }

  m_nodes_model->setDocument(to);
  m_ui->m_nodes_view->setDocument(to);
}

void NodesDock::changeEvent(QEvent *event) {
  QDockWidget::changeEvent(event);

  switch (event->type()) {
    case QEvent::LanguageChange:
      retranslateUi();
      break;
    default:
      break;
  }
}

void NodesDock::searchNodes(const QString &search) {
  m_filter_model->setFilterWildcard(search);
}

void NodesDock::initUi() {
  m_ui->setupUi(this);

  m_filter_model->setSourceModel(m_nodes_model.get());
  m_ui->m_nodes_view->setModel(m_filter_model.get());

  m_filter_model->setFilterKeyColumn(NodesTreeModel::Column::NameColumn);
  m_filter_model->setRecursiveFilteringEnabled(true);

  const auto &handler = FlowDocumentActionHandler::getInstance();

  auto buttons_container = new QToolBar();
  buttons_container->setFloatable(false);
  buttons_container->setMovable(false);
  buttons_container->setIconSize(QSize(16, 16));

  buttons_container->addWidget(handler.createAddNodeButton());
  buttons_container->addAction(handler.getRemoveNodeAction());
  buttons_container->addAction(handler.getRaiseNodeAction());
  buttons_container->addAction(handler.getLowerNodeAction());
  buttons_container->addAction(handler.getDuplicateNodeAction());

  m_ui->m_toolbar_layout->addWidget(buttons_container);
}

void NodesDock::initConnections() {
  connect(m_ui->m_search_nodes_edit, &QLineEdit::textChanged, this,
          &NodesDock::searchNodes);
}

void NodesDock::retranslateUi() { m_ui->retranslateUi(this); }

}  // namespace flow_document