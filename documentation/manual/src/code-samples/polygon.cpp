class Polygon_2r : public Visual::Geometry {
public:
    /* default constructor/destructor */

    void push_back(SharedPoint_2r v);
    void pop_back();
    SharedPoint_2r back(const size_t i = 0) const;

    /* corresponding "front" methods e.g. pop_front */

    void set_mat_vertex(const Visual::Material mat_vertex);
    void set_mat_edge(const Visual::Material mat_edge);
    void set_mat_face(const Visual::Material mat_face);

private:
    Polyline_2r boundary_;
    Visual::Material mat_face_;
};