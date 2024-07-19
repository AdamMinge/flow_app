/* ----------------------------------- Local -------------------------------- */
#include "aegis/server.h"

#include "aegis/client.h"
/* ------------------------------------ Qt ---------------------------------- */
#include <QThreadPool>
/* -------------------------------------------------------------------------- */

namespace aegis {

Server::Server(QObject *parent) : QTcpServer(parent) {}

bool Server::start(quint16 port) { return listen(QHostAddress::Any, port); }

void Server::incomingConnection(qintptr socket_descriptor) {
  auto client = new Client(socket_descriptor);
  QThreadPool::globalInstance()->start(client);
}

}  // namespace aegis
