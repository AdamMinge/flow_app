# ----------------------------------------------------------------------- #
# ---------------------------- Set CMake Version ------------------------ #
# ----------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
# ----------------------------------------------------------------------- #
# ---------------------------- Generate headers ------------------------- #
# ----------------------------------------------------------------------- #
set(include_root ${EGNITE_SOURCE_DIR}/include)
configure_file(${include_root}/config.h.in ${include_root}/config.h)
# ----------------------------------------------------------------------- #
# --------------------------- Add subdirectories ------------------------ #
# ----------------------------------------------------------------------- #
add_subdirectory(utils)
add_subdirectory(egnite)
add_subdirectory(plugins)
add_subdirectory(editor)
# ----------------------------------------------------------------------- #
