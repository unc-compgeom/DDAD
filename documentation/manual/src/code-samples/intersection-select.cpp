void Select() override {
    LOG(DEBUG) << "selecting polyline.";

    Visual::Material selected_mat;
    selected_mat.set_ambient(Visual::Color::SKYBLUE);

    SharedPoint_2r last_vertex;
    for (auto vertex : model_polyline_.vertices()) {
        SigPushVisualPoint_2r(*vertex, Visual::Point(selected_mat, 
            model_polyline_.z_order()));

        if (last_vertex) {
            SigPushVisualSegment_2r(Segment_2r(last_vertex, vertex),
                                    Visual::Segment(selected_mat));
        }

        last_vertex = vertex;
    }
}