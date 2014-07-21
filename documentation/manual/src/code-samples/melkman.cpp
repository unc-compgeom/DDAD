Polygon_2r Melkman(const Polyline_2r& P, Visual::IGeometryObserver* observer) {
    Polygon_2r hull;
    hull.AddObserver(observer);

    // initialize hull
    hull.push_back(P[0]); hull.push_back(P[1]); hull.push_back(P[0]);

    for (size_t i = 2; i < P.size(); ++i) {
        if (!RIsLeftOrInsidePQ(*hull.back(1), *hull.back(0), *P[i]) ||
            !RIsLeftOrInsidePQ(*P[i], *hull.front(1), *hull.front(0))) {
            while (!RIsLeftOrInsidePQ(*hull.back(1), *hull.back(0), *P[i])) {
                hull.pop_back();
            }
            while (!RIsLeftOrInsidePQ(*P[i], *hull.front(0), *hull.front(1))) {
                hull.pop_front();
            }
            hull.push_back(P[i]);
            hull.push_front(P[i]);
        }
    }

    return hull;
}