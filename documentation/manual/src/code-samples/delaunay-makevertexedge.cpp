QuadEdge::Edge* Terrain_3r::MakeVertexEdge(QuadEdge::Vertex *v,
                                           QuadEdge::Face *left,
                                           QuadEdge::Face *right,
                                           SharedPoint_3r vnew_pos) {

    // we need to pop all faces and edges on the ccw traveral from the left
    // face to the right face in v's orbit. the vertexedgeiterator will begin
    // at a random edge in the orbit, and we have no way of determining whether
    // we are inside or outside the left-to-right range on the first pass
    // through the edges. the iterator will give us the edges back in ccw order
    // however.

    // the strategy is to place all edges in v's orbit into a list and rotate
    // the list so that the leftmost edge to pop is the first element.
    std::list<QuadEdge::Edge*> rotated_orbit;

    // iterate through the orbit and track the leftmost edge insertion index.
    size_t right_idx = 0;
    size_t i = 0;
    QuadEdge::VertexEdgeIterator orbit(v);
    QuadEdge::Edge *e;
    while ((e = orbit.next()) != 0) {
        if (e->Right() == right) {
            right_idx = i;
        }
        rotated_orbit.push_back(e);
        ++i;
    }

    // move the left-right range to the beginning of list.
    std::rotate(begin(rotated_orbit),
                std::next(begin(rotated_orbit), right_idx),
                end(rotated_orbit));

    // pop faces and edges in left-right range.
    for (auto edge : rotated_orbit) {
        SigPopFace(edge->Right());
        if (edge->Right() == left) {
            break;
        }
        SigPopEdge(edge);
    }

    // make topological changes to QuadEdge cell, set new vertex position.
    QuadEdge::Edge* enew = terrain_->makeVertexEdge(v, left, right);
    QuadEdge::Vertex* vnew = enew->Dest();
    vnew->pos = vnew_pos;

    // we assume vnew_pos is already registered, so just push the vertex.
    SigPushVertex(vnew);

    // push all new edges and faces.
    QuadEdge::VertexEdgeIterator orbitnew(vnew);
    while ((e = orbitnew.next()) != 0) {
        SigPushEdge(e);
        SigPushFace(e->Left());
    }

    return enew;
}
