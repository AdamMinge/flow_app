#ifndef FLOW_NO_DOCUMENT_WIDGET_H
#define FLOW_NO_DOCUMENT_WIDGET_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QSortFilterProxyModel>
#include <QWidget>
/* -------------------------------------------------------------------------- */

namespace Ui {
class NoDocumentWidget;
}

class NoDocumentWidget : public QWidget {
  Q_OBJECT

 public:
  explicit NoDocumentWidget(QWidget *parent = nullptr);
  ~NoDocumentWidget() override;

 protected:
  void changeEvent(QEvent *event) override;

 private:
  void retranslateUi();

 private:
  QScopedPointer<Ui::NoDocumentWidget> m_ui;
};

#endif  // FLOW_NO_DOCUMENT_WIDGET_H
