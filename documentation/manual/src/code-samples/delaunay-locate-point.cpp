QuadEdge::Edge* Terrain_3r::LocalizePoint(const Point_3r& sample) {
    QuadEdge::CellFaceIterator faces(terrain_);
    QuadEdge::Face *f = nullptr;
    while ((f = faces.next())) {
        QuadEdge::Edge *e1 = f->getEdge();
        QuadEdge::Edge *e2 = e1->Lnext();
        QuadEdge::Vertex *v1 = e1->Org();
        QuadEdge::Vertex *v2 = e1->Dest();
        QuadEdge::Vertex *v3 = e2->Dest();
        if (Predicate::Orient2D(*v1->pos, *v2->pos, sample) >= 0 &&
            Predicate::Orient2D(*v2->pos, *v3->pos, sample) >= 0 &&
            Predicate::Orient2D(*v3->pos, *v1->pos, sample) >= 0) {
            return e1;
        }
    }

    return nullptr;
}