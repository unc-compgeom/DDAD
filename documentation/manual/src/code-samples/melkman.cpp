Polygon_2rDq Melkman(const PolyChain_2r& P, Visual::IGeometryObserver* observer){
    Polygon_2rDq D;
    D.AddObserver(observer);
    std::list<PolyChainVertex_2r> vertices = P.vertices();
    std::list<PolyChainVertex_2r>::iterator it = vertices.begin();
    PolyChainVertex_2r v0 = *it; it++;
    PolyChainVertex_2r v1 = *it; it++;
    D.PushFront(v1);
    D.PushFront(v0);
    D.PushFront(v1);

    for(std::list<PolyChainVertex_2r>::iterator ii = it; ii != vertices.end(); ii ++){
        PolyChainVertex_2r pi = *ii;
        Line_2r topEdge = Line_2r(D[0].vertex_sptr(), D[1].vertex_sptr());
        Ray_2r topRay = Ray_2r(D[1].vertex_sptr(), topEdge.V());
        Line_2r botEdge = Line_2r(D[D.NumVertices()-2].vertex_sptr(), D[D.NumVertices()-1].vertex_sptr());
        Ray_2r botRay = Ray_2r(D[D.NumVertices()-1].vertex_sptr(), botEdge.V());

        if((Predicate::AIsRightOfB(pi.vertex(), topEdge) || Predicate::AIsAheadOfB(pi.vertex(), topRay)) ||
        (Predicate::AIsRightOfB(pi.vertex(), botEdge) || Predicate::AIsAheadOfB(pi.vertex(), botRay))){
            while(Predicate::AIsRightOfB(pi.vertex(), topEdge) || Predicate::AIsAheadOfB(pi.vertex(), topRay)){
                // Pop top
                D.PopFront();
                topEdge = Line_2r(D[0].vertex_sptr(), D[1].vertex_sptr());
                topRay = Ray_2r(D[1].vertex_sptr(), topEdge.V());
            }
            while(Predicate::AIsRightOfB(pi.vertex(), botEdge) || Predicate::AIsAheadOfB(pi.vertex(), botRay)){
                // Pop bot
                D.PopBack();
                botEdge = Line_2r(D[D.NumVertices()-2].vertex_sptr(), D[D.NumVertices()-1].vertex_sptr());
                botRay = Ray_2r(D[D.NumVertices()-1].vertex_sptr(), botEdge.V());
            }
            D.PushFront(pi);
            D.PushBack(pi);
        }
    }
    return D;
}