Polygon_2r Melkman(const Polyline_2r& P, Visual::IGeometryObserver* observer) {
    Polygon_2r hull;

    Visual::Material hull_mat;
    hull_mat.set_ambient(Visual::Color::MAGENTA);
    hull.set_mat_vertex(hull_mat); hull.set_mat_edge(hull_mat);
    hull.set_z_order(1);
    hull.AddObserver(observer);

    // initialize hull
    hull.push_back(*P[1]); hull.push_back(*P[0]); hull.push_back(*P[1]);

    for (size_t i = 2; i < P.size(); ++i) {
        if (!RIsLeftOrInsidePQ(*hull.back(1), *hull.back(0), *P[i]) ||
            !RIsLeftOrInsidePQ(*hull.front(0), *hull.front(1), *P[i])) {
            while (!RIsLeftOrInsidePQ(*hull.back(1), *hull.back(0), *P[i])) {
                hull.pop_back();
            }
            while (!RIsLeftOrInsidePQ(*hull.front(0), *hull.front(1), *P[i])) {
                hull.pop_front();
            }
            hull.push_back(*P[i]);
            hull.push_front(*P[i]);
        }
    }

    return hull;
}