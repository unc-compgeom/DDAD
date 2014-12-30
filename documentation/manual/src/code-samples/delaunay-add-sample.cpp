void Terrain_3r::AddSample(const Point_3r& sample) {
    SharedPoint_3r sample_r = std::make_shared<Point_3r>(
        sample.x(), sample.y(), sample.z()
    );
    SigRegisterPoint_3r(*sample_r);

    // find the triangle containing the sample
    QuadEdge::Edge *e1 = LocalizePoint(*sample_r);
    QuadEdge::Edge *e2 = e1->Lnext();
    QuadEdge::Edge *e3 = e1->Lprev();
    QuadEdge::Vertex *v1 = e1->Org();
    QuadEdge::Vertex *v2 = e1->Dest();
    QuadEdge::Vertex *v3 = e2->Dest();
    QuadEdge::Face *f = e1->Left();

    // stick the new sample into the containing triangle and update topology
    QuadEdge::Edge *enew1 = MakeFaceEdge(f, v1, v2);
    QuadEdge::Face *f2 = enew1->Right();
    QuadEdge::Vertex *vnew = MakeVertexEdge(v2, f2, f, sample_r)->Dest();
    QuadEdge::Edge *enew2 = MakeFaceEdge(f, vnew, v3);

    std::vector<QuadEdge::Edge*> neighbors;
    neighbors.push_back(e1->Sym());
    neighbors.push_back(e2->Sym());
    neighbors.push_back(e3->Sym());
    TestAndSwapEdges(neighbors, *sample_r);
}