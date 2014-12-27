set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(QT_INSTALL_DIR "" CACHE PATH "Path to Qt installation directory.")
set(WINSDK_DIR "" CACHE PATH "Path to Windows SDK directory.")
set(CMAKE_PREFIX_PATH ${QT_INSTALL_DIR}; ${WINSDK_DIR})

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Gui)
find_package(Qt5OpenGL)

link_directories(${DDAD_BINARY_DIR}/geometry)

qt5_wrap_ui(
    UI_HEADERS
    forms/dialog_about.ui
    forms/dialog_preferences.ui
    forms/window_main.ui
    forms/point_set_creation_method.ui
    forms/point_set_algorithms.ui
    forms/polyline_creation_method.ui
    forms/polyline_algorithms.ui
    forms/polytope_creation_method.ui
    forms/polytope_algorithms.ui
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
    qt_point_set_creation_method.cpp
    qt_point_set_algorithms.cpp
    qt_polyline_creation_method.cpp
    qt_polyline_algorithms.cpp
    qt_polytope_creation_method.cpp
    qt_polytope_algorithms.cpp
    scene.cpp
    common.h
    resources/icon.rc
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
)
