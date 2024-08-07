/* ----------------------------------- Local -------------------------------- */
#include "flow/document/flow/pin.h"
/* -------------------------------------------------------------------------- */

namespace flow {

Pin::Pin(QVariant data, QString caption)
    : m_data(std::move(data)), m_caption(std::move(caption)) {}

Pin::~Pin() = default;

Pin::Pin(const Pin &other) {
  m_data = other.m_data;
  m_caption = other.m_caption;
}

Pin::Pin(Pin &&other) {
  m_data = std::move(other.m_data);
  m_caption = std::move(other.m_caption);
  m_listeners = std::move(other.m_listeners);
}

Pin &Pin::operator=(const Pin &other) {
  m_data = other.m_data;
  m_caption = other.m_caption;

  return *this;
}

Pin &Pin::operator=(Pin &&other) {
  m_data = std::move(other.m_data);
  m_caption = std::move(other.m_caption);
  m_listeners = std::move(other.m_listeners);

  return *this;
}

const QVariant &Pin::getData() const { return m_data; }

const QString &Pin::getCaption() const { return m_caption; }

void Pin::setData(QVariant data) {
  m_data = std::move(data);
  dataChangeNotify();
}

void Pin::setCaption(QString caption) { m_caption = caption; }

void Pin::addListener(const size_t &id, Listener &&listener) {
  Q_ASSERT(!m_listeners.contains(id));
  m_listeners.emplace(id, std::move(listener));
}

void Pin::removeListener(const size_t &id) {
  Q_ASSERT(m_listeners.contains(id));
  m_listeners.erase(id);
}

void Pin::clearListeners() { m_listeners.clear(); }

void Pin::dataChangeNotify() {
  for (auto &[_, listener] : m_listeners) listener(m_data);
}

}  // namespace flow
