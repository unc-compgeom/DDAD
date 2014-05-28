/**
 * Filename: NN_transform.cpp
 * Project name: degDrivenAlgoDesign
 * Created on: 07 Mar 2012
 * Author: David L. Millman <dave@cs.unc.edu>
 */

#include "NN_transform.h"

/********************************************************
 *                 Minor classes
 ********************************************************/
std::string NN_transform::Possible_list_cell::to_string() const {
    std::ostringstream os;
    os << s << "--" << end <<" -- "
        << bin_num << " [" << bin_begin << "," << bin_end << ")";
    return os.str();
}


NN_transform::Converted_lines::Converted_lines(
        const Possible_list& pset_generator, int one) {
    Y = one;
    int num_lines = pset_generator.size();
    lines.resize(pset_generator.size());
    rand_order.resize(Discrete_upper_envelope::rand_order_mem_size(num_lines));
    sorted_lines.resize(Discrete_upper_envelope::sorted_lines_mem_size(num_lines));
    build_env01.resize(Discrete_upper_envelope::building_envelope_mem_size(num_lines));
    build_env02.resize(Discrete_upper_envelope::building_envelope_mem_size(num_lines));
    build_orient.resize(Discrete_upper_envelope::building_with_orient_mem_size(num_lines));

    for (size_t i = 0 ; i < pset_generator.size() ; ++i) {
        NN_transform::site_to_line(lines[i], pset_generator[i].s, Y);
    }
}


void NN_transform::Converted_lines::inc_row(Possible_list& pset_gen) {
    ++Y;
    for (size_t i = 0 ; i < pset_gen.size() ; ++i) {
        if (pset_gen[i].expired(Y)) { pset_gen[i].set( (pset_gen[i].link)+1); }
        NN_transform::site_to_line(lines[i], pset_gen[i].s, Y);
    }
}

/********************************************************
 *                 Predicates & Constructions
 ********************************************************/
bool NN_transform::first_closer_perturbed(const Point& a, const Point& b,
        const Point& q) const {
    switch (Point_2::closer(a, b, q)) {
        case Point_2::CLOSER_TO_FIRST: return true;
        case Point_2::CLOSER_TO_SECOND: return false;
        case Point_2::CLOSER_EQUIDISTANT: return Point_2::precedes(a,b);
        default: rTerminate("Case not covered", DDAD::Case_not_covered_error());
    }
}


int NN_transform::voronoi_cell_boundary_perturbed(const Point& a,
        const Point& b) const {
    rAssert(Point_2::on_vertical_line(a,b));
    return std::floor( Constructions_2::coord_y_mid_point(a,b) );
}


void NN_transform::site_to_line(Line& line, const Site& s, int Y) {
    int i = Constructions_2::coord_x(s);
    int j = Constructions_2::coord_y(s);
    line.reset(2.*i, 2*j*Y - i*i - j*j, s.id());
}


/********************************************************
 *                 Nearest Neighbor Transform
 ********************************************************/
void NN_transform::build(const std::vector< Point >& input, int UB) {
    build_deg_2_usq(input, UB, false);
}



void NN_transform::build_brute_force(
        const std::vector< Point >& input, int UB) {

    rAssert(UB > 1 && input.size() > 0);

    // init
    U_ = UB;
    S_ = input;

    // allocate memory for transform
    nn_.alloc(one(), U());
    dist_.alloc(one(), U());

    // iterate over each pixel
    for (int x = one() ; x <= U() ; ++x) {
        for (int y = one() ; y <= U() ; y++) {
            Point p(x,y);
            Site_iterator s_min = input.begin();
            for (Site_iterator si = input.begin()+1 ; si != input.end() ; ++si) {
                if (first_closer_perturbed(*si, *s_min, p)) { s_min = si; }
            }
            nn_.set(x,y,s_min->id());
        }
    }
}


void NN_transform::allocate(int one, int U) {
    nn_.alloc(one, U);
    dist_.alloc(one, U);
}


void NN_transform::build_w_algo(const std::vector< Point >& input, int UB, bool sorted,
        const DUE_algo& due_algo) {

    rAssert(UB > 1 && input.size() > 0);

    // init
    U_ = UB;
    S_ = input;

    // allocate memory for transform and due
    allocate(one(), U());

    // log the input points
    rLog(nn_build, "Input points:\n%s", vect_to_cstring(input.begin(), input.end()));

    // compute the 1d voronoi diagram and preprocessing structure for generating
    // possible lists.
    Voronoi_1 all_voronois;
    Possible_list pset_generator;
    if (sorted) {
        compute_voronois_init_pset_generator_assuming_S_sorted(all_voronois,
                pset_generator, input);
    } else {
        compute_voronois_init_pset_generator(all_voronois, pset_generator, input);
    }

    // now for each possible set we do a horrizontal sweep computing the nearest
    // neighbor for each grid point of the horizontal line
    Discrete_upper_envelope due;
    Converted_lines converted_lines(pset_generator, one()-1);
    for (int i = one() ; i <= U() ; ++i) { // i is for debugging
        /** increment line */
        converted_lines.inc_row(pset_generator);

        // log and verify
        rAssert(i == converted_lines.Y);
        rLog(nn_build, "Converted_lines for %d\n%s", i, vect_to_cstring(
                    converted_lines.lines.begin(), converted_lines.lines.end()));

        /** compute the next due */
        due.clear();
        switch(due_algo) {
        case DUE_ALGO_RIC:
            due.build_ric_with_slope_unique_lines_sorted_by_slope(converted_lines.lines, U(),
                    converted_lines.rand_order,
                    converted_lines.sorted_lines,
                    converted_lines.build_env01,
                    converted_lines.build_env02);
            break;
        case DUE_ALGO_ULOGU:
            due.build_ulogu_with_slope_unique_lines_sorted_by_slope(converted_lines.lines, U(),
                    converted_lines.sorted_lines,
                    converted_lines.build_env01);
            break;

        case DUE_ALGO_DEG_3:
            due.build_orient_with_slope_unique_lines_sorted_by_slope(converted_lines.lines, U(),
                    converted_lines.sorted_lines,
                    converted_lines.build_orient);
            break;
        }

        rLog(nn_build, "DUE for %d\n%s", i, vect_to_cstring(due.begin(), due.end()));

        /** unpack DUE */
        unpack(due, i);
    }

    // fill in dist
    for (int i = one() ; i < U() ; ++i) {
        for (int j = one() ; j < U() ; ++j) {
            const Site& si = input[nn_.get(i,j)];
            dist_.set(i,j, Constructions_2::dist(i,j,
                        Constructions_2::coord_x(si),
                        Constructions_2::coord_y(si)));
        }
    }
}


void NN_transform::unpack(const Discrete_upper_envelope& due, int y) {
    typedef Discrete_upper_envelope::const_iterator DUE_iter;
    for (DUE_iter cell = due.begin() ; cell != due.end() ; ++cell) {
        nn_.set(cell->left, cell->right, y, cell->line.id());
    }
}



void NN_transform::build_deg_3_usq(const std::vector< Point >& input, int UB, bool sorted) {
    build_w_algo(input, UB, sorted, DUE_ALGO_DEG_3);
}


void NN_transform::build_deg_2_usq(const std::vector< Point >& input, int UB, bool sorted) {
    build_w_algo(input, UB, sorted, DUE_ALGO_RIC);
}


void NN_transform::build_deg_2_usq_logu(const std::vector< Point >& input, int UB, bool sorted) {
    build_w_algo(input, UB, sorted, DUE_ALGO_ULOGU);
}


void NN_transform::compute_voronois_init_pset_generator(Voronoi_1& all_voronois,
            Possible_list& pset_generator, const Sites& S) {

    // First we sort the sites by x braking ties by y by
    // running a radix sort with radix U.
    // Another way to think of this is first we use a stable counting sort to
    // sort by y and then again use a stable counting sort to sort by x


    //
    // first we do a stable counting sort by y
    //

    // count how many sites have y coordinate Y
    // that is, how many sites are in each bin
    std::vector< int > C(U()+1,0);
    for (Site_iterator si = S.begin() ; si != S.end() ; ++si) {
        ++C[Constructions_2::coord_y(*si)];
    }
    rLog(nn_init, "Bin Counts: %s", vect_to_string(C.begin(), C.end(), " ", " ").c_str());

    // prefix sum: for each coordinate y count the number of sites with
    // coordinate less than or equal to y.
    for (int i = 1 ; i <= U() ; ++i) { C[i] += C[i-1]; }

    // put the sites into their y bins.
    std::vector< Site_reverse_iterator > bins(S.size());
    for (Site_reverse_iterator si = S.rbegin() ; si != S.rend() ; ++si) {
        bins[ C[Constructions_2::coord_y(*si)]-1 ] = si;
        --C[Constructions_2::coord_y(*si)];
    }
    rLog(nn_init, "Bins: \n %s",
            vect_of_ptrs_to_cstring(bins.begin(), bins.end()));


    //
    // stable counting sort by x
    //

    // zero the counts and then
    // count how many sites have y coordinate Y
    // that is, how many sites are in each bin
    typedef std::vector< Site_reverse_iterator >::reverse_iterator RI;
    for (size_t i = 0 ; i < C.size() ; ++i) { C[i] = 0; }
    for (RI si = bins.rbegin() ; si != bins.rend() ; ++si) {
        ++C[Constructions_2::coord_x(**si)];
    }
    rLog(nn_init, "Bin Counts: %s", vect_to_string(C.begin(), C.end(), " ", " ").c_str());

    // prefix sum: for each coordinate x count the number of sites with
    // coordinate less than or equal to x.
    // Also, in this pass compute the number of columns with at leaset one site.
    int non_empty_cols = 0;
    for (int i = 1 ; i <= U() ; ++i) {
        non_empty_cols += C[i] != 0;
        C[i] += C[i-1];
    }
    rLog(nn_init, "Num non empty cols: %d", non_empty_cols);

    // next we set up the bins of the pset_generator.
    int j = 0;
    pset_generator.resize(non_empty_cols);
    for (int i = 1 ; i <= U() ; ++i) {
        // when C[i] == C[i-1] column i contains zero sites
        if (C[i] == C[i-1]) { continue; }

        pset_generator[j].bin_num = i;
        pset_generator[j].bin_begin = C[i-1];
        pset_generator[j].bin_end = C[i];
        ++j;
    }
    rLog(nn_init, "Pset_gen: \n %s",
            vect_to_cstring(pset_generator.begin(), pset_generator.end()));

    // allocate space for all 1d voronoi diagrams
    // and put the sites into their x bins (which will give us the 1d voronoi
    // diagrams.
    all_voronois.resize(S.size());
    for (RI si = bins.rbegin() ; si != bins.rend() ; ++si) {
        all_voronois[ C[Constructions_2::coord_x(**si)]-1 ].s = **si;
        --C[Constructions_2::coord_x(**si)];
    }
    rLog(nn_init, "All voronois: \n %s",
            vect_to_cstring(all_voronois.begin(), all_voronois.end()));

    // iterate over 1d voronoi assigning start and end to each cell
    typedef Voronoi_1::iterator Vor_iter;
    all_voronois.front().begin = one();
    for (Vor_iter ci = all_voronois.begin()+1 ; ci != all_voronois.end() ; ++ci) {
        if (Point_2::on_vertical_line((ci-1)->s, (ci)->s)) {
            int cell_end = voronoi_cell_boundary_perturbed((ci-1)->s, ci->s);
            (ci-1)->end = cell_end;
            ci->begin = cell_end+1;
        } else {
            (ci-1)->end = U();
            ci->begin = one();
        }
    }
    all_voronois.back().end = U();

    // iterate over the pset_generators, initiating sites and pointers
    typedef Possible_list::iterator PL_iter;
    for (PL_iter pc = pset_generator.begin() ; pc != pset_generator.end() ; ++pc) {
        Vor_iter first_cell = all_voronois.begin()+pc->bin_begin;
        pc->bin_num = Constructions_2::coord_x(first_cell->s);
        pc->set(first_cell);
    }
    rLog(nn_init, "All voronois: \n %s",
            vect_to_cstring(all_voronois.begin(), all_voronois.end()));
    rLog(nn_init, "Pset_gen: \n %s",
            vect_to_cstring(pset_generator.begin(), pset_generator.end()));
}

void NN_transform::sort_sites(Sites& S, int U) {

    //
    // first we do a stable counting sort by y
    //
    Sites tmp = S;

    // count how many sites have y coordinate Y
    // that is, how many sites are in each bin
    std::vector< int > C(U+1,0);
    for (Site_iterator si = tmp.begin() ; si != tmp.end() ; ++si) {
        ++C[Constructions_2::coord_y(*si)];
    }
    rLog(nn_init, "Bin Counts: %s", vect_to_string(C.begin(), C.end(), " ", " ").c_str());

    // prefix sum: for each coordinate y count the number of sites with
    // coordinate less than or equal to y.
    for (int i = 1 ; i <= U ; ++i) { C[i] += C[i-1]; }

    // put the sites into their y bins.
    std::vector< Site_reverse_iterator > bins(tmp.size());
    for (Site_reverse_iterator si = tmp.rbegin() ; si != tmp.rend() ; ++si) {
        bins[ C[Constructions_2::coord_y(*si)]-1 ] = si;
        --C[Constructions_2::coord_y(*si)];
    }
    rLog(nn_init, "Bins: \n %s",
            vect_of_ptrs_to_cstring(bins.begin(), bins.end()));


    //
    // stable counting sort by x
    //

    // zero the counts and then
    // count how many sites have y coordinate Y
    // that is, how many sites are in each bin
    typedef std::vector< Site_reverse_iterator >::reverse_iterator RI;
    for (size_t i = 0 ; i < C.size() ; ++i) { C[i] = 0; }
    for (RI si = bins.rbegin() ; si != bins.rend() ; ++si) {
        ++C[Constructions_2::coord_x(**si)];
    }
    rLog(nn_init, "Bin Counts: %s", vect_to_string(C.begin(), C.end(), " ", " ").c_str());

    // prefix sum: for each coordinate x count the number of sites with
    // coordinate less than or equal to x.
    // Also, in this pass compute the number of columns with at leaset one site.
    for (int i = 1 ; i <= U ; ++i) { C[i] += C[i-1]; }

    // allocate space for all 1d voronoi diagrams
    // and put the sites into their x bins (which will give us the 1d voronoi
    // diagrams.
    for (RI si = bins.rbegin() ; si != bins.rend() ; ++si) {
        S[ C[Constructions_2::coord_x(**si)]-1 ] = **si;
        --C[Constructions_2::coord_x(**si)];
    }
    rLog(nn_init, "S sorted: \n %s",
            vect_to_cstring(S.begin(), S.end()));
}


void NN_transform::compute_voronois_init_pset_generator_assuming_S_sorted(
        Voronoi_1& all_voronois, Possible_list& pset_generator, const Sites& S) {
    // first copy S to all_voronois
    all_voronois.resize(S.size());
    for (size_t i = 0 ; i < S.size() ; ++i) { all_voronois[i].s = S[i]; }

    // next we set up the pset_generator.
    pset_generator.resize(U());
    int j = 0;
    pset_generator[0].bin_num = Constructions_2::coord_x(S[j]);
    pset_generator[0].bin_begin = 0;
    for (size_t i = 1 ; i < S.size() ; ++i) {
        if (pset_generator[j].bin_num != Constructions_2::coord_x(S[i])) {
            pset_generator[j].bin_end = i;
            ++j;
            pset_generator[j].bin_num = Constructions_2::coord_x(S[i]);
            pset_generator[j].bin_begin = i;
        }
    }
    pset_generator[j].bin_end = S.size();
    pset_generator.resize(j+1);
    rLog(nn_init, "Pset_gen: \n %s",
            vect_to_cstring(pset_generator.begin(), pset_generator.end()));
    rLog(nn_init, "All voronois: \n %s",
            vect_to_cstring(all_voronois.begin(), all_voronois.end()));

    // iterate over 1d voronoi assigning start and end to each cell
    typedef Voronoi_1::iterator Vor_iter;
    all_voronois.front().begin = one();
    for (Vor_iter ci = all_voronois.begin()+1 ; ci != all_voronois.end() ; ++ci) {
        if (Point_2::on_vertical_line((ci-1)->s, (ci)->s)) {
            int cell_end = voronoi_cell_boundary_perturbed((ci-1)->s, ci->s);
            (ci-1)->end = cell_end;
            ci->begin = cell_end+1;
        } else {
            (ci-1)->end = U();
            ci->begin = one();
        }
    }
    all_voronois.back().end = U();

    // iterate over the pset_generators, initiating sites and pointers
    typedef Possible_list::iterator PL_iter;
    for (PL_iter pc = pset_generator.begin() ; pc != pset_generator.end() ; ++pc) {
        Vor_iter first_cell = all_voronois.begin()+pc->bin_begin;
        pc->bin_num = Constructions_2::coord_x(first_cell->s);
        pc->set(first_cell);
    }
    rLog(nn_init, "All voronois: \n %s",
            vect_to_cstring(all_voronois.begin(), all_voronois.end()));
    rLog(nn_init, "Pset_gen: \n %s",
            vect_to_cstring(pset_generator.begin(), pset_generator.end()));
}


bool NN_transform::verify(const std::vector< Point >& input, int UB) const {
    NN_transform verify;
    verify.build_brute_force(input, UB);

    for (int y = one() ; y <= U() ; ++y) {
        for (int x = one() ; x <= U() ; ++x) {
            if (post_office_query(x,y) != verify.post_office_query(x,y)) {
                std::cout << "(" << x << "," << y << ") Expected: "
                    << verify.post_office_query(x,y) <<  " Actual: " << post_office_query(x,y);
                return false;
            }
        }
    }
    return true;
}


std::string NN_transform::to_string() const {
    std::ostringstream os;
    for (int x = U() ; x >= one() ; --x) {
        for (int y = one() ; y <= U() ; ++y) {
            os << post_office_query(x,y) << " ";
        }
        os << std::endl;
    }
    return os.str();
}


