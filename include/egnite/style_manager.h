#ifndef EGNITE_STYLE_MANAGER_H
#define EGNITE_STYLE_MANAGER_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
#include <QStringList>
#include <QStyle>
/* --------------------------------- Standard ------------------------------- */
#include <functional>
#include <memory>
#include <unordered_map>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite {

class LIB_EGNITE_API StyleManager : public QObject {
  Q_OBJECT

  using StyleFactory = std::function<QStyle *()>;

 public:
  static StyleManager &getInstance();
  static void deleteInstance();

 public:
  ~StyleManager() override;

  template <typename REGISTERED_TYPE, typename... ARGS>
  void registerStyle(const QString &style_name, ARGS &&...args);
  void unregisterStyle(const QString &style_name);

  [[nodiscard]] QStringList getAvailableStyles() const;
  [[nodiscard]] QString getCurrentStyle() const;

 public Q_SLOTS:
  bool setStyle(const QString &style_name);

 Q_SIGNALS:
  void styleChanged(const QString &style_name);
  void styleRegistered(const QString &style_name);
  void styleUnregistered(const QString &style_name);

 private:
  explicit StyleManager();

 private:
  static std::unique_ptr<StyleManager> m_instance;

  std::unordered_map<QString, StyleFactory> m_factories;
};

template <typename REGISTERED_TYPE, typename... ARGS>
void StyleManager::registerStyle(const QString &style_name, ARGS &&...args) {
  Q_ASSERT(!m_factories.contains(style_name));
  auto creator = StyleFactory([&args...]() {
    return new REGISTERED_TYPE(std::forward<ARGS>(args)...);
  });

  m_factories.insert(std::make_pair(style_name, creator));
  Q_EMIT styleRegistered(style_name);
}

}  // namespace egnite

#endif  // EGNITE_STYLE_MANAGER_H
