cmake_minimum_required(VERSION 2.6)
project(degDrivenAlgoDesign)

################################################################################
# set the compile flags for a specific run type
################################################################################
# run types are profile, time and debug
#set(run_type "time")
#set(run_type "profile")
set(run_type "debug")

################################################################################
# set the include and linking directories
################################################################################
include_directories( /usr/include /usr/local/include)
link_directories()

################################################################################
# toggle visual debugging
################################################################################
#set(is_visual_debugging true)

################################################################################
# toggle logging
################################################################################
set(is_logging true)

################################################################################
# toggle cmake variables
################################################################################
set(CMAKE_VERBOSE_MAKEFILE false)

################################################################################
# set the compiler and linker flags
################################################################################
if (runtype STREQUAL "debug")
    set(CMAKE_CXX_COMPILER "clang")
endif (runtype STREQUAL "debug")

if (run_type STREQUAL "profile")
    set(CMAKE_CXX_FLAGS "-g -DNDEBUG -fpermissive -Wall -O1")
elseif (run_type STREQUAL "time")
    set(CMAKE_CXX_FLAGS "-DNDEBUG -fpermissive -Wall -O3")
elseif (run_type STREQUAL "debug")
    set(CMAKE_CXX_FLAGS "-g -fpermissive -Wall -O3")
else ()
    set(CMAKE_CXX_FLAGS "-fpermissive -Wall")
endif ()

set(CMAKE_EXE_LINKER_FLAGS "")
set(CORE_LIBS "")

################################################################################
# setup/disable rLog support
################################################################################
# we always disable logging when timing
if (run_type STREQUAL "time" OR run_type STREQUAL "profile")
    set(is_logging false)
endif (run_type STREQUAL "time" OR run_type STREQUAL "profile")

if (is_logging)
    set(RLOG_ON true)
else(is_logging)
    set(RLOG_ON false)
endif(is_logging)

if (RLOG_ON)
    message(STATUS "Logging with rlog")
    string(LENGTH ${CMAKE_HOME_DIRECTORY} NUM_CHAR_TO_TRIM)
    add_definitions(-DPATH_TRIM=${NUM_CHAR_TO_TRIM} -DDDAD_LOGGING_ON)
    link_libraries(rlog)
else (RLOG_ON)
    message(STATUS "Stubbing out rlog")
    string(LENGTH ${CMAKE_HOME_DIRECTORY} NUM_CHAR_TO_TRIM)
    add_definitions(-DPATH_TRIM=${NUM_CHAR_TO_TRIM})
endif(RLOG_ON)

################################################################################
# setup/disable gtest support
################################################################################
link_libraries(gtest pthread)

################################################################################
# setup/disable openMP support
################################################################################
#find_package(OpenMP)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OPENMP_CXX_FLAGS}")
## TODO: Add OpenMP correctly
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fgomp")

################################################################################
# setup DDAD library and compiler flags
################################################################################
add_subdirectory(base)
link_libraries(ddad)
add_subdirectory(algos)
link_libraries(ddad_algos)
add_subdirectory(test)
add_subdirectory(experiment)


################################################################################
# setup/disable graphics support
################################################################################
find_package(GLUT)
find_package(OpenGL)
if (NOT GLUT_FOUND OR NOT OPENGL_FOUND OR NOT OPENGL_GLU_FOUND)
    message(STATUS "Could NOT find OpenGL, GLU and GLUT -- Graphics disabled")
    add_definitions(-DDDAD_GRAPHICS_DISABLED)

    # disable visual debugger if it is enabled
    if (IS_VISUAL_DEBUGGING)
        message(STATUS
            "Warning: Visual debugging requested but graphics is disabled.
            Visual debugging will be disabled as well.  To remove this status
            message, please, set the IS_VISUAL_DEBUGGING variable to false.")
        set(IS_VISUAL_DEBUGGING false)
    endif(IS_VISUAL_DEBUGGING)
else()
    message(STATUS "OpenGL, GLU and GLUT found -- Graphics enabled")
    # if requested set up visual debugger
    if (IS_VISUAL_DEBUGGING)
        add_definitions(-DDDAD_VISUAL_DEBUGGER_ON)
    endif (IS_VISUAL_DEBUGGING)

    # add src directories with vizualization and linking libraries
    set(CORE_LIBS ${CORE_LIBS} ${GLUT_LIBRARY} ${OPENGL_LIBRARY} m)
    add_subdirectory(viz)
    link_libraries(ddad_viz)
    add_subdirectory(demo)
    add_subdirectory(apps_graphical)
endif(NOT GLUT_FOUND OR NOT OPENGL_FOUND OR NOT OPENGL_GLU_FOUND)


################################################################################
# non-graphical apps
################################################################################
#add_subdirectory(app_nongraphical)

# TODO: add cmake script for finding rlog and gtest
#       dependancies and/or write a script to strip
#       out logging from the library
#



