/*!
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 2013-07-21
 */

#include "common.h"
#include "arithmetic.h"
#include "line.h"
#include "intersection.h"
#include "polygon.h"
#include "wedge.h"

namespace DDAD {

//=============================================================================
// Melkman's algorithm
//=============================================================================

Polygon_2rDq Melkman(const PolyChain_2r P,
      Visual::IGeometryObserver* observer){
    LOG(INFO) << "Melkman";
    Polygon_2rDq D;
    std::list<PolyChainVertex_2r> vertices = P.vertices();
    std::list<PolyChainVertex_2r>::iterator it = vertices.begin();
    SharedPoint_2r v0 = (*it).vertex_sptr();
    SharedPoint_2r v1 = (*it).vertex_sptr();
    SharedPoint_2r pi;
    D.PushFront(v1);
    D.PushFront(v0);
    D.PushFront(v1);

    for(std::list<PolyChainVertex_2r>::iterator ii = it; ii != vertices.end(); ii ++){
        SharedPoint_2r pi = (*ii).vertex_sptr();
        LOG(INFO) << "test";

        Line_2r topEdge = Line_2r(D[0], D[1]);
        Ray_2r topRay = Ray_2r(D[1], topEdge.V());
        Line_2r botEdge = Line_2r(D[D.NumVertices()-2], D[D.NumVertices()-1]);
        Ray_2r botRay = Ray_2r(D[D.NumVertices()-1], botEdge.V());


        if((Predicate::AIsRightOfB(*pi, topEdge) || Predicate::AIsAheadOfB(*pi, topRay)) ||
        (Predicate::AIsRightOfB(*pi, botEdge) || Predicate::AIsAheadOfB(*pi, botRay))){
            while(Predicate::AIsRightOfB(*pi, topEdge) || Predicate::AIsAheadOfB(*pi, topRay)){
                // Pop top
                topEdge = Line_2r(D.PopFront(), topEdge.p_sptr());
                topRay = Ray_2r(topEdge.q_sptr(), topEdge.V());
            }
            while(Predicate::AIsRightOfB(*pi, botEdge) || Predicate::AIsAheadOfB(*pi, botRay)){
                // Pop bot
                botEdge = Line_2r(D.PopBack(), botEdge.p_sptr());
                botRay = Ray_2r(botEdge.q_sptr(), botEdge.V());
            }
            D.PushBack(pi);
            D.PushFront(pi);
        }
    }
    return D;
}



//=============================================================================
// Polygon_2rDq
//=============================================================================

Polygon_2rDq::Polygon_2rDq() {}

Polygon_2rDq::~Polygon_2rDq() {}

void Polygon_2rDq::PushFront(SharedPoint_2r v){
    boundary_.push_front(v);
}

void Polygon_2rDq::PushBack(SharedPoint_2r v){
    boundary_.push_back(v);
}

SharedPoint_2r Polygon_2rDq::PopFront(){
    SharedPoint_2r v = boundary_.front();
    boundary_.pop_front();
    return v;
}

SharedPoint_2r Polygon_2rDq::PopBack(){
    SharedPoint_2r v = boundary_.back();
    boundary_.pop_back();
    return v;
}
const size_t Polygon_2rDq::NumVertices() const {
    return boundary_.size();
}

SharedPoint_2r Polygon_2rDq::operator [](int index) {
    return boundary_[index];
}


//=============================================================================
// Polygon_2r
//=============================================================================

Polygon_2r::Polygon_2r() {
    boundary_.AddObserver(this);
}

Polygon_2r::~Polygon_2r() {
    for (auto i = begin(triangulation_); i != end(triangulation_); ++i) {
        SigPopVisualTriangle_2r(*i);
    }
}

void Polygon_2r::AppendVertexToBoundary(const Point_2r& v) {
    AppendVertexToBoundary(std::make_shared<Point_2r>(v));
}

void Polygon_2r::AppendVertexToBoundary(SharedPoint_2r v) {
    boundary_.AppendVertex(v);

    if (boundary_.vertices().size() > 2) {
        Triangle_2r t(std::prev(end(boundary_.vertices_), 1)->vertex_sptr(),
                      begin(boundary_.vertices_)->vertex_sptr(),
                      std::prev(end(boundary_.vertices_), 2)->vertex_sptr());
        triangulation_.push_back(t);
        Visual::Triangle vt;
        //vt.set_diffuse(diffuse_);
        SigPushVisualTriangle_2r(t, vt);
    }
}

/*!
 * @brief Polygon_2r::ComputeIntegerHull
 */
void Polygon_2r::ComputeIntegerHull() {
    // canonicalize the boundary chain
    boundary_.RotateToMaxX();

    // grab the rightmost vertex
    auto vert_itr_max_x = begin(boundary_.vertices());

    // find the bottommost vertex
    bool (*cmp)(const PolyChainVertex_2r&,
                const PolyChainVertex_2r&) = Predicate::AIsBelowB;
    auto vert_itr_min_y = std::min_element(begin(boundary_.vertices()),
                                           end(boundary_.vertices()), cmp);

    // construct an initial z and use it to init the tentative integer hull
    Point_2r z(Floor(vert_itr_max_x->vertex().x()),
               Floor(vert_itr_min_y->vertex().y()));
    PolyChain_2r tentative_hull;
    tentative_hull.AddObserver(this);
    tentative_hull.AppendVertex(z);

    // initialize wedge stack
    WedgeStack_2r wstack;
    wstack.AddObserver(this);
    wstack.AttachToOrigin(tentative_hull.back().vertex_sptr());
    wstack.Push(std::make_shared<Wedge_2r>(Vector_2r( 0, 1), Vector_2r(-1, 0)));
    wstack.Push(std::make_shared<Wedge_2r>(Vector_2r( 1, 0), Vector_2r( 0, 1)));
    wstack.Push(std::make_shared<Wedge_2r>(Vector_2r( 0,-1), Vector_2r( 1, 0)));
    wstack.Push(std::make_shared<Wedge_2r>(Vector_2r(-1, 0), Vector_2r( 0,-1)));
    wstack.Push(std::make_shared<Wedge_2r>(Vector_2r( 0, 1), Vector_2r(-1, 0)));
    wstack.Push(std::make_shared<Wedge_2r>(Vector_2r( 1, 0), Vector_2r( 0, 1)));

    auto vert_itr_P = vert_itr_max_x;
    auto L = Line_2r(SLOPE_PINFINITY, vert_itr_P->vertex().x());

    while(!wstack.empty()) {
        auto wtop = wstack.Pop();
        auto ray_zv = Ray_2r(tentative_hull.back().vertex_sptr(), wtop->v());
        auto isect_L_zv = Intersection::Line_2rRay_2r(&L, &ray_zv);

        if(Predicate::AreParallel(L, ray_zv) || !Predicate::IsEmpty(isect_L_zv)) {
            while(Predicate::OrientationPQR(ray_zv, vert_itr_P->vertex()) != ORIENTATION_LEFT) {
                if(vert_itr_P == std::prev(end(boundary_.vertices())))
                    vert_itr_P = begin(boundary_.vertices());
                else
                    vert_itr_P++;

                L = vert_itr_P->edge_prev().support();

                if(Predicate::AIsRightOfB(z, L))
                    return;
            }
            z = Construction::LastBefore(L, ray_zv);
            tentative_hull.AppendVertex(z);
            wstack.AttachToOrigin(tentative_hull.back().vertex_sptr());
        } else {
            wstack.Push(std::make_shared<Wedge_2r>(wtop->u()+wtop->v(), wtop->v()));
            wstack.Push(std::make_shared<Wedge_2r>(wtop->u(), wtop->u()+wtop->v()));
        }
    }

    do {
        tentative_hull.RemoveFront();
    } while(begin(tentative_hull.vertices())->vertex() !=
            std::prev(end(tentative_hull.vertices()))->vertex());
}


void Polygon_2r::CloseBoundary() {
    boundary_.Close();
}

const PolyChain_2r& Polygon_2r::boundary() const {
    return boundary_;
}

const size_t Polygon_2r::NumVertices() const {
    return boundary().vertices().size();
}

//=============================================================================
// Implementation: PolyChain_2r
//=============================================================================

PolyChain_2r::PolyChain_2r() :
    closed_(false) {}

PolyChain_2r::~PolyChain_2r() {
    for(auto i = begin(vertices_); i != end(vertices_); ++i) {
        SigPopVisualPoint_2r(i->vertex());

        if(i != std::prev(end(vertices_))) {
            SigPopVisualSegment_2r(i->edge_next());
        }
    }
}

void PolyChain_2r::AppendVertex(const Point_2r& v) {
    AppendVertex(std::make_shared<Point_2r>(v));
}

void PolyChain_2r::AppendVertex(SharedPoint_2r v) {
    PolyChainVertex_2r chain_vertex(v);
    vertices_.push_back(chain_vertex);

    Visual::Material vMat;
    //vMat.set_ambient(Visual::Color(0, 151, 255, 255));
    Visual::Point vPoint(vMat);
    Visual::Segment vSeg(vMat);

    SigRegisterPoint_2r(*chain_vertex.vertex_sptr());
    SigPushVisualPoint_2r(chain_vertex.vertex(), vPoint);

    if (vertices_.size() > 1) {
        auto e0 = std::prev(end(vertices_), 2);
        auto e1 = std::prev(end(vertices_), 1);
        Segment_2r edge(e0->vertex_sptr(), e1->vertex_sptr());
        e0->set_edge_next(edge);
        e1->set_edge_prev(edge);
        SigPushVisualSegment_2r(edge, vSeg);
    }
}

void PolyChain_2r::RemoveFront() {
    SigPopVisualPoint_2r(vertices_.front().vertex());
    SigPopVisualSegment_2r(vertices_.front().edge_next(), 2000);
    vertices_.pop_front();
    begin(vertices_)->set_edge_prev(Segment_2r());
}

void PolyChain_2r::Close() {
    auto e0 = std::prev(end(vertices_), 1);
    auto e1 = begin(vertices_);
    Segment_2r edge(e0->vertex_sptr(), e1->vertex_sptr());
    e0->set_edge_next(edge);
    e1->set_edge_prev(edge);
    SigPushVisualSegment_2r(edge, Visual::Segment());
    set_closed(true);
}

void PolyChain_2r::RotateToMaxX() {
    bool (*compare)(const PolyChainVertex_2r&,
                    const PolyChainVertex_2r&) = Predicate::AIsLeftOfB;
    auto vmax_x = std::max_element(vertices_.begin(), vertices_.end(),
                                   compare);
    std::rotate(vertices_.begin(), vmax_x, vertices_.end());
}

PolyChainVertex_2r& PolyChain_2r::back() {
    return vertices_.back();
}

PolyChainVertex_2r& PolyChain_2r::front() {
    return vertices_.front();
}

const std::list<PolyChainVertex_2r>& PolyChain_2r::vertices() const {
    return vertices_;
}

std::list<PolyChainVertex_2r>& PolyChain_2r::vertices(){
    return vertices_;
}

const bool PolyChain_2r::closed() const {
    return closed_;
}
void PolyChain_2r::set_vertices(const std::list<PolyChainVertex_2r>& vertices) {
    vertices_ = vertices;
}
void PolyChain_2r::set_closed(const bool closed) {
    closed_ = closed;
}

//=============================================================================
// Implementation: PolyChainVertex_2r
//=============================================================================

PolyChainVertex_2r::PolyChainVertex_2r() {}

PolyChainVertex_2r::PolyChainVertex_2r(const Point_2r& vertex) :
    vertex_(std::make_shared<Point_2r>(vertex)) {}

PolyChainVertex_2r::PolyChainVertex_2r(SharedPoint_2r vertex) :
    vertex_(vertex) {}

// Predicates =================================================================

namespace Predicate {

bool AIsBelowB(const PolyChainVertex_2r& a, const PolyChainVertex_2r& b) {
    return AIsBelowB(a.vertex(), b.vertex());
}

bool AIsLeftOfB(const PolyChainVertex_2r& a, const PolyChainVertex_2r& b) {
    return AIsLeftOfB(a.vertex(), b.vertex());
}

} // namespace Predicate

// Accessors/Mutators =========================================================

const Point_2r& PolyChainVertex_2r::vertex() const {
    return *vertex_;
}
const Segment_2r& PolyChainVertex_2r::edge_next() const {
    return edge_next_;
}
const Segment_2r& PolyChainVertex_2r::edge_prev() const {
    return edge_prev_;
}
SharedPoint_2r PolyChainVertex_2r::vertex_sptr() {
    return vertex_;
}
void PolyChainVertex_2r::set_vertex(const Point_2r& v) {
    *vertex_ = v;
}
void PolyChainVertex_2r::set_edge_next(const Segment_2r& e) {
    edge_next_ = e;
}
void PolyChainVertex_2r::set_edge_prev(const Segment_2r& e) {
    edge_prev_ = e;
}
void PolyChainVertex_2r::set_vertex_sptr(SharedPoint_2r v) {
    vertex_ = v;
}

} // namespace DDAD
