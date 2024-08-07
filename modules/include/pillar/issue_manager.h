#ifndef PILLAR_ISSUE_MANAGER_H
#define PILLAR_ISSUE_MANAGER_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
/* --------------------------------- Standard ------------------------------- */
#include <memory>
/* ----------------------------------- Local -------------------------------- */
#include "pillar/export.h"
#include "pillar/issue.h"
/* -------------------------------------------------------------------------- */

namespace pillar {

class LIB_PILLAR_API IssueManager : public QObject {
  Q_OBJECT

 public:
  [[nodiscard]] static IssueManager &getInstance();
  static void deleteInstance();

 public:
  ~IssueManager() override;

  void report(const Issue &issue);

  void clear(const QVariant &context);
  void clear();

 Q_SIGNALS:
  void onWarningReport(const pillar::Issue &issue);
  void onErrorReport(const pillar::Issue &issue);

  void onClear(const QVariant &context);
  void onClear();

 protected:
  explicit IssueManager();

 private:
  static std::unique_ptr<IssueManager> m_instance;
};

inline void REPORT_WARNING(QString text, QVariant context = QVariant{},
                           Issue::Callback callback = Issue::Callback{}) {
  IssueManager::getInstance().report(Issue{Issue::Severity::Warning,
                                           std::move(text), std::move(context),
                                           std::move(callback)});
}

inline void REPORT_ERROR(QString text, QVariant context = QVariant{},
                         Issue::Callback callback = Issue::Callback{}) {
  IssueManager::getInstance().report(Issue{Issue::Severity::Error,
                                           std::move(text), std::move(context),
                                           std::move(callback)});
}

inline void CLEAR_REPORTS(const QVariant &context) {
  IssueManager::getInstance().clear(context);
}

}  // namespace pillar

#endif  // PILLAR_ISSUE_MANAGER_H
