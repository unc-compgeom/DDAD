/**
 * Filename: Poly_chain_2.h
 * Project name: degDrivenAlgoDesign
 * Created on: 26 May 2011
 * @author: David L. Millman <dave@cs.unc.edu>
 */


#ifndef _DDAD_POLY_CHAIN_2_H_
#define _DDAD_POLY_CHAIN_2_H_

#include <iostream>
#include <sstream>
#include <list>

#include "DDAD_base.h"
#include "Point_2.h"


class Poly_chain_2 {

public:
    typedef Point_2::Std_point Point;
    typedef std::vector< Point > Vertex_list;

protected:

    /** the ccw ordering of the vertices of the polygon */
    Vertex_list vertices_;

    /** flag that specifies if the polychain is open or closed */
    bool is_open_;

public:

    /** create a default polychain of no vertices, which is open */
    Poly_chain_2(bool is_open=true) : is_open_(is_open) {}

    /** iterator over the list of vertices */
    Vertex_list::iterator vertices_begin() { return vertices_.begin(); }
    Vertex_list::iterator vertices_end() { return vertices_.end(); }
    Vertex_list::const_iterator vertices_begin() const {
        return vertices_.begin();
    }
    Vertex_list::const_iterator vertices_end() const {
        return vertices_.end();
    }

    Vertex_list::const_reverse_iterator vertices_rbegin() const {
        return vertices_.rbegin();
    }
    Vertex_list::const_reverse_iterator vertices_rend() const {
        return vertices_.rend();
    }

    /* return true if the polychain is open */
    bool is_open() const { return is_open_; }
    bool is_closed() const { return !is_open_; }

    /** return the number of vertices of the polygonal chain */
    int num_vertices() const { return vertices_.size(); }

    /** reserve space for n elts in the polychain */
    void reserve(int n) { vertices_.reserve(n); }

    /**
     * move polychain p onto the end of this polychain.  If the last
     * vertex of this is the same as the first vertex of p, the vertex
     * is NOT duplicated.  If the first vertex of this is the same as
     * the last vertex of p we simply have an open chain whose
     * coordinates have the same beginning and end coordinates.
     *
     * This is implemented with the constant time stl operation of splice
     * for two lists.
     *
     * Note: if this or p is a closed chain, there is no side effect
     * (except for logging warning).
     */
    void splice(Poly_chain_2& p);

    /**
     * Append the point p to the end of this polychin.  If the last vertex
     * of this is the same as p, this operation has no effect.
     *
     * Note: if this is a closed chain, there is no side effect (except
     * for logging a warning).
     */
    void append(const Point& p);

    /**
     * Remove the last vertex from the chain.  If the chain has no
     * vertices nothing happens.
     *
     * Note: if this is a closed chain, there is no side effect (except
     * for loggin a warning.
     */
    void remove_back();

    /**
     * close this chain.  If the first and last vertex have the same
     * coordinate the last vertex is removed.
     */
    void close_chain();

    /** return a constant reference to the last vertex of the chain */
    const Point& back() { return vertices_.back(); }

    /** return the string representation of the convex hull */
    std::string to_string() const;

    /** Output operator */
    friend std::ostream& operator<<(std::ostream& out,
            const Poly_chain_2& e) {
        return out << e.to_string();
    }

};

#endif
