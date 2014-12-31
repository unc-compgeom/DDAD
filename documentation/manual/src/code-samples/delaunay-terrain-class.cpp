class Terrain_3r : public Visual::Geometry {
public:
    Terrain_3r();
    ~Terrain_3r();
    void Initialize(const AABB_2r& region);
    void AddSample(const Point_3r& sample);

private:
    // visualization helper functions
    void SigPushVertex(QuadEdge::Vertex* v);
    void SigPopVertex(QuadEdge::Vertex* v);
    void SigPushEdge(QuadEdge::Edge* e);
    void SigPopEdge(QuadEdge::Edge* e);
    void SigPushFace(QuadEdge::Face* f);
    void SigPopFace(QuadEdge::Face* f);

    QuadEdge::Edge* MakeVertexEdge(QuadEdge::Vertex *v, QuadEdge::Face *left,
                                   QuadEdge::Face *right, SharedPoint_3r vnew_pos);

    QuadEdge::Edge* MakeFaceEdge(QuadEdge::Face *f, QuadEdge::Vertex *org,
                                 QuadEdge::Vertex *dest);

    void KillVertexEdge(QuadEdge::Edge *e);
    void KillFaceEdge(QuadEdge::Edge *e);

    size_t EdgeCount(QuadEdge::Face *f);

    // delaunay triangulation subroutines
    QuadEdge::Edge* LocalizePoint(const Point_3r& sample);
    void TestAndSwapEdges(std::vector<QuadEdge::Edge*>& edges,
                          const Point_3r& sample);


    QuadEdge::Cell* terrain_;
    AABB_2r region_;
    Material mat_vertex_;
    Material mat_edge_;
    Material mat_face_;
};