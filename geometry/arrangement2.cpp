#include "arithmetic.h"
#include "line.h"
#include "arrangement.h"

namespace DDAD{

int CountIntersections(const Arrangement_2r &A,
                       Visual::IGeometryObserver *observer)
{
    //# Invariants for the algorithm:
    //#
    //#	- All intersections whose witnesses are left of the sweepline have
    //#    been reported
    //#	- The order of segments along the sweepline is consistent with pushing
    //#    all intersections as far right as possible
    //#
    //# Events:
    //#	- Sweep line reaches an endpoint
    //#
    //# Preconditions:
    //#	- red/red and blue/blue intersections do not exist (such a coloring
    //#    does exist)
    //#
    //# Output:
    //#	- The number of intersections that occur
    //#
    //# Input:
    //#	- set of segments, each colored red or blue
    //#
    int crossings = 0;
    int endpoint_index = 0; //debug
    BundleTree bdt = BundleTree();
    BundleList bdl = BundleList();
    SharedBundle above;
    SharedBundle below;
    std::list<ArrangementVertex_2r> L = A.get_vertices();
    //vertex comparator
    struct compare{
        bool operator()(ArrangementVertex_2r a, ArrangementVertex_2r b){
            if(a.get_x() < b.get_x()){return true;}
            if(a.get_x() > b.get_x()){return false;}
            return a.get_y() < b.get_y();
        }
    };
    // sort vertices lexicographically
    L.sort(compare());
    // generate bounding box sentinels
    bdl.GenerateSentinels(L, bdt);

    for(std::list<ArrangementVertex_2r>::iterator ii = L.begin();
        ii != L.end(); ii ++)
    {
        std::cout << "\nDealing with event " << ii->get_point();

        //


        //
    }
    return crossings;
}

}// namespace DDAD
