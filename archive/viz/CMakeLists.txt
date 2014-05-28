###############################################################################
#
#  Visualize Degree Driven Algorithm Library code structure:
#
#   * ddad base - contains any global data types common accross visualizations
#
#   * data structures - viz routines for data structures
###############################################################################

add_library(ddad_viz
###############################################################################
# files of: the base
###############################################################################
    DDAD_drawer.h OGL.h

###############################################################################
# files of: data structures
###############################################################################
    Convex_hull_drawer.cpp
    Discrete_upper_envelope_drawer.cpp
    NN_transform_drawer.cpp
#Voronoi_polygon_set_drawer.cpp
#Quad_edge_ds_drawer.cpp
#Gabriel_graph_drawer.cpp
#Arrangement_drawer.cpp
)
