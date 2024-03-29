# ----------------------------------------------------------------------- #
# ---------------------------- Set CMake Version ------------------------ #
# ----------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
# ----------------------------------------------------------------------- #
# ---------------------------- Set files variable ----------------------- #
# ----------------------------------------------------------------------- #
set(source_root
    ${EGNITE_SOURCE_DIR}/src/plugins/flow_document/extensions/node/math)
set(include_root
    ${EGNITE_SOURCE_DIR}/include/plugins/flow_document/extensions/node/math)

set(sources
    ${source_root}/math_node_value.cpp
    ${source_root}/math_node_addition.cpp
    ${source_root}/math_node_division.cpp
    ${source_root}/math_node_multiplication.cpp
    ${source_root}/math_node_negation.cpp
    ${source_root}/math_node_subtraction.cpp
    ${source_root}/math_node_power.cpp
    ${source_root}/math_node_square_root.cpp
    ${source_root}/math_node_equal.cpp
    ${source_root}/math_node_greater_equal.cpp
    ${source_root}/math_node_greater.cpp
    ${source_root}/math_node_lower_equal.cpp
    ${source_root}/math_node_lower.cpp
    ${source_root}/math_node_not_equal.cpp
    ${source_root}/math_node_plugin.cpp)

set(includes
    ${include_root}/export.h
    ${include_root}/math_node_value.h
    ${include_root}/math_node_addition.h
    ${include_root}/math_node_division.h
    ${include_root}/math_node_multiplication.h
    ${include_root}/math_node_negation.h
    ${include_root}/math_node_subtraction.h
    ${include_root}/math_node_power.h
    ${include_root}/math_node_square_root.h
    ${include_root}/math_node_equal.h
    ${include_root}/math_node_greater_equal.h
    ${include_root}/math_node_greater.h
    ${include_root}/math_node_lower_equal.h
    ${include_root}/math_node_lower.h
    ${include_root}/math_node_not_equal.h
    ${include_root}/math_node_plugin.h)
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
  math_node
  SOURCES
  ${sources}
  ${includes}
  DEPENDS
  Qt6::Widgets
  flow_document)

target_include_directories(
  math_node
  PUBLIC $<BUILD_INTERFACE:${include_root}>
  INTERFACE $<INSTALL_INTERFACE:include>
  PRIVATE ${source_root})
# ----------------------------------------------------------------------- #
