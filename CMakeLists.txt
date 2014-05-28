#==============================================================================
# @author Clinton Freeman
# @date 2014-03-18
#==============================================================================

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)
 
project(DDAD)

include_directories(${DDAD_SOURCE_DIR}/dependencies/mpir/Win32/Debug)
link_directories(${DDAD_SOURCE_DIR}/dependencies/mpir/Win32/Debug)

add_subdirectory(logging)
add_subdirectory(geometry)
add_subdirectory(workbench)
