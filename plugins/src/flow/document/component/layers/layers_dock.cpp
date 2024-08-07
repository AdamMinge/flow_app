/* ----------------------------------- Local -------------------------------- */
#include "flow/document/component/layers/layers_dock.h"

#include "flow/document/action_handler.h"
#include "flow/document/component/layers/layers_tree_model.h"
/* ------------------------------------ Qt ---------------------------------- */
#include <QEvent>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>
/* ----------------------------------- Qtils -------------------------------- */
#include <qtils/model/reverse_proxy_model.h>
/* ------------------------------------ Ui ---------------------------------- */
#include "flow/document/ui_layers_dock.h"
/* -------------------------------------------------------------------------- */

namespace flow {

LayersDock::LayersDock(QWidget *parent)
    : FlowDockWidget(parent),
      m_ui(new Ui::LayersDock()),
      m_layers_model(new LayersTreeModel),
      m_filter_model(new qtils::QtReverseProxyModel) {
  setObjectName(QLatin1String("Layers"));

  initUi();
  initConnections();

  retranslateUi();
}

LayersDock::~LayersDock() = default;

void LayersDock::onDocumentChanged(FlowDocument *from, FlowDocument *to) {
  if (to) {
    m_ui->m_layers_view->header()->setSectionResizeMode(
        LayersTreeModel::Column::NameColumn, QHeaderView::Stretch);
    m_ui->m_layers_view->header()->setSectionResizeMode(
        LayersTreeModel::Column::VisibleColumn, QHeaderView::Fixed);
    m_ui->m_layers_view->header()->setSectionResizeMode(
        LayersTreeModel::Column::LockedColumn, QHeaderView::Fixed);
  }

  m_layers_model->setDocument(to);
  m_ui->m_layers_view->setDocument(to);
}

void LayersDock::changeEvent(QEvent *event) {
  QDockWidget::changeEvent(event);

  switch (event->type()) {
    case QEvent::LanguageChange:
      retranslateUi();
      break;
    default:
      break;
  }
}

void LayersDock::searchLayers(const QString &search) {
  m_filter_model->setFilterWildcard(search);
}

void LayersDock::initUi() {
  m_ui->setupUi(this);

  m_filter_model->setSourceModel(m_layers_model.get());
  m_ui->m_layers_view->setModel(m_filter_model.get());

  m_filter_model->setFilterKeyColumn(LayersTreeModel::Column::NameColumn);
  m_filter_model->setRecursiveFilteringEnabled(true);

  const auto &handler = ActionHandler::getInstance();

  auto buttons_container = new QToolBar();
  buttons_container->setFloatable(false);
  buttons_container->setMovable(false);
  buttons_container->setIconSize(QSize(16, 16));

  buttons_container->addWidget(handler.createAddLayerButton());
  buttons_container->addAction(handler.getRemoveLayerAction());
  buttons_container->addAction(handler.getRaiseLayerAction());
  buttons_container->addAction(handler.getLowerLayerAction());
  buttons_container->addAction(handler.getDuplicateLayerAction());
  buttons_container->addSeparator();
  buttons_container->addAction(handler.getShowHideOtherLayersAction());
  buttons_container->addAction(handler.getLockUnlockOtherLayersAction());

  m_ui->m_toolbar_layout->addWidget(buttons_container);
}

void LayersDock::initConnections() {
  connect(m_ui->m_search_layers_edit, &QLineEdit::textChanged, this,
          &LayersDock::searchLayers);
}

void LayersDock::retranslateUi() { m_ui->retranslateUi(this); }

}  // namespace flow