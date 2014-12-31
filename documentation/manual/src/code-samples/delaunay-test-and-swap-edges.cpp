void Terrain_3r::TestAndSwapEdges(std::vector<QuadEdge::Edge*>& edges,
                                  const Point_3r& sample) {
    while (!edges.empty()) {
        QuadEdge::Edge *e1 = edges.back();
        edges.pop_back();
        QuadEdge::Vertex *v1 = e1->Org(), *v2 = e1->Dest();
        QuadEdge::Edge *e2 = e1->Lnext();
        QuadEdge::Edge *e3 = e1->Lprev();
        QuadEdge::Vertex *v3 = e2->Dest();
        QuadEdge::Edge *e4 = e1->Rnext();
        QuadEdge::Edge *e5 = e1->Rprev();

        if (Predicate::InCircle(*v1->pos, *v2->pos, *v3->pos, sample) > 0) {
            QuadEdge::Face *left = e1->Left();
            KillFaceEdge(e1);
            MakeFaceEdge(left, e2->Dest(), e5->Dest());
            edges.push_back(e2->Sym());
            edges.push_back(e3->Sym());
        }
    }
}