include_directories(${DEGDRIVENALGODESIGN_SOURCE_DIR}/src)

set(LIBRARIES ${CORE_LIBS})
set(DEMOS
    demo_Convex_hull
    demo_Discrete_upper_envelope
    demo_NN_transform
    #    demo_Voronoi_polygon_set
    #    demo_Quad_edge_ds
    #    demo_Gabriel_graph
    #    demo_Arrangement
)

# build demos
foreach(demo ${DEMOS})
    add_executable(${demo} ${demo}.cpp)
    target_link_libraries(${demo} ${LIBRARIES})
endforeach(demo)
