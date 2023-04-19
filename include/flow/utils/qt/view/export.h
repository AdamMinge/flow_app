#ifndef FLOW_VIEW_EXPORT_H
#define FLOW_VIEW_EXPORT_H

/* ------------------------------------ Qt ---------------------------------- */
#include <QtCore/QtGlobal>
/* -------------------------------------------------------------------------- */

#if defined(VIEW_EXPORTS)
#define VIEW_API Q_DECL_EXPORT
#else
#define VIEW_API Q_DECL_IMPORT
#endif

#endif  // FLOW_VIEW_EXPORT_H
