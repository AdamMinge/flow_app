#ifndef EGNITE_LOGGING_MANAGER_H
#define EGNITE_LOGGING_MANAGER_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite {

class LIB_EGNITE_API LoggingManager : public QObject {
  Q_OBJECT

 public:
  enum class Severity { Info, Warning, Error };

 public:
  [[nodiscard]] static LoggingManager &getInstance();
  static void deleteInstance();

 public:
  ~LoggingManager() override;

  void log(Severity severity, const QString &message);

 Q_SIGNALS:
  void onInfoLog(const QString &message);
  void onWarningLog(const QString &message);
  void onErrorLog(const QString &message);

 protected:
  explicit LoggingManager();

 private:
  static QScopedPointer<LoggingManager> m_instance;
};

inline void LOG_INFO(const QString &message) {
  LoggingManager::getInstance().log(LoggingManager::Severity::Info, message);
}

inline void LOG_WARNING(const QString &message) {
  LoggingManager::getInstance().log(LoggingManager::Severity::Warning, message);
}

inline void LOG_ERROR(const QString &message) {
  LoggingManager::getInstance().log(LoggingManager::Severity::Error, message);
}

}  // namespace egnite

#endif  // EGNITE_LOGGING_MANAGER_H
