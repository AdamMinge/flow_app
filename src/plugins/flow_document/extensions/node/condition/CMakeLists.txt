# ----------------------------------------------------------------------- #
# ---------------------------- Set CMake Version ------------------------ #
# ----------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
# ----------------------------------------------------------------------- #
# ---------------------------- Set files variable ----------------------- #
# ----------------------------------------------------------------------- #
set(source_root
    ${EGNITE_SOURCE_DIR}/src/plugins/flow_document/extensions/node/condition)
set(include_root
    ${EGNITE_SOURCE_DIR}/include/plugins/flow_document/extensions/node/condition
)

set(sources
    ${source_root}/condition_node_switcher.cpp
    ${source_root}/condition_node_dispatcher.cpp
    ${source_root}/condition_node_plugin.cpp)

set(includes
    ${include_root}/export.h ${include_root}/condition_node_switcher.h
    ${include_root}/condition_node_dispatcher.h
    ${include_root}/condition_node_plugin.h)
# ----------------------------------------------------------------------- #
# ------------------------- Qt libraries configuration ------------------ #
# ----------------------------------------------------------------------- #
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
# ----------------------------------------------------------------------- #
# -------------------------- Find external libraries -------------------- #
# ----------------------------------------------------------------------- #
find_package(Qt6 REQUIRED COMPONENTS Widgets)
# ----------------------------------------------------------------------- #
# -------------------------- Create plugin shared lib ------------------- #
# ----------------------------------------------------------------------- #
egnite_add_plugins(
  condition_node
  SOURCES
  ${sources}
  ${includes}
  DEPENDS
  Qt6::Widgets
  flow_document)

target_include_directories(
  condition_node
  PUBLIC $<BUILD_INTERFACE:${include_root}>
  INTERFACE $<INSTALL_INTERFACE:include>
  PRIVATE ${source_root})
# ----------------------------------------------------------------------- #
