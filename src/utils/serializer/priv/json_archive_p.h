#ifndef UTILS_SERIALIZER_JSON_ARCHIVE_P_H
#define UTILS_SERIALIZER_JSON_ARCHIVE_P_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QIODevice>
#include <QJsonValue>
/* ---------------------------------- Standard ------------------------------ */
#include <stack>
#include <variant>
/* -------------------------------------------------------------------------- */

namespace utils {

class JsonArchive {
  using ValueWithKey = struct {
    QJsonValue value;
    std::variant<qsizetype, QString> key;
  };

 public:
  explicit JsonArchive();
  explicit JsonArchive(QIODevice& device);

  void push(const QString& key);
  void push(qsizetype number);

  void pop();

  [[nodiscard]] const QJsonValue& value() const;
  [[nodiscard]] QJsonValue& value();

  void save();

 private:
  QIODevice& m_device;
  QJsonValue m_root;
  std::stack<ValueWithKey> m_stack;
};

}  // namespace utils

#endif  // UTILS_SERIALIZER_JSON_ARCHIVE_P_H
