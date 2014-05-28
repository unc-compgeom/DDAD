/**
 * Filename: NN_transform.h
 * Project name: degDrivenAlgoDesign
 * Created on: 07 Mar 2012
 * Author: David L. Millman <dave@cs.unc.edu>
 */

#ifndef _DDAD_NN_TRANSFORM_H_
#define _DDAD_NN_TRANSFORM_H_

#include <string>

#include "../base/DDAD_base.h"
#include "../base/Point_2.h"
#include "../base/Predicates_2.h"
#include "../base/Constructions_2.h"

#include "../algos/Discrete_upper_envelope.h"

static RLogChannel *nn_init = DEF_CHANNEL("debug/nn_transform/init", Log_Info);
static RLogChannel *nn_build = DEF_CHANNEL("debug/nn_transform/build", Log_Info);

class NN_transform {

/** type exports (more after polygon) */
public:
    typedef Point_2::Std_point Point;
    typedef Point Site;
    typedef std::vector< Site > Sites;
    typedef Sites::const_iterator Site_iterator;
    typedef Sites::const_reverse_iterator Site_reverse_iterator;

protected:
    // TODO: DLM: this should be changed to a special
    // line of the form \deg(1)x+\deg(2)
    typedef Line_2::Slope_and_y_intercept Line;

    struct Voronoi_cell_1 {
        Site s;
        int begin, end; // [begin, end]
        friend std::ostream& operator<<(std::ostream& out,
                const Voronoi_cell_1& c) {
            return out << c.s << " [" << c.begin << "," << c.end << "]";
        }
    };
    typedef std::vector< Voronoi_cell_1 > Voronoi_1;

    struct Possible_list_cell {
        Site s;
        int bin_num, bin_begin, bin_end;  // [bin_begin, bin_end)
        int end; // end]
        Voronoi_1::const_iterator link;

        /** set this possible cell from the specified cell */
        void set(Voronoi_1::const_iterator cell) {
            s = cell->s; end = cell->end; link = cell;
        }
        bool expired(int y) { return y > end; }

        std::string to_string() const;
        friend std::ostream& operator<<(std::ostream& out,
                const Possible_list_cell& c) { return out << c.to_string(); }
    };
    typedef std::vector< Possible_list_cell > Possible_list;

    struct Converted_lines {
        int Y;
        std::vector< Line > lines;

        // memory to use in for building
        Discrete_upper_envelope::Rand_order_mem rand_order;
        Discrete_upper_envelope::Sorted_lines_mem sorted_lines;
        Discrete_upper_envelope::Building_envelope_mem build_env01;
        Discrete_upper_envelope::Building_envelope_mem build_env02;
        Discrete_upper_envelope::Building_with_orient_mem build_orient;

        Converted_lines(const Possible_list& pset_generator, int one);
        void inc_row(Possible_list& pset_gen);
    };

    template <typename T>
    class Grid {
    protected:
        std::vector< std::vector< T > > p_;
    public:
        void alloc(int one, int U) {
            p_.resize(U+one);
            for (int i = one; i <= U ; ++i) { p_[i].resize(U+one, -1); }
        }
        void set(int x, int y, T i) { p_[y][x] = i; }
        void set(int x_begin, int x_end, int y, T i) {
            std::vector< int >::iterator it;
            std::vector< int >& row = p_[y];
            for (it = row.begin()+x_begin ; it != row.begin()+x_end+1 ; ++it) {
                *it = i;
            }
        }
        T get(int x, int y) const { return p_[y][x]; }
    };


/** data members and local functions */
protected:

    /** the set of input site */
    Sites S_;

    /** The labels for the nearest neighbor transform */
    Grid<int> nn_;
    Grid<float> dist_;

    /** the last point in the grid to process (inclusive) */
    int U_;

    /** the first point in the grid to process (inclusive) */
    const static int one_ = 1;

    /**
     * return true if the query point q is closer to a than b
     * if the distances are the same q is closer to the first point
     * in lexograical ordering.
     *
     * @degree 2
     *
     * @param a the first point
     * @param b the second point
     * @param q the query point
     * @return true if q is closer to a using perturbation
     */
    bool first_closer_perturbed(const Point& a, const Point& b, const Point& q) const;

    /**
     * given two points a and b with the same x coordinate
     * return the boundary of the discrete 1D voronoi cell between a and b.
     * Which is the floor of the y-coordinate of the mid point of a and b.
     *
     * @degree 1
     *
     * @param a the first point
     * @param b the second point
     * @return the boundary of the discrete 1D voronoi cell between a and b.
     */
    int voronoi_cell_boundary_perturbed(const Point& a, const Point& b) const;


    /**
     * given a site s=(i,j) and an value Y. Convert the site to
     * a line of the form:
     * y = 2i x + 2*j-i^2-j^2
     * and store it in line
     *
     * @degree 1x+2
     *
     * @param memory to store line
     * @param site to convert to line
     * @param line in which to do the conversion
     */
    static void site_to_line(Line& line, const Site& s, int Y);

    /**
     * Given a set of sites S, For each column Ci that contains a site,
     * compute the 1d voronoi diagram of the sites along Ci.  Store all the 1D
     * voronoi diagrams into all_voronois such that for each Vor(Ci) the cells
     * are orded in increasing order of cell's beginning and Ci < Cj for i < j.
     * Finally, init pset_generator so that each element of pset_generator
     * is a copy of the first cell of each Vor(Ci).
     *
     * @degree 1
     *
     * @param all_voronois memory location to store the 1D voronois of S.
     * @param pset_generator memory location to store the pset_generators.
     * @param S the sites to compute the 1D voronoi diagrams of.
     */
    void compute_voronois_init_pset_generator(Voronoi_1& all_voronois,
            Possible_list& pset_generator, const Sites& S);

    void compute_voronois_init_pset_generator_assuming_S_sorted(
            Voronoi_1& all_voronois, Possible_list& pset_generator, const Sites& S);

    /** the types of supported due algorithms */
    enum DUE_algo { DUE_ALGO_RIC, DUE_ALGO_ULOGU, DUE_ALGO_DEG_3 };

    /** build the nn transform using the specified due algorithm */
    void build_w_algo(const std::vector< Point >& input, int UB,
            bool sorted, const DUE_algo& due_algo);

    /** Unpack the due into the nn transform along line Y=y */
    virtual void unpack(const Discrete_upper_envelope& due, int y);

    /** allocate memory for the nn and distance transform over region one...U*/
    virtual void allocate(int one, int U);

public:

    NN_transform() { }

    /** Build the NN_transform of the input sites in UBxUB grid */
    void build(const std::vector< Point >& input, int UB);

    /** Build the NN_transform of the input sites in UBxUB grid by brute force */
    void build_brute_force(const std::vector< Point >& input, int UB);

    /** Build the NN_transform of the input sites in UBxUB grid by brute force */
    void build_deg_3_usq(const std::vector< Point >& input, int UB, bool sorted=false);

    /** Build the NN_transform of the input sites in UBxUB grid by brute force */
    void build_deg_2_usq(const std::vector< Point >& input, int UB, bool sorted=false);

    /** Build the NN_transform of the input sites in UBxUB grid by brute force */
    void build_deg_2_usq_logu(const std::vector< Point >& input, int UB, bool sorted=false);

    /** Verify that the computed discrete Voronoi diagram is correct */
    bool verify(const std::vector< Point >& input, int UB) const;

    /** return the one of the problem */
    int one() const { return one_; }

    /** Return the F of the problem */
    int U() const { return U_; }

    /** Return the number of sites for generating the transform */
    int N() const { return S_.size(); }

    /** make a post office query */
    int post_office_query(int x, int y) const {
        assert(one() <= x && x <= U() && one() <= y && y <= U());
        return nn_.get(x,y);
    }

    /** make a post office query */
    float dist_query(int x, int y) const {
        assert(one() <= x && x <= U() && one() <= y && y <= U());
        return dist_.get(x,y);
    }

    /** begin iterator over sites */
    Site_iterator site_begin() const { return S_.begin(); }

    /** end iterator over sites */
    Site_iterator site_end() const { return S_.end(); }

    /** return the strung representation of the discrete voronoi diagram */
    virtual std::string to_string() const;

    /** output operator */
    friend std::ostream& operator<<(std::ostream& out, const NN_transform& r) {
        return out << r.to_string();
    }


    static void sort_sites(Sites& S, int U);

};

#endif

