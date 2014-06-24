#==============================================================================
# @author Clinton Freeman
# @date 2014-03-18
#==============================================================================



#include_directories(${Workbench_SOURCE_DIR}/logging)
#include_directories(${Workbench_SOURCE_DIR}/geometry)

link_directories(${Workbench_BINARY_DIR}/logging)
link_directories(${Workbench_BINARY_DIR}/geometry)

qt5_wrap_ui(
    UI_HEADERS
    forms/dialog_about.ui
    forms/dialog_preferences.ui
    forms/window_main.ui
)

qt5_add_resources(
    UI_RESOURCES
    resources/resources.qrc
)

add_executable(
    workbench
    config.cpp
    grid.cpp
    logger.cpp
    main.cpp
    opengl.cpp
    qt_dialog_about.cpp
    qt_dialog_preferences.cpp
    qt_widget_orthographic.cpp
    qt_widget_perspective.cpp
    qt_window_main.cpp
    scene.cpp
    common.h
    ${UI_HEADERS}
    ${UI_RESOURCES}
)

qt5_use_modules(
    workbench
    Core
    Gui
    Widgets
    OpenGL
)

target_link_libraries(
    workbench
    geometry
    mpir
    mpirxx
#    test
)
