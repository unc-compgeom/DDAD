/**
 * Filename: Discrete_upper_envelope.cpp
 * Project name: degDrivenAlgoDesig
 * Created on: 21 Feb 2011
 * Author: David L. Millman <dave@cs.unc.edu>
 */

#include "Discrete_upper_envelope.h"

/********************************************************
 *                 Envelope Cell
 ********************************************************/
void Discrete_upper_envelope::Envelope_cell::set_lr(int l, int r) {
    rAssert(l <= r);
    left=l;
    right=r;
}


std::string Discrete_upper_envelope::Envelope_cell::to_string() const {
    std::ostringstream os;
    os << line << ": (" << left << ", " << right << ")";
    return os.str();
}

/********************************************************
 *                 Building envelope Cell
 ********************************************************/
std::string Discrete_upper_envelope::Building_envelope_cell::to_string(
        bool follow_intersections) const {
    std::ostringstream os;
    os << "Begin: " << left << "\tline: " << *line;
    if (follow_intersections) {
        os << "\n\t\tIntersecting Lines";
        Building_envelope_cell* p = ptr;
        while (p != NULL) {
            os << "\n\t\t " << p->to_string(false);
            p = p->ptr;
        }
    }
    return os.str();
}

/********************************************************
 *                 Three sorted lists
 ********************************************************/
Discrete_upper_envelope::Three_sorted_lists_of_lines::Three_sorted_lists_of_lines(
        Building_envelope_cell& ci, Building_envelope_cell* left_list,
        Building_envelope_cell* left_list_tail)
            : inf(INT_MAX, INT_MAX), all_sorted_head(&inf, INT_MAX) {
    cell = &ci;
    left = left_list;
    left_tail = left_list_tail;
    bottom = ci.ptr;
    all_sorted_head.ptr = NULL;
    all_sorted_tail = &all_sorted_head;
}


void Discrete_upper_envelope::Three_sorted_lists_of_lines::filter_below() {
    // Let's call the the least sloped line in the left list be l0.
    // We will filter using the observation that any line intersecting the
    // bottom of the cell, with slope less than l0 cannot be part of the DUE.
    rAssert(left != NULL);

    // get l0
    const Line* l0 = left_tail->line;
    rLog(up_env_ric, "Filter bottom using line %s", thing_to_cstring(*l0));
    rLog(up_env_ric, "Three_sorted_lists_of_lines:\n%s", thing_to_cstring(*this));

    // filter bottom using l0
    while (bottom != NULL
            && Discrete_upper_envelope::line_slope_lt(*(bottom->line), *l0)) {
        rLog(up_env_ric, "Filtering: \n%s", thing_to_cstring(*(bottom->line)));
        Building_envelope_cell* next_bottom = bottom->ptr;
        bottom->ptr = NULL;
        bottom = next_bottom;
    }

    // filter cell using l0
    l0 = left->line;
    rLog(up_env_ric, "Filter cell using line %s", thing_to_cstring(*l0));
    if ( Discrete_upper_envelope::line_slope_lt(*(cell->line), *l0) ) {
        cell = NULL;
    }

    // log the new lines
    rLog(up_env_ric, "Filtered Three_sorted_lists_of_lines:\n%s",
            thing_to_cstring(*this));
}


void Discrete_upper_envelope::Three_sorted_lists_of_lines::filter_all_sorted(const Line& filter) {

    Building_envelope_cell* cur = all_sorted_head.ptr;
    while (Discrete_upper_envelope::line_slope_lt(*(cur->line), filter)) {
        // incrament
        Building_envelope_cell* tmp = cur;
        cur = cur->ptr;
        tmp->ptr = NULL;
    }
    all_sorted_head.ptr = cur;
    rAssert(*(all_sorted_head.ptr->line) == filter);
}

void Discrete_upper_envelope::Three_sorted_lists_of_lines::init_iteration() {

    rAssert(left != NULL);
    if (bottom == NULL) { bottom = &all_sorted_head; }
    if (cell == NULL) { cell = &all_sorted_head; }
    all_sorted_head.ptr = NULL;
    all_sorted_tail = &all_sorted_head;

    // get rid of warning
#ifndef NDEBUG
    const Line *l = left->line;
    const Line *best = least_slope_and_inc();
    rAssert(l == best);
#else
    least_slope_and_inc();
#endif
}


const Discrete_upper_envelope::Line*
Discrete_upper_envelope::Three_sorted_lists_of_lines::least_slope_and_inc() {
    rAssert(!empty());
    const Line *l = left->line;
    const Line *b = bottom->line;
    const Line *c = cell->line;
    Building_envelope_cell *best = NULL;
    // l least slopped
    if (Discrete_upper_envelope::line_slope_lt(*l, *b)
            && Discrete_upper_envelope::line_slope_lt(*l, *c)) {
        best = left;
        left = (left->ptr != NULL) ? left->ptr : &all_sorted_head;
    // b least slopped
    } else if (Discrete_upper_envelope::line_slope_lt(*b, *l)
            && Discrete_upper_envelope::line_slope_lt(*b, *c)) {
        best = bottom;
        bottom = (bottom->ptr != NULL) ? bottom->ptr : &all_sorted_head;
    // c least slopped
    } else {
        rAssert(Discrete_upper_envelope::line_slope_lt(*c, *l)
                && Discrete_upper_envelope::line_slope_lt(*c, *b));
        best = cell;
        cell = &all_sorted_head;
    }
    rAssert(best != NULL);
    best->ptr = all_sorted_tail->ptr;
    all_sorted_tail->ptr = best;
    all_sorted_tail = best;
    return best->line;
}


bool Discrete_upper_envelope::Three_sorted_lists_of_lines::empty() const {
    return left == &(all_sorted_head)
        && bottom == &(all_sorted_head)
        && cell == &(all_sorted_head);
}


std::string Discrete_upper_envelope::Three_sorted_lists_of_lines::to_string() const {
    std::ostringstream os;
    os << "**Left**: ";
    if (left == NULL || left == &all_sorted_head) { os << "EMPTY"; }
    else { os << "\n" << left->to_string(true); }
    os << std::endl;
    os << "**Bottom**: ";
    if (bottom == NULL || bottom == &all_sorted_head) { os << "EMPTY"; }
    else { os << "\n" << bottom->to_string(true); }
    os << std::endl;
    os << "**Cell**: ";
    if (cell == NULL || cell == &all_sorted_head) { os << "EMPTY"; }
    else { os << cell->to_string(false); }
    return os.str();
}


/********************************************************
 *                 One sorted lists
 ********************************************************/
const Discrete_upper_envelope::Line*
Discrete_upper_envelope::One_sorted_list_of_lines::least_slope_and_inc() {
    const Line* best = *(cur);
    ++(cur);
    return best;
}


/********************************************************
 *                 Longer Operations
 ********************************************************/
int Discrete_upper_envelope::bin_search_intersection(
        const Line& ll, const Line& hh, int low, int high){

    // get the easy degerate case out of the way
    if (line_intersect(ll,hh,low)) { return low; }
    if (line_intersect(ll,hh,high)) { return high; }

    // label lines such that l is the lower line on the left
    const Line* l;
    const Line* h;
    if (line_below(ll,hh,low)) {
        l = &ll;
        h = &hh;
    } else {
        l = &hh;
        h = &ll;
    }

    // check if the lines intersect in the interval
    if ( line_below(*l, *h, high) ) { return -1; }

    // if we got here we know that the lines intersect in (low, high)
    // loop with invar the intersection is in [lb, hb)
    int ub = high, lb = low, m;
    while (ub-lb > 1) {
        rAssert( !line_above(*l,*h,lb) && line_above(*l, *h, ub));

        // set new m
        m = lb + (ub-lb)/2;

        // check if the intervals swapped
        if ( !line_above(*l, *h, m)) { lb = m; }
        else { ub = m; }
    }
    return lb;
}


/********************************************************
 *                 Envelope Calc Helper Functions
 ********************************************************/
template< typename Envelope, typename SortedLines >
void Discrete_upper_envelope::due_w_bin_search_of_sorted_and_filtered_lines(
        Envelope& envelope, SortedLines& L, int one, int U) {


    // typedefs and preconditions
    typedef typename Envelope::value_type Cell;

    L.init_iteration();
    while (! L.empty() ) {

        const Line* cur_line = L.least_slope_and_inc();
        Line_2::Cell_inter_type inter_type;

        // log the line and the current state of the envelope
        rLog(upper_envelope, "Adding %s\n", thing_to_cstring(*cur_line));
        rLog(upper_envelope, "Current envelope:\n %s",
                vect_to_cstring(envelope.begin(), envelope.end()));

        // remove all cells that are killed by the current line
        int back_right = U;
        while (envelope.size() != 1 && (inter_type =
                    Line_2::classify_intersection(*cur_line, envelope.back().line_op(),
                        envelope.back().left, back_right)) == Line_2::CELL_INTER_INTERIOR) {
        //while (envelope.size() != 1 && Line_2::interior_intersection(
        //            *cur_line, envelope.back().line_op(), envelope.back().left, back_right)) {
            back_right = envelope.back().left-1;
            envelope.pop_back();
        }

        // create new cell
        if (envelope.size() == 1) {
            envelope.push_back(Cell(cur_line, one));
        } else {
            int back_end = -1;
            const Cell& back = envelope.back();
            //Line_2::Cell_inter_type inter_type = Line_2::classify_intersection(
            //        *cur_line, back.line_op(), back.left, back_right);

            switch (inter_type) {

                // last cell is not changed but
                case Line_2::CELL_INTER_EMPTY:
                case Line_2::CELL_INTER_RIGHT_AND_LEFT_BDD:
                case Line_2::CELL_INTER_RIGHT_BDD: back_end = back_right; break;

                // the line intersects the last cell.
                // Clip the old last cell and create a new cell for the current line

                // since I already know where the intersection occurs, I don't need
                // to do the binary search
                case Line_2::CELL_INTER_LEFT_BDD: back_end = back.left; break;

                // I don't know where in the last cell the intersection occurs so I need
                // to do a binary search to find it.
                case Line_2::CELL_INTER_LOWER_BDD:
                    back_end = bin_search_intersection(*cur_line,
                            back.line_op(), back.left, back_right);
                    break;

                // If we got here then the current line does not intersect the cell
                // in a lower boundary.  If this was true we should not have left
                // the while loop.
                case Line_2::CELL_INTER_INTERIOR:
                    rTerminate("Unexpected State", DDAD::Unexpected_state_error());
            };

            // set the new end for the back cell and if necessary create a new cell
            // for the current line
            if (Predicates_2::in_closed_interval(one, U, back_end+1)) {
                envelope.push_back(Cell(cur_line, back_end+1));
            }
        }
        // log the envelope
        rLog(upper_envelope, "Adding line complete:\n %s",
                vect_to_cstring(envelope.begin(), envelope.end()));
    }
}


template< class OutputIter, class InputIter >
void Discrete_upper_envelope::sort_and_filter_by_slope(
        OutputIter out, InputIter begin, InputIter end) const {

    typedef std::pair<int, const Line* > Line_pair;
    typedef std::vector< std::pair<int, const Line* > > Line_pair_vec;

    // sort the input by slope
    // we do this by creating a vector where the key is the slope
    // and the value is a pointer to the line.
    Line_pair_vec sort_lines;

    // TODO: @todo once we have counting sort fix slope scaling
    // find min slope to scale keys
    int min_slope = INT_MAX;
    for (InputIter li = begin; li != end ; ++li) {
        min_slope = std::min(min_slope, Line_2::slope(*li));
    }
    int tranlate_slope = (min_slope>=0)?0:-min_slope;
    for (InputIter li = begin; li != end ; ++li) {
        sort_lines.push_back(Line_pair(Line_2::slope(*li)+tranlate_slope, &*li));
    }
    Radix_sort::radix_sort_pair(sort_lines.begin(), sort_lines.end());

    // log the sorted list
    rLog(upper_envelope, "Lines sorted:\n %s",
            pair_vect_x_ptr_to_cstring(sort_lines.begin(), sort_lines.end()));

    // iterate over all the sorted lines
    // if any two lines have the same slope,
    // select only the line with the larger y-intercept value
    // if the line was selected add it to output set of lines
    const Line* best_line = sort_lines[0].second;
    for (typename Line_pair_vec::iterator iter = sort_lines.begin()+1 ;
            iter != sort_lines.end() ; ++iter) {
        const Line* li = iter->second;
        // check if the next line to add has the same
        // slope as the ith line
        // if it does not add it the the return list
        if (line_slope_neq(*best_line, *li)) {
            *out++ = best_line;
            best_line = li;
        } else {
            // lines have sampe slopes,
            // pick the line with the higher y-intercept
            assert (*best_line != *li);  // verify lines are unique
            if (line_y_int_lt(*best_line, *li)) { best_line = li; }
        }
    }
    // add the last best line
    *out++ = best_line;
}

template < typename Lines >
void Discrete_upper_envelope::init_build_and_verify_input(
        const Lines& input, int LB, int UB) {

    assert(input.size() > 0 && LB < UB);

    // set the size
    one_ = LB;
    U_ = UB;
}

/********************************************************
 *   Internal Envelope Building Ops (the heave lifting)
 ********************************************************/
void Discrete_upper_envelope::build_ric_with_slope_unique_lines_sorted_by_slope(
        std::list< const Line* >& sorted_lines,
        Rand_order_mem& rand_order,
        Building_envelope_mem& building_envelope01,
        Building_envelope_mem& building_envelope02) {

    // typedefs
    typedef Rand_order_mem::iterator Rand_order_iter;
    typedef Building_envelope::iterator Building_env_iter;

    // checks
    rAssert(rand_order.size() == sorted_lines.size());

    // compute the random insertion order
    int bin_begin = 1 << DDAD_util::log2(sorted_lines.size());
    int bin_size = sorted_lines.size()-bin_begin;
    while(bin_begin >= 1) {
        Shuffle::standard_sequential_sample_with_removal(
                rand_order.begin()+bin_begin, sorted_lines, bin_size);
        bin_size = bin_begin = bin_begin >> 1;
    }
    rAssert(sorted_lines.size() == 1);
    Shuffle::standard_sequential_sample_with_removal(
            rand_order.begin(), sorted_lines, 1);
    rLog(up_env_ric, "Random insertion order:\n%s",
            vect_to_cstring(rand_order.begin(), rand_order.end()));

    // init the envelopes for building between
    // the largest a building envelope could be is all
    // lines plus the end sentinal line.
    Line neg_inf(INT_MIN, INT_MIN);
    Line pos_inf(INT_MAX, INT_MAX);

    building_envelope01.resize(0);
    building_envelope02.resize(0);
    Building_envelope *build_env = &building_envelope01;
    Building_envelope *next_env = &building_envelope02;
    build_env->push_back(Building_envelope_cell(&neg_inf, one()-1));
    build_env->push_back(Building_envelope_cell(rand_order.begin()->line, one()));
    build_env->push_back(Building_envelope_cell(&pos_inf, U()+1));


    // init vars for adding
    int delta = 1, num_lines = (int)rand_order.size();
    int end_offset = 0, begin_offset = std::min(end_offset+delta, num_lines-1);
    Building_env_iter end, begin;

    // add the lines in increasing sizes of powers of 2
    while (end_offset < num_lines-1) {

        begin = rand_order.begin() + begin_offset;
        end = rand_order.begin() + end_offset;

        // log the current envelope and the lines processed in this round
        rLog(up_env_ric, "Building envelope:\n%s",
                vect_to_cstring(build_env->begin(), build_env->end()));
        rLog(up_env_ric, "Process:\n%s", vect_to_cstring(end+1, begin+1));

        // for each line l identify the left most cell intersected by l
        // (if such a cell exists)
        // the order of the slopes of the upper envelope
        // must be ordered by slope so we can treat this as a
        // merge operation
        Building_env_iter c_j = build_env->begin()+build_env->size()-1;
        for (Rand_order_iter l = begin ; l != end ; --l) {

            // log the line that we are processing this iteration
            rLog(up_env_ric, "Finding intersections for %s",
                    thing_to_cstring(*(l->line)));

            // advance along the upper envelope until
            // the slope of l is between the slope of two lines of the
            // upper envelope
            while ( c_j != build_env->begin()
                    && line_slope_lt(*(l->line), *(c_j->line)) ) { --c_j; }

            // this assertion verifys that we found the correct possible
            // insertion point for l
            rAssert( c_j != build_env->end() && (c_j+1) != build_env->end() );
            rAssert( line_slope_lt(*(c_j)->line,*(l->line)));
            rAssert( line_slope_lt(*(l->line), *(c_j+1)->line) );
            rLog(up_env_ric, "First cell with smaller slope\n%s",
                    thing_to_cstring(*c_j));

            // next we walk backwards thought the cells,
            // to find the left most cell intersected by l->line
            // left_most will store the current left most
            // cell intersected by l->line.  If
            // at the end left_most is still build_env->end()
            // then l->line did not intersect any cells
            Building_env_iter left_most = (c_j+2 != build_env->end()
                    && line_above_or_on(*(l->line),*(c_j+1)->line, (c_j+1)->left)) ?
                (c_j+1) : build_env->end();

            for (Building_env_iter inter = c_j; inter != build_env->begin()
                    && line_above(*(l->line), *(inter->line), (inter+1)->left) ;
                    --inter) {
                left_most = inter;
                rLog(up_env_ric, "Intersection with \n %s", thing_to_cstring(*inter));
            }
            rAssert(left_most != build_env->begin());

            // if the cell intersected any cells
            // set the intersection flag for the left most intersection cell
            if (left_most != build_env->end()) {
                l->ptr = left_most->ptr;
                left_most->ptr = &(*l);
            }
            rLog(up_env_ric, "Intersection test ends at \n %s",
                    (left_most == build_env->end()) ?
                    "NO_INTERS" : thing_to_cstring(*left_most));
        }

        // log the current envelope after all intersections found
        rLog(up_env_ric, "Building envelope after intersections:\n%s",
                vect_to_cstring(build_env->begin(), build_env->end()));

        // set up the next upper envelope
        next_env->push_back(build_env->front());
        Building_envelope_cell* left_list = &(*(build_env->begin()));
        Building_envelope_cell* left_list_tail = &(*(build_env->begin()));
        Building_env_iter inf_cell = build_env->begin()+(build_env->size()-1);

        // compute the upper envelope in each cell
        for (Building_env_iter ci = build_env->begin()+1 ; ci != inf_cell ; ++ci) {

            // log the cell we are updating
            rLog(up_env_ric, "Cell to update\n%s", thing_to_cstring(*ci));

            // construct and filter the three sorted list of lines
            Three_sorted_lists_of_lines L(*ci, left_list, left_list_tail);
            rLog(up_env_ric, "Init three sorted lists:\n%s", thing_to_cstring(L));
            L.filter_below();

            // compute the discrete upper envelope of the intersecting lines
            due_w_bin_search_of_sorted_and_filtered_lines(
                    *next_env, L, ci->left, (ci+1)->left-1);

            // log the envelope
            rLog(up_env_ric, "Next Envelope\n%s",
                    vect_to_cstring(next_env->begin(), next_env->end()));

            // update the left list by filtering all lines that are before
            // the filter in the all sorted list
            L.filter_all_sorted(*(next_env->back().line));
            left_list = L.all_sorted_head.ptr;
            left_list_tail = L.all_sorted_tail;
            L.all_sorted_head.ptr = NULL;
        }

        // copy the infinite cell to the end
        next_env->push_back(build_env->back());

        // log the current envelope after all intersections found
        rLog(up_env_ric, "Next Envelope:\n%s",
                vect_to_cstring(next_env->begin(), next_env->end()));

        // update for the next round
        std::swap(next_env, build_env);
        next_env->resize(0);

        delta *= 2;
        end_offset = begin_offset;
        begin_offset = std::min(end_offset+delta, num_lines-1);
    }


    // copy build envelope over to envelope
    building_envelope_to_envelope(envelope_, *build_env);
}


void Discrete_upper_envelope::build_ulogu_with_slope_unique_lines_sorted_by_slope(
        const std::vector< const Line* >& sorted_lines,
        Building_envelope_mem& build_env) {

    // init the sorted line processor and building_envelope

    Line neg_inf(INT_MIN, INT_MIN);
    Line pos_inf(INT_MAX, INT_MAX);
    One_sorted_list_of_lines processor(sorted_lines);
    build_env.resize(0);
    build_env.push_back(Building_envelope_cell(&neg_inf, one()-1));

    // compute the due for sorted and filtered lines between one..U
    due_w_bin_search_of_sorted_and_filtered_lines(build_env, processor, one(), U());
    build_env.push_back(Building_envelope_cell(&pos_inf, U()+1));

    // convert the building envelope to an envelope
    building_envelope_to_envelope(envelope_, build_env);

    // log the envelope
    rLog(up_env_ulogu, "Output of build_ulogu:\n %s",
           vect_to_cstring(envelope_.begin(), envelope_.end()));
}


void Discrete_upper_envelope::build_orient_with_slope_unique_lines_sorted_by_slope(
        const std::vector< const Line* >& sorted_lines,
        Building_with_orient_mem& dual_points) {

    // typedefs and preconditions
    typedef Point_2::Std_point Point;
    typedef Poly_chain_2::Vertex_list::const_iterator Vertex_iter;

    // if there is only 1 line then we are done
    if (sorted_lines.size() == 1) {
        envelope_.push_back(Envelope_cell(*sorted_lines.front(), one(), U()));
        return;
    }

    // dualize the lines
    dual_points.resize(0);
    for (std::vector< const Line* >::const_iterator iter = sorted_lines.begin() ;
            iter != sorted_lines.end() ; ++iter) {
        dual_points.push_back(Line_2::dual(**iter));
    }

    // log the points
    rLog(up_env_orient, "Dual Points:\n %s",
            vect_to_cstring(dual_points.begin(), dual_points.end()));

    // compute the lower hull of the duals points
    Convex_hull::Graham_yao ch;
    Poly_chain_2 lower_hull;
    lower_hull.reserve(dual_points.size());
    ch.build_lower_hull(lower_hull, dual_points);

    // log the lower envelope
    rLog(up_env_orient, "Lower hull\n %s", thing_to_cstring(lower_hull));

    // iterate over the lines of the upper envelope and
    // convert them to cells
    int cell_begin = one();
    for (Vertex_iter pi = lower_hull.vertices_begin() ;
            pi != lower_hull.vertices_end() && cell_begin <= U() ; ++pi) {

        // compute the end of the envelope for line corresponding to pi
        // by getting the next dual point
        // if the point is the last point then dual(pi) is the
        // remainder of the upper envelope.
        // if it is not, compute the intersection of dual(pi) and
        // dual(pip1) to determine where the cell ends
        int cell_end;
        Vertex_iter pip1 = DDAD_util::next(pi);
        const Line* l_pi = (const Line*)pi->data();
        if (pip1 == lower_hull.vertices_end()) { cell_end = U(); }
        else {
            int x_coord;
            const Line* l_pip1 = (const Line*)pip1->data();
            Line_2::vertical_grid_line_left_or_thourgh_intersection(
                    x_coord, *l_pi, *l_pip1);
            cell_end = std::min(U(), x_coord);
        }

        if (cell_end >= cell_begin) {
            envelope_.push_back(Envelope_cell(*l_pi, cell_begin, cell_end));
            cell_begin = cell_end+1;
        }
    }

    // log the sorted list
    rLog(up_env_orient, "Output of build_orient:\n %s",
           vect_to_cstring(envelope_.begin(), envelope_.end()));
}


/********************************************************
 *                 Envelope Calculation
 ********************************************************/
void Discrete_upper_envelope::build(const std::vector< Line >& input, int UB) {
    build_ric(input, 1, UB);
}


void Discrete_upper_envelope::build_ric(const std::vector< Line >& input,
        int LB, int UB) {

    // set the size
    init_build_and_verify_input(input, LB, UB);

    // log the input
    rLog(up_env_ric, "Input for build_ric\n %s",
            vect_to_string(input.begin(),input.end(), "\t","\n",true).c_str());

    // sort the lines by slope filtering duplicate slopes
    std::list< const Line* > sorted_lines;
    sort_and_filter_by_slope(back_inserter(sorted_lines),
            input.begin(), input.end());

    // log the sorted and filtered lines
    rLog(up_env_ric, "Sorted filtered lines:\n%s",
            vect_of_ptrs_to_cstring(sorted_lines.begin(), sorted_lines.end()));

    int num_lines = sorted_lines.size();
    Rand_order_mem ro(rand_order_mem_size(num_lines));
    Building_envelope_mem be01(building_envelope_mem_size(num_lines));
    Building_envelope_mem be02(building_envelope_mem_size(num_lines));
    build_ric_with_slope_unique_lines_sorted_by_slope(sorted_lines, ro, be01, be02);
}


void Discrete_upper_envelope::build_ulogu(
        const std::vector< Line >& input, int LB, int UB) {

    // init and verify
    init_build_and_verify_input(input, LB, UB);

    // log the input
    rLog(up_env_ulogu, "Input for build_ulogu\n %s",
            vect_to_string(input.begin(),input.end(), "\t","\n",true).c_str());

    // sort the lines by slope filtering duplicate slopes
    std::vector< const Line* > sorted_lines;
    sort_and_filter_by_slope(back_inserter(sorted_lines),
            input.begin(), input.end());

    // log the sorted and filtered lines
    rLog(up_env_ulogu, "Sorted filtered lines:\n%s",
            vect_of_ptrs_to_cstring(sorted_lines.begin(), sorted_lines.end()));


    // reserve space for the building envelope
    Building_envelope build_env;
    build_env.reserve(building_envelope_mem_size(sorted_lines.size()));

    // build the upper envelope
    build_ulogu_with_slope_unique_lines_sorted_by_slope(sorted_lines, build_env);
}


void Discrete_upper_envelope::build_orient(
        const std::vector< Line >& input, int LB, int UB) {

    // typedefs and preconditions
    typedef Point_2::Std_point Point;

    // init and verify
    init_build_and_verify_input(input, LB, UB);

    // log the input
    rLog(up_env_orient, "Input for build_orient\n %s",
            vect_to_string(input.begin(),input.end(), "\t","\n",true).c_str());

    // sort the lines by slope filtering duplicate slopes
    std::vector< const Line* > sorted_lines;
    sort_and_filter_by_slope(back_inserter(sorted_lines),
            input.begin(), input.end());

    // build the DUE
    std::vector< Point > dual_points;
    dual_points.reserve(sorted_lines.size());
    build_orient_with_slope_unique_lines_sorted_by_slope(sorted_lines, dual_points);
}


void Discrete_upper_envelope::build_brute_force(
        const std::vector< Line >& input, int LB, int UB) {

    typedef std::vector< Line >::const_iterator Line_iter;
    typedef std::list< Line_iter >::iterator Line_handel_iter;

    // init and verify
    init_build_and_verify_input(input, LB, UB);

    // the current set of upper lines and envelope cells being built
    Line_iter upper_line;

    for (int x = one() ; x <= U() ; ++x) {

        // set the first line to be the upper line
        upper_line = input.begin();

        // iterate over lines find the lines that evaluates to the max val
        for (Line_iter li = DDAD_util::next(input.begin()) ;
                li != input.end() ; ++li) {

            if (line_above(*li, *upper_line, x)
                    || (line_intersect(*li, *upper_line,x)
                        && Line_2::slope_order(*li, *upper_line) == Line_2::SLOPE_ORDER_LESS)) {
                upper_line = li;
            }
        }

        // output the upper line
        rLog(up_env_brute, "Upper line %s", thing_to_cstring(*upper_line));

        // setup new cell or expand last cell
        if (envelope_.size() == 0 || envelope_.back().line != *upper_line) {
            envelope_.push_back(Envelope_cell(*upper_line, x,x));
        } else {
            envelope_.back().right = x;
        }
    }
}


void Discrete_upper_envelope::build_ric_with_slope_unique_lines_sorted_by_slope(
            const std::vector< Line >& input, int UB,
            Rand_order_mem& rand_order,
            Sorted_lines_mem&,
            Building_envelope_mem& building_envelope01,
            Building_envelope_mem& building_envelope02) {

    // log the input lines
    rLog(up_env_ric, "Input lines: \n%s",
            vect_to_cstring(input.begin(), input.end()));

    // init the problem
    init_build_and_verify_input(input, 1, UB);

    // create the sorted list of pointers to lines
    std::list< const Line * > sorted_lines(input.size());
    std::list< const Line * >::iterator target_iter = sorted_lines.begin();
    std::vector< Line >::const_iterator src_iter = input.begin();
    for ( ; src_iter != input.end() ; ++src_iter, ++target_iter) {
        *target_iter = &(*src_iter);
    }

    // log the pointers to lines
    rLog(up_env_ric, "Pointers to input lines: \n%s",
            vect_of_ptrs_to_cstring(sorted_lines.begin(), sorted_lines.end()));

    // build the due
    build_ric_with_slope_unique_lines_sorted_by_slope(sorted_lines, rand_order,
            building_envelope01, building_envelope02);
}


void Discrete_upper_envelope::build_ulogu_with_slope_unique_lines_sorted_by_slope(
        const std::vector< Line >& input, int UB,
        Sorted_lines_mem& sorted_lines,
        Building_envelope_mem& build_env) {

    // typedefs and preconditions
    typedef std::vector< const Line* >::iterator Line_ptr_iter;

    // init and verify
    init_build_and_verify_input(input, 1, UB);

    // log the input
    rLog(up_env_ulogu, "Input for build_ulogu\n %s",
            vect_to_string(input.begin(),input.end(), "\t","\n",true).c_str());

    // create the sorted list of pointers to lines
    sorted_lines.resize(input.size());
    std::vector< const Line * >::iterator target_iter = sorted_lines.begin();
    std::vector< Line >::const_iterator src_iter = input.begin();
    for ( ; src_iter != input.end() ; ++src_iter, ++target_iter) {
        *target_iter = &(*src_iter);
    }

    // log the sorted and filtered lines
    rLog(up_env_ulogu, "Sorted filtered lines:\n%s",
            vect_of_ptrs_to_cstring(sorted_lines.begin(), sorted_lines.end()));

    // build the envelope
    build_ulogu_with_slope_unique_lines_sorted_by_slope(sorted_lines, build_env);
}


void Discrete_upper_envelope::build_orient_with_slope_unique_lines_sorted_by_slope(
        const std::vector< Line >& input, int UB,
        Sorted_lines_mem& sorted_lines,
        Building_with_orient_mem& dual_points) {

    // init and verify
    init_build_and_verify_input(input, 1, UB);

    // log the input
    rLog(up_env_orient, "Input for build_orient\n %s",
            vect_to_string(input.begin(),input.end(), "\t","\n",true).c_str());

    // create the sorted list of pointers to lines
    sorted_lines.resize(input.size());
    std::vector< const Line * >::iterator target_iter = sorted_lines.begin();
    std::vector< Line >::const_iterator src_iter = input.begin();
    for ( ; src_iter != input.end() ; ++src_iter, ++target_iter) {
        *target_iter = &(*src_iter);
    }

    // build the due
    build_orient_with_slope_unique_lines_sorted_by_slope(sorted_lines, dual_points);
}


bool Discrete_upper_envelope::verify(const std::vector< Line >& input) {
    Discrete_upper_envelope verify;
    verify.build_brute_force(input, one(), U());

    Upper_envelope::const_iterator v_iter = verify.begin();
    Upper_envelope::const_iterator t_iter = begin();

    while (v_iter != verify.end() && t_iter != end() && *v_iter == *t_iter) {
        ++v_iter;
        ++t_iter;
    }
    if (v_iter != verify.end() || t_iter != end()) {
        rError("Verify failed...\nBrute Force DUE:\n%sComputed DUE\n%s\n",
                thing_to_cstring(verify), thing_to_cstring(*this));
    }

    return (v_iter == verify.end() && t_iter == end());
}

std::string Discrete_upper_envelope::to_string() const {
    return vect_to_string(envelope_.begin(), envelope_.end(), "\t", "\n");
}

