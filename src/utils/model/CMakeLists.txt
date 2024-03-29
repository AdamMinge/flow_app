# ----------------------------------------------------------------------- #
# ---------------------------- Set CMake Version ------------------------ #
# ----------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
# ----------------------------------------------------------------------- #
# ---------------------------- Set files variable ----------------------- #
# ----------------------------------------------------------------------- #
set(source_root ${EGNITE_SOURCE_DIR}/src/utils/model)
set(include_root ${EGNITE_SOURCE_DIR}/include/utils/model)

set(sources ${source_root}/leaf_filter_proxy_model.cpp
            ${source_root}/reverse_proxy_model.cpp ${source_root}/cast.cpp)

set(includes ${include_root}/export.h ${include_root}/leaf_filter_proxy_model.h
             ${include_root}/reverse_proxy_model.h ${include_root}/cast.h)
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
# -------------------------- Create utils shared lib -------------------- #
# ----------------------------------------------------------------------- #
egnite_add_utils(model SOURCES ${sources} ${includes} DEPENDS Qt6::Widgets)

target_include_directories(
  model
  PUBLIC $<BUILD_INTERFACE:${EGNITE_SOURCE_DIR}/include/>
  INTERFACE $<INSTALL_INTERFACE:include>
  PRIVATE ${EGNITE_SOURCE_DIR}/src/)
# ----------------------------------------------------------------------- #
