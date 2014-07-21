void Polyline_2r::push_back(SharedPoint_2r v) {
    vertices_.push_back(v);

    SigRegisterPoint_2r(*v);
    SigPushVisualPoint_2r(*v, Visual::Point(mat_vertex_));

    if (vertices_.size() > 1) {
        SigPushVisualSegment_2r(Segment_2r(back(1), back(0)),
                                Visual::Segment(mat_edge_), 1000);
    }
}