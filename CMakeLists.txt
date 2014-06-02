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


add_subdirectory(logging)
add_subdirectory(geometry)
add_subdirectory(workbench)
