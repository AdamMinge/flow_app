#ifndef UTILS_PROPERTY_BROWSER_EXPORT_H
#define UTILS_PROPERTY_BROWSER_EXPORT_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QtCore/QtGlobal>
/* -------------------------------------------------------------------------- */

#if defined(PROPERTY_BROWSER_EXPORTS)
#define PROPERTY_BROWSER_API Q_DECL_EXPORT
#else
#define PROPERTY_BROWSER_API Q_DECL_IMPORT
#endif

#endif  // UTILS_PROPERTY_BROWSER_EXPORT_H
