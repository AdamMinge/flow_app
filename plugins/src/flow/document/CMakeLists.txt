# ----------------------------------------------------------------------- #
# ---------------------------- Set CMake Version ------------------------ #
# ----------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.11)
# ----------------------------------------------------------------------- #
# ---------------------------- Set files variable ----------------------- #
# ----------------------------------------------------------------------- #
set(source_root ${PILLAR_SOURCE_DIR}/plugins/src/flow/document)
set(include_root ${PILLAR_SOURCE_DIR}/plugins/include/flow/document)
set(ui_root ${PILLAR_SOURCE_DIR}/plugins/ui/flow/document)
set(resource_root ${PILLAR_SOURCE_DIR}/plugins/resource/flow/document)

set(sources
    ${source_root}/plugin.cpp
    ${source_root}/document.cpp
    ${source_root}/format.cpp
    ${source_root}/action_handler.cpp
    ${source_root}/reader.cpp
    ${source_root}/writer.cpp
    ${source_root}/editor.cpp
    ${source_root}/script_module.cpp
    ${source_root}/new_document_widget.cpp
    ${source_root}/event/change_event.cpp
    ${source_root}/event/layer_change_event.cpp
    ${source_root}/event/node_change_event.cpp
    ${source_root}/event/connection_change_event.cpp
    ${source_root}/flow/node.cpp
    ${source_root}/flow/object.cpp
    ${source_root}/flow/pin.cpp
    ${source_root}/flow/flow.cpp
    ${source_root}/flow/layer.cpp
    ${source_root}/flow/connection.cpp
    ${source_root}/flow/group_layer.cpp
    ${source_root}/flow/node_layer.cpp
    ${source_root}/flow/layer_iterator.cpp
    ${source_root}/flow/factory/object_factory.cpp
    ${source_root}/flow/factory/node_factory.cpp
    ${source_root}/flow/factory/layer_factory.cpp
    ${source_root}/command/add_remove_connection.cpp
    ${source_root}/command/add_remove_node.cpp
    ${source_root}/command/add_remove_layer.cpp
    ${source_root}/command/change_layer.cpp
    ${source_root}/command/change_node.cpp
    ${source_root}/command/change_connection.cpp
    ${source_root}/command/reparent_layer.cpp
    ${source_root}/command/raise_lower_layer.cpp
    ${source_root}/command/raise_lower_node.cpp
    ${source_root}/command/duplicate_layer.cpp
    ${source_root}/command/duplicate_node.cpp
    ${source_root}/component/dock_widget.cpp
    ${source_root}/component/dock_widget_factory.cpp
    ${source_root}/component/factories/factories_dock.cpp
    ${source_root}/component/factories/factories_view.cpp
    ${source_root}/component/factories/factories_tree_model.cpp
    ${source_root}/component/properties/properties_dock.cpp
    ${source_root}/component/properties/properties_browser.cpp
    ${source_root}/component/properties/variant_property_manager.cpp
    ${source_root}/component/properties/object_properties.cpp
    ${source_root}/component/properties/layer_properties.cpp
    ${source_root}/component/properties/node_properties.cpp
    ${source_root}/component/properties/connection_properties.cpp
    ${source_root}/component/properties/create_property.cpp
    ${source_root}/component/properties/factory/object_properties_factory.cpp
    ${source_root}/component/properties/factory/layer_properties_factory.cpp
    ${source_root}/component/properties/factory/node_properties_factory.cpp
    ${source_root}/component/properties/factory/connection_properties_factory.cpp
    ${source_root}/component/properties/factory/utils.cpp
    ${source_root}/component/nodes/nodes_dock.cpp
    ${source_root}/component/nodes/nodes_view.cpp
    ${source_root}/component/nodes/nodes_tree_model.cpp
    ${source_root}/component/layers/layers_dock.cpp
    ${source_root}/component/layers/layers_view.cpp
    ${source_root}/component/layers/layers_tree_model.cpp
    ${source_root}/component/connections/connections_dock.cpp
    ${source_root}/component/connections/connections_view.cpp
    ${source_root}/component/connections/connections_tree_model.cpp
    ${source_root}/component/connections/new_connections_dialog.cpp
    ${source_root}/component/scene/scene.cpp
    ${source_root}/component/scene/view.cpp
    ${source_root}/component/scene/item/object_item.cpp
    ${source_root}/component/scene/item/flow_item.cpp
    ${source_root}/component/scene/item/layer_item.cpp
    ${source_root}/component/scene/item/node_item.cpp
    ${source_root}/component/scene/item/connection_item.cpp
    ${source_root}/component/scene/item/selection_rectangle.cpp
    ${source_root}/component/scene/item/object_selection_item.cpp
    ${source_root}/component/scene/item/cubic_path_item.cpp
    ${source_root}/component/scene/item/factory/item_factory.cpp
    ${source_root}/component/scene/item/factory/connection_item_factory.cpp
    ${source_root}/component/scene/item/factory/flow_item_factory.cpp
    ${source_root}/component/scene/item/factory/layer_item_factory.cpp
    ${source_root}/component/scene/item/factory/node_item_factory.cpp
    ${source_root}/component/scene/item/factory/utils.cpp
    ${source_root}/component/scene/style/style_manager.cpp
    ${source_root}/component/scene/style/style_writer.cpp
    ${source_root}/component/scene/style/style_reader.cpp
    ${source_root}/component/scene/style/style.cpp
    ${source_root}/component/scene/tool/tools_bar.cpp
    ${source_root}/component/scene/tool/tool.cpp
    ${source_root}/component/scene/tool/node_selection_tool.cpp
    ${source_root}/component/scene/tool/node_connection_tool.cpp
    ${source_root}/component/scene/tool/factory/tool_factory.cpp
    ${source_root}/component/scene/tool/factory/node_connection_tool_factory.cpp
    ${source_root}/component/scene/tool/factory/node_selection_tool_factory.cpp)

set(includes
    ${include_root}/export.h
    ${include_root}/plugin.h
    ${include_root}/concept.h
    ${include_root}/resources.h
    ${include_root}/document.h
    ${include_root}/format.h
    ${include_root}/action_handler.h
    ${include_root}/reader.h
    ${include_root}/writer.h
    ${include_root}/editor.h
    ${include_root}/script_module.h
    ${include_root}/new_document_widget.h
    ${include_root}/event/change_event.h
    ${include_root}/event/layer_change_event.h
    ${include_root}/event/node_change_event.h
    ${include_root}/event/connection_change_event.h
    ${include_root}/flow/node.h
    ${include_root}/flow/object.h
    ${include_root}/flow/pin.h
    ${include_root}/flow/flow.h
    ${include_root}/flow/layer.h
    ${include_root}/flow/connection.h
    ${include_root}/flow/group_layer.h
    ${include_root}/flow/node_layer.h
    ${include_root}/flow/layer_iterator.h
    ${include_root}/flow/factory/object_factory.h
    ${include_root}/flow/factory/node_factory.h
    ${include_root}/flow/factory/layer_factory.h
    ${include_root}/flow/serialization/object_unique_ptr.h
    ${include_root}/command/add_remove_connection.h
    ${include_root}/command/add_remove_node.h
    ${include_root}/command/add_remove_layer.h
    ${include_root}/command/change_value.h
    ${include_root}/command/change_layer.h
    ${include_root}/command/change_node.h
    ${include_root}/command/change_connection.h
    ${include_root}/command/reparent_layer.h
    ${include_root}/command/raise_lower_layer.h
    ${include_root}/command/raise_lower_node.h
    ${include_root}/command/duplicate_layer.h
    ${include_root}/command/duplicate_node.h
    ${include_root}/component/dock_widget.h
    ${include_root}/component/dock_widget_factory.h
    ${include_root}/component/factories/factories_dock.h
    ${include_root}/component/factories/factories_view.h
    ${include_root}/component/factories/factories_tree_model.h
    ${include_root}/component/properties/properties_dock.h
    ${include_root}/component/properties/properties_browser.h
    ${include_root}/component/properties/variant_property_manager.h
    ${include_root}/component/properties/object_properties.h
    ${include_root}/component/properties/layer_properties.h
    ${include_root}/component/properties/node_properties.h
    ${include_root}/component/properties/connection_properties.h
    ${include_root}/component/properties/create_property.h
    ${include_root}/component/properties/factory/object_properties_factory.h
    ${include_root}/component/properties/factory/layer_properties_factory.h
    ${include_root}/component/properties/factory/node_properties_factory.h
    ${include_root}/component/properties/factory/connection_properties_factory.h
    ${include_root}/component/properties/factory/utils.h
    ${include_root}/component/nodes/nodes_dock.h
    ${include_root}/component/nodes/nodes_view.h
    ${include_root}/component/nodes/nodes_tree_model.h
    ${include_root}/component/layers/layers_dock.h
    ${include_root}/component/layers/layers_view.h
    ${include_root}/component/layers/layers_tree_model.h
    ${include_root}/component/connections/connections_dock.h
    ${include_root}/component/connections/connections_view.h
    ${include_root}/component/connections/connections_tree_model.h
    ${include_root}/component/connections/new_connections_dialog.h
    ${include_root}/component/scene/scene.h
    ${include_root}/component/scene/view.h
    ${include_root}/component/scene/item/object_item.h
    ${include_root}/component/scene/item/flow_item.h
    ${include_root}/component/scene/item/layer_item.h
    ${include_root}/component/scene/item/node_item.h
    ${include_root}/component/scene/item/connection_item.h
    ${include_root}/component/scene/item/selection_rectangle.h
    ${include_root}/component/scene/item/object_selection_item.h
    ${include_root}/component/scene/item/cubic_path_item.h
    ${include_root}/component/scene/item/factory/item_factory.h
    ${include_root}/component/scene/item/factory/connection_item_factory.h
    ${include_root}/component/scene/item/factory/flow_item_factory.h
    ${include_root}/component/scene/item/factory/layer_item_factory.h
    ${include_root}/component/scene/item/factory/node_item_factory.h
    ${include_root}/component/scene/item/factory/utils.h
    ${include_root}/component/scene/style/style_manager.h
    ${include_root}/component/scene/style/style_writer.h
    ${include_root}/component/scene/style/style_reader.h
    ${include_root}/component/scene/style/style.h
    ${include_root}/component/scene/tool/tools_bar.h
    ${include_root}/component/scene/tool/tool.h
    ${include_root}/component/scene/tool/node_selection_tool.h
    ${include_root}/component/scene/tool/node_connection_tool.h
    ${include_root}/component/scene/tool/factory/tool_factory.h
    ${include_root}/component/scene/tool/factory/node_connection_tool_factory.h
    ${include_root}/component/scene/tool/factory/node_selection_tool_factory.h)

set(resources ${resource_root}/document.qrc)

set(uis
    ${ui_root}/new_document_widget.ui
    ${ui_root}/factories_dock.ui
    ${ui_root}/properties_dock.ui
    ${ui_root}/layers_dock.ui
    ${ui_root}/connections_dock.ui
    ${ui_root}/new_connections_dialog.ui
    ${ui_root}/nodes_dock.ui)
# ----------------------------------------------------------------------- #
# ------------------------- Qt libraries configuration ------------------ #
# ----------------------------------------------------------------------- #
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS ${ui_root})
# ----------------------------------------------------------------------- #
# -------------------------- Find external libraries -------------------- #
# ----------------------------------------------------------------------- #
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Qml)
find_package(qtils REQUIRED COMPONENTS pointer model dialog delegate serializer
                                       property_browser)
# ----------------------------------------------------------------------- #
# -------------------------- Create plugin shared lib ------------------- #
# ----------------------------------------------------------------------- #
pillar_add_plugins(
  flow_document
  SOURCES
  ${sources}
  ${includes}
  ${resources}
  DEPENDS
  Qt6::Core
  Qt6::Widgets
  pillar::pillar
  qtils::pointer
  DEPENDS_PRIVATE
  qtils::model
  qtils::dialog
  qtils::delegate
  qtils::serializer
  qtils::property_browser)

target_include_directories(
  flow_document
  PUBLIC $<BUILD_INTERFACE:${PILLAR_SOURCE_DIR}/plugins/include>
  INTERFACE $<INSTALL_INTERFACE:include>
  PRIVATE ${PILLAR_SOURCE_DIR}/plugins/src)

pillar_add_translations(
  flow_document
  QM_DIR
  ${resource_root}/translations
  DIRS
  ${include_root}
  ${source_root}
  TS_FILES
  ${resource_root}/translations/document_pl.ts
  ${resource_root}/translations/document_en.ts)
# ----------------------------------------------------------------------- #
