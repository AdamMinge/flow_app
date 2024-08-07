/* ----------------------------------- Local -------------------------------- */
#include "aegis/server/server_manager.h"

#include "aegis/server/server.h"
/* -------------------------------------------------------------------------- */

namespace aegis {

std::unique_ptr<ServerManager> ServerManager::m_instance =
    std::unique_ptr<ServerManager>(nullptr);

ServerManager& ServerManager::getInstance() {
  if (!m_instance) m_instance.reset(new ServerManager);

  return *m_instance;
}

void ServerManager::deleteInstance() { m_instance.reset(nullptr); }

ServerManager::ServerManager() : m_server(new Server(this)) {}

ServerManager::~ServerManager() = default;

bool ServerManager::start(const QHostAddress& host, quint16 port) {
  return m_server->listen(host, port);
}

void ServerManager::stop() { m_server->close(); }

bool ServerManager::isRunning() const { return m_server->isListening(); }

QHostAddress ServerManager::getHost() const {
  return m_server->serverAddress();
}

quint16 ServerManager::getPort() const { return m_server->serverPort(); }

}  // namespace aegis
