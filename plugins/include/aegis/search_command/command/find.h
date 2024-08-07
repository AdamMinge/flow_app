#ifndef AEGIS_SEARCH_COMMAND_FIND_H
#define AEGIS_SEARCH_COMMAND_FIND_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QCommandLineParser>
#include <QObject>
#include <QTimer>
/* ---------------------------- Plugin Aegis Server ------------------------- */
#include <aegis/server/command/command.h>
#include <aegis/server/response.h>
/* ----------------------------------- Local -------------------------------- */
#include "aegis/search_command/export.h"
/* -------------------------------------------------------------------------- */

namespace aegis {

class ObjectSearcher;

/* ---------------------------- FoundObjectsMessage ------------------------- */

struct LIB_AEGIS_SERVER_API FoundObjectsMessage {
  Q_GADGET

 public:
  Q_PROPERTY(QStringList objects MEMBER objects)

  QStringList objects;
};

/* ------------------------------- ObjectsFinder ---------------------------- */

class LIB_AEGIS_SEARCH_COMMAND_API ObjectsFinder {
 public:
  using Result = Response<FoundObjectsMessage>;

 public:
  explicit ObjectsFinder(const ObjectSearcher& searcher);
  ~ObjectsFinder();

  Result find(const QString& id);

 private:
  const ObjectSearcher& m_searcher;
};

/* ------------------------------- FindCommand ------------------------------ */

class LIB_AEGIS_SEARCH_COMMAND_API FindCommand : public Command {
 public:
  explicit FindCommand(const CommandManager& manager);
  ~FindCommand() override;

  [[nodiscard]] QString getName() const override;

  [[nodiscard]] QByteArray exec(const QStringList& args) override;

 private:
  QCommandLineParser m_parser;
  ObjectsFinder m_finder;
};

}  // namespace aegis

#endif  // AEGIS_SEARCH_COMMAND_FIND_H