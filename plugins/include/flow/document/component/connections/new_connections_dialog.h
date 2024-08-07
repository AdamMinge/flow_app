#ifndef FLOW_DOCUMENT_NEW_CONNECTIONS_DIALOG_H
#define FLOW_DOCUMENT_NEW_CONNECTIONS_DIALOG_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QDialog>
/* ----------------------------------- Local -------------------------------- */
#include "flow/document/export.h"
/* -------------------------------------------------------------------------- */

namespace Ui {
class NewConnectionsDialog;
}

namespace flow {

class FlowDocument;
class Connection;
class NodeLayer;

class LIB_FLOW_DOCUMENT_API NewConnectionsDialog : public QDialog {
  Q_OBJECT

 public:
  NewConnectionsDialog(FlowDocument *document, QWidget *parent = nullptr);
  ~NewConnectionsDialog() override;

  void setNodeLayer(NodeLayer *layer);

 public Q_SLOTS:
  void accept() override;

 protected Q_SLOTS:
  void changeEvent(QEvent *event) override;

  void fillLayers();

  void fillInputNodes();
  void fillOutputNodes();

  void fillInputPins();
  void fillOutputPins();

  void validate();

 private:
  void initUi();
  void initConnections();
  void retranslateUi();

  [[nodiscard]] std::unique_ptr<Connection> createConnection() const;

 private:
  QScopedPointer<Ui::NewConnectionsDialog> m_ui;

  FlowDocument *m_document;
};

}  // namespace flow

#endif  // FLOW_DOCUMENT_NEW_CONNECTIONS_DIALOG_H
