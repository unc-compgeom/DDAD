QuadEdge::Edge* Terrain_3r::MakeFaceEdge(QuadEdge::Face *f,
                                         QuadEdge::Vertex *org,
                                         QuadEdge::Vertex *dest) {
    SigPopFace(f);
    QuadEdge::Edge* e = terrain_->makeFaceEdge(f, org, dest);
    SigPushFace(e->Left());
    SigPushFace(e->Right());
    SigPushEdge(e);
    return e;
}