include_directories(${DEGDRIVENALGODESIGN_SOURCE_DIR}/src)

set(LIBRARIES ${CORE_LIBS})
set(GRAPHICAL_APPS
    nn_transform
)

# build apps
foreach(app ${GRAPHICAL_APPS})
    add_executable(${app} ${app}.cpp)
    target_link_libraries(${app} ${LIBRARIES})
endforeach(app)
