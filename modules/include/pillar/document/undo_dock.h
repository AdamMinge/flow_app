#ifndef PILLAR_UNDO_DOCK_H
#define PILLAR_UNDO_DOCK_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QDockWidget>
#include <QUndoStack>
#include <QUndoView>
/* ----------------------------------- Local -------------------------------- */
#include "pillar/export.h"
/* -------------------------------------------------------------------------- */

namespace pillar {

class LIB_PILLAR_API UndoDock : public QDockWidget {
  Q_OBJECT

 public:
  explicit UndoDock(QWidget *parent = nullptr);
  ~UndoDock() override;

  void setStack(QUndoStack *stack);

 protected:
  void changeEvent(QEvent *event) override;

 private:
  void retranslateUi();

 private:
  QUndoView *m_undo_view;
};

}  // namespace pillar

#endif  // PILLAR_UNDO_DOCK_H
