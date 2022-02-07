#ifndef FLOW_INTERFACE_DOCUMENT_FORMAT_H
#define FLOW_INTERFACE_DOCUMENT_FORMAT_H

/* ----------------------------------- Local -------------------------------- */
#include "flow/api/export.h"
#include "flow/api/file_format.h"
/* -------------------------------------------------------------------------- */

namespace api::document
{

  class IDocument;

  class API_API IDocumentFormat : public IFileFormat
  {
    Q_OBJECT

  public:
    explicit IDocumentFormat(QObject *parent = nullptr);
    ~IDocumentFormat() override;

    virtual std::unique_ptr<IDocument> load(const QString &file_name,
                                            QString *error) = 0;
    virtual bool save(const IDocument &document, const QString &file_name,
                      QString *error) = 0;
  };

}// namespace api::document

#endif//FLOW_INTERFACE_DOCUMENT_FORMAT_H
