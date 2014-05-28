/**
 * Filename: Poly_chain_2.cpp
 * Project name: degDrivenAlgoDesign
 * Created on: 26 May 2011
 * @author: David L. Millman <dave@cs.unc.edu>
 */

#include "Poly_chain_2.h"

void Poly_chain_2::splice(Poly_chain_2& p) {

    if (is_closed() || p.is_closed()) {
        rWarning("Attempt to append closed chains");
        return;
    }

    // check if the end of this chain matches
    if (num_vertices() != 0 && p.num_vertices() != 0
            && vertices_.back() == *(p.vertices_begin())) {
        vertices_.pop_back();
    }
    //vertices_.splice(vertices_.end(), p.vertices_);
    vertices_.insert(vertices_.end(), p.vertices_.begin(), p.vertices_.end());
}


void Poly_chain_2::append(const Point& p) {

    if (is_closed()) {
        rWarning("Attempt to append to a closed chain");
        return;
    }

    // check if the end of this chain matches p
    if (num_vertices() != 0 && vertices_.back() == p) { return; }

    vertices_.push_back(p);

}


std::string Poly_chain_2::to_string() const {
    std::ostringstream os;
    os << DDAD_util::vect_to_string(vertices_.begin(), vertices_.end());
    if (is_closed()) {
        os << DDAD_util::vect_to_string(vertices_.begin(), ++(vertices_.begin()));
    }
    return os.str();
}


void Poly_chain_2::remove_back() {
    if (is_closed()) {
        rWarning("Attempt to remover a vertex from a closed chain");
        return;
    }

    if (num_vertices() == 0) { return;}
    vertices_.pop_back();
}


void Poly_chain_2::close_chain() {

    if (is_closed()) { return; }

    // if the front and back vertex have the same coordniate
    if ( !vertices_.empty()
            && vertices_.back() == vertices_.front()) {
        vertices_.pop_back();
    }

    is_open_ = false;
}

