#include "arithmetic.h"
#include "line.h"
#include "intersection.h"
#include "arrangement.h"


namespace DDAD {


//=============================================================================
// Implementation: Arrangement_2r
//=============================================================================

Arrangement_2r::~Arrangement_2r() {
    for(auto i = begin(segments_); i != end(segments_); ++i) {
        SigPopVisualPoint_2r(i->p());
        SigPopVisualPoint_2r(i->q());
        SigPopVisualSegment_2r(*i);
    }
}

void Arrangement_2r::AddSegment(Point_2r& v, Point_2r& w, bool color){
    segments_.push_front(Segment_2r_colored(v, w, color));
    //Create visual point and segment
    Visual::Material vMat;
    vMat.set_ambient(Visual::Color(255*color, 0, 255*(!color), 255));   //if color = 1, set to red, else set to blue
    Visual::Point vPoint(vMat);
    Visual::Segment vSeg(vMat);

    //Push the segment
    SigPushVisualPoint_2r(v,vPoint);
    SigPushVisualPoint_2r(w,vPoint);
    SigPushVisualSegment_2r(segments_.front(), vSeg);
}

void Arrangement_2r::EndSegment(Point_2r &v){
    AddSegment(floater_, v, current_color_);
}

void Arrangement_2r::PushPoint(Point_2r &v, bool color){
    PushPoint(std::make_shared<Point_2r>(v), color);
}

void Arrangement_2r::PushPoint(SharedPoint_2r v, bool color){
    floater_ = *v;
    current_color_ = color;

    //Create visual point
    Visual::Material vMat;
    vMat.set_ambient(Visual::Color(255*color, 0, 255*(!color), 255));   //if color = 1, set to red, else set to blue
    Visual::Point vPoint(vMat);

    //Push the point
    SigPushVisualPoint_2r(floater_,vPoint);
}

void Arrangement_2r::PopPoint(){
    SigPopVisualPoint_2r(floater_);
}

const std::list<Segment_2r_colored>& Arrangement_2r::segments() const{
    return segments_;
}

} // namespace DDAD


//PolyChain_2r::PolyChain_2r() :
//    closed_(false) {}

//PolyChain_2r::~PolyChain_2r() {
//    for(auto i = begin(vertices_); i != end(vertices_); ++i) {
//        SigPopVisualPoint_2r(i->vertex());

//        if(i != std::prev(end(vertices_))) {
//            SigPopVisualSegment_2r(i->edge_next());
//        }
//    }
//}

//void PolyChain_2r::AppendVertex(const Point_2r& v) {
//    AppendVertex(std::make_shared<Point_2r>(v));
//}

//void PolyChain_2r::AppendVertex(SharedPoint_2r v) {
//    PolyChainVertex_2r chain_vertex(v);
//    vertices_.push_back(chain_vertex);

//    Visual::Material vMat;
//    //vMat.set_ambient(Visual::Color(0, 151, 255, 255));
//    Visual::Point vPoint(vMat);
//    Visual::Segment vSeg(vMat);

//    SigRegisterPoint_2r(*chain_vertex.vertex_sptr());
//    SigPushVisualPoint_2r(chain_vertex.vertex(), vPoint);

//    if (vertices_.size() > 1) {
//        auto e0 = std::prev(end(vertices_), 2);
//        auto e1 = std::prev(end(vertices_), 1);
//        Segment_2r edge(e0->vertex_sptr(), e1->vertex_sptr());
//        e0->set_edge_next(edge);
//        e1->set_edge_prev(edge);
//        SigPushVisualSegment_2r(edge, vSeg);
//    }
//}

//void PolyChain_2r::RemoveFront() {
//    SigPopVisualPoint_2r(vertices_.front().vertex());
//    SigPopVisualSegment_2r(vertices_.front().edge_next(), 2000);
//    vertices_.pop_front();
//    begin(vertices_)->set_edge_prev(Segment_2r());
//}

//void PolyChain_2r::Close() {
//    auto e0 = std::prev(end(vertices_), 1);
//    auto e1 = begin(vertices_);
//    Segment_2r edge(e0->vertex_sptr(), e1->vertex_sptr());
//    e0->set_edge_next(edge);
//    e1->set_edge_prev(edge);
//    SigPushVisualSegment_2r(edge, Visual::Segment());
//    set_closed(true);
//}

//void PolyChain_2r::RotateToMaxX() {
//    bool (*compare)(const PolyChainVertex_2r&,
//                    const PolyChainVertex_2r&) = Predicate::AIsLeftOfB;
//    auto vmax_x = std::max_element(vertices_.begin(), vertices_.end(),
//                                   compare);
//    std::rotate(vertices_.begin(), vmax_x, vertices_.end());
//}

//PolyChainVertex_2r& PolyChain_2r::back() {
//    return vertices_.back();
//}

//PolyChainVertex_2r& PolyChain_2r::front() {
//    return vertices_.front();
//}

//const std::list<PolyChainVertex_2r>& PolyChain_2r::vertices() const {
//    return vertices_;
//}

//std::list<PolyChainVertex_2r>& PolyChain_2r::vertices(){
//    return vertices_;
//}

//const bool PolyChain_2r::closed() const {
//    return closed_;
//}
//void PolyChain_2r::set_vertices(const std::list<PolyChainVertex_2r>& vertices) {
//    vertices_ = vertices;
//}
//void PolyChain_2r::set_closed(const bool closed) {
//    closed_ = closed;
//}
