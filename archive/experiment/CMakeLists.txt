include_directories(${DEGDRIVENALGODESIGN_SOURCE_DIR}/src)

set(EXPERIMENTS
    time_convex_hull
    time_nn_transform
    nnt_vary_u_and_delta
    nnt_mpeg7
)

# build experiments
foreach(experiment ${EXPERIMENTS})
    add_executable(${experiment} ${experiment}.cpp)
    target_link_libraries(${experiment} ${LIBRARIES})
endforeach(experiment)

