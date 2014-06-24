#==============================================================================
# @author Clinton Freeman
# @date 2014-03-18
#==============================================================================

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)
project(DDAD)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
   # Mac OS X specific code
    include_directories(${DDAD_SOURCE_DIR}/dependencies/mpir/Mac)
    link_directories(${DDAD_SOURCE_DIR}/dependencies/mpir/Mac)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

IF(${WIN32})
   # Windows specific code
    include_directories(${DDAD_SOURCE_DIR}/dependencies/mpir/Win32/Debug)
    link_directories(${DDAD_SOURCE_DIR}/dependencies/mpir/Win32/Debug)
ENDIF(${WIN32})

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /INCREMENTAL:NO" )

set(QT_INSTALL_DIR "" CACHE PATH "Path to Qt installation directory.")
set(WINSDK_DIR "" CACHE PATH "Path to Windows SDK directory.")
set(CMAKE_PREFIX_PATH ${QT_INSTALL_DIR}; ${WINSDK_DIR})

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Gui)
find_package(Qt5OpenGL)

add_subdirectory(logging)
add_subdirectory(geometry)
add_subdirectory(workbench)
add_subdirectory(tests)
