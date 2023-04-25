#ifndef EGNITE_FILE_FORMAT_H
#define EGNITE_FILE_FORMAT_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QObject>
/* ----------------------------------- Local -------------------------------- */
#include "egnite/export.h"
/* -------------------------------------------------------------------------- */

namespace egnite {

class LIB_EGNITE_API FileFormat : public QObject {
  Q_OBJECT

 public:
  enum class Capability;
  Q_DECLARE_FLAGS(Capabilities, Capability);

 public:
  explicit FileFormat(QObject *parent = nullptr);
  ~FileFormat() override;

  [[nodiscard]] bool hasCapabilities(Capabilities capabilities) const;
  [[nodiscard]] virtual Capabilities getCapabilities() const;

  [[nodiscard]] virtual QString getNameFilter() const = 0;
  [[nodiscard]] virtual QString getShortName() const = 0;
  [[nodiscard]] virtual bool supportsFile(const QString &filename) const = 0;
};

enum class FileFormat::Capability {
  NoCapability = 0x0,
  Read = 0x1,
  Write = 0x2,
  ReadWrite = Read | Write
};

}  // namespace egnite

Q_DECLARE_INTERFACE(egnite::FileFormat, "org.egnite.FileFormat")
Q_DECLARE_OPERATORS_FOR_FLAGS(egnite::FileFormat::Capabilities);

#endif  // EGNITE_FILE_FORMAT_H
