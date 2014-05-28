##########################################################################
#
#  Degree Driven Algorithm Library code structure:
#
#   * base - contains functions helpful for the whole project
#       project such as logging libs, contstans, error types etc.
#
#   * utility - contains utility functions that are for the most part
#       independent of ddad.
#
#   * primitives - contains all the primitives such as representations
#       of points, lines, segments, etc.
#
#   * predicates - contains all predicate implemenation as well as code
#       required to ensure exact evaluation
#
#   * data structures -
###############################################################################

add_library(ddad
##########################################################################
# files of: the base
##########################################################################
    DDAD_base.cpp
    DDAD_rlog.h

##########################################################################
# files of: utility
##########################################################################
    Timer.h
    Radix_sort.h

##########################################################################
# files of: primitives
##########################################################################
    Point_2.cpp
    Line_2.cpp
    Segment_2.cpp
    Poly_chain_2.cpp

##########################################################################
# files of: predicates
##########################################################################
    Algebraic_predicates.h
    Predicates_2.cpp
    Constructions_2.cpp

##########################################################################
# files of: data structures
##########################################################################
)

