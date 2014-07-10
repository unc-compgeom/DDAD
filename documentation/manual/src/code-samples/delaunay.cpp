    void Initialize(const std::vector<Point_3f>& samples) {
        assert(!samples.empty());

        // setup bounding triangle topology
        cell_ = QuadEdge::Cell::make();

        QuadEdge::CellVertexIterator iter(cell_);
        QuadEdge::Vertex *v1 = iter.next();

        QuadEdge::Edge *a = v1->getEdge();
        QuadEdge::Face *left = a->Left();
        QuadEdge::Face *right = a->Right();

        QuadEdge::Vertex *v2 = cell_->makeVertexEdge(v1, left, right)->Dest();
        QuadEdge::Vertex *v3 = cell_->makeVertexEdge(v2, left, right)->Dest();

        // compute a bounding box of the input points xy coordinates
        float maxx, maxy, maxz, minx, miny, minz;
        minx = maxx = samples.at(0).x();
        miny = maxy = samples.at(0).y();
        minz = maxz = samples.at(0).z();
        for (auto i = begin(samples); i != end(samples); ++i) {
            minx = std::min(i->x(), minx);
            miny = std::min(i->y(), miny);
            minz = std::min(i->z(), minz);
            maxx = std::max(i->x(), maxx);
            maxy = std::max(i->y(), maxy);
            maxz = std::max(i->z(), maxz);
        }

        // compute the max variance along a cardinal axis
        float dx = maxx - minx;
        float dy = maxy - miny;
        float dmax = std::max(dx, dy);
        dmax = std::max(dmax, 10.0f);   // in case of a single point
        float cx = dx * 0.5f;
        float cy = dy * 0.5f;

        // set triangle vertex positions
        v1->pos = std::make_shared<Point_3r>(cx - 20.0f * dmax, cy - dmax, 0);
        SigRegisterPoint_3r(*v1->pos);
        v2->pos = std::make_shared<Point_3r>(cx + 20.0f * dmax, cy - dmax, 0);
        SigRegisterPoint_3r(*v2->pos);
        v3->pos = std::make_shared<Point_3r>(cx, cy + 20.0f * dmax, 0);
        SigRegisterPoint_3r(*v3->pos);


        dummy_ = std::make_shared<Point_3r>(0, 0, 0);
        SigRegisterPoint_3r(*dummy_);
        ConstructViz();

        // incrementally construct the triangulation
        for (auto i = begin(samples); i != end(samples); ++i) {
            ClearViz();
            AddPoint(*i);
            ConstructViz();
        }

        //ClearViz();
        //DeleteSetupVertices(cell_, v1, v2, v3);
        //ConstructViz();
    }