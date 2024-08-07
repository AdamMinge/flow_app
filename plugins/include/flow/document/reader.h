#ifndef FLOW_DOCUMENT_FLOW_DOCUMENT_READER_H
#define FLOW_DOCUMENT_FLOW_DOCUMENT_READER_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QIODevice>
/* --------------------------------- Standard ------------------------------- */
#include <memory>
/* ----------------------------------- Local -------------------------------- */
#include "flow/document/export.h"
/* -------------------------------------------------------------------------- */

namespace flow {

class FlowDocument;

class LIB_FLOW_DOCUMENT_API FlowDocumentReader {
  class FlowDocumentReaderImpl;

 public:
  explicit FlowDocumentReader();
  ~FlowDocumentReader();

  std::unique_ptr<FlowDocument> read(QIODevice &device,
                                     QString *error = nullptr);
  std::unique_ptr<FlowDocument> read(const QString &file_name,
                                     QString *error = nullptr);

  bool isValid(const QString &file_name);
  bool isValid(QIODevice &device);

 private:
  std::unique_ptr<FlowDocumentReaderImpl> m_impl;
};

}  // namespace flow

#endif  // FLOW_DOCUMENT_FLOW_DOCUMENT_READER_H
