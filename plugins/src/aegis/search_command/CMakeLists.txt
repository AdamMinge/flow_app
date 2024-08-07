# ----------------------------------------------------------------------- #
# ---------------------------- Set CMake Version ------------------------ #
# ----------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
# ----------------------------------------------------------------------- #
# ---------------------------- Set files variable ----------------------- #
# ----------------------------------------------------------------------- #
set(source_root ${PILLAR_SOURCE_DIR}/plugins/src/aegis/search_command)
set(include_root ${PILLAR_SOURCE_DIR}/plugins/include/aegis/search_command)

set(sources
    ${source_root}/plugin.cpp ${source_root}/command/find.cpp
    ${source_root}/command/parent.cpp ${source_root}/command/children.cpp)

set(includes
    ${include_root}/export.h ${include_root}/plugin.h
    ${include_root}/command/find.h ${include_root}/command/parent.h
    ${include_root}/command/children.h)
# ----------------------------------------------------------------------- #
# ------------------------- Qt libraries configuration ------------------ #
# ----------------------------------------------------------------------- #
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
# ----------------------------------------------------------------------- #
# -------------------------- Find external libraries -------------------- #
# ----------------------------------------------------------------------- #
find_package(Qt6 REQUIRED COMPONENTS Widgets Qml)
# ----------------------------------------------------------------------- #
# -------------------------- Create plugin shared lib ------------------- #
# ----------------------------------------------------------------------- #
pillar_add_plugins(
  aegis_search_command
  SOURCES
  ${sources}
  ${includes}
  DEPENDS
  Qt6::Core
  Qt6::Widgets
  plugin::aegis_server)

target_include_directories(
  aegis_search_command
  PUBLIC $<BUILD_INTERFACE:${PILLAR_SOURCE_DIR}/plugins/include>
  INTERFACE $<INSTALL_INTERFACE:include>
  PRIVATE ${PILLAR_SOURCE_DIR}/plugins/src)
# ----------------------------------------------------------------------- #
