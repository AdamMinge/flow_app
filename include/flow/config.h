/* -------------------------------------------------------------------------- */
#ifndef FLOW_CONFIG_H
#define FLOW_CONFIG_H
/* -------------------------------------------------------------------------- */
/* ---------------------------- Define FLOW version ------------------------- */
/* -------------------------------------------------------------------------- */
#define FLOW_VERSION_MAJOR @FLOW_VERSION_MAJOR @
#define FLOW_VERSION_MINOR @FLOW_VERSION_MINOR @
#define FLOW_VERSION_PATCH @FLOW_VERSION_PATCH @
#define FLOW_VERSION \
  @FLOW_VERSION_MAJOR FLOW_VERSION_MINOR FLOW_VERSION_PATCH @
#define FLOW_VERSION_STR \
  "0.1.0"
/* -------------------------------------------------------------------------- */
/* ----------------------- Identify the operating system -------------------- */
/* -------------------------------------------------------------------------- */
#if defined(_WIN32)
#define FLOW_OS_WINDOWS
#elif defined(__linux__)
#define FLOW_OS_LINUX
#else
#error This operating system is not supported by FLOW library
#endif
/* -------------------------------------------------------------------------- */
/* ------------------------ Define a portable debug macro ------------------- */
/* -------------------------------------------------------------------------- */
#if !defined(NDEBUG)
#define FLOW_DEBUG
#endif
/* -------------------------------------------------------------------------- */
/* -------------------- Define helpers to set items as deprecated ----------- */
/* -------------------------------------------------------------------------- */
#if !defined(FLOW_DEPRECATED_WARNINGS)
#define FLOW_DEPRECATED [[deprecated]]
#else
#define FLOW_DEPRECATED
#endif
/* -------------------------------------------------------------------------- */
#endif  // FLOW_CONFIG_H
/* -------------------------------------------------------------------------- */
