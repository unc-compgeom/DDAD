namespace Intersection {

/*!
 * @brief Represents the intersection of two 2D lines.
 *
 * Maintains raw pointers to the two lines, and is not responsible for deleting
 * these lines. This class does not modify the lines. Users of this class are
 * responsible for ensuring that the pointers remain valid during an instance's
 * lifetime. The intersection, like most points, is canonically allocated on the heap.
 */
class Line_2rLine_2r {
public:
    enum Type {
        INTERSECTION_EMPTY,
        INTERSECTION_POINT,
        INTERSECTION_COINCIDENT
    };

    Line_2rLine_2r();
    Line_2rLine_2r(const Line_2r* line_imp, const Line_2r* line_par);

    void Update();

    const Point_2r& intersection() const;
    const Type type() const;
    const rational& time() const;
    SharedPoint_2r intersection_sptr();

private:
    const Line_2r* line_imp_;
    const Line_2r* line_par_;

    SharedPoint_2r intersection_;
    Type type_;
    rational time_;
};

}