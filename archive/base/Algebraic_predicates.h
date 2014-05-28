/**
 * Filename: Algebraic_predicates.h
 * Project name: degDrivenAlgoDesign
 * Created on: 12 Nov 2010
 * @author: David L. Millman <dave@cs.unc.edu>
 */

#ifndef _DDAD_ALGEBRAIC_PREDICATES_H_
#define _DDAD_ALGEBRAIC_PREDICATES_H_

class Algebraic_predicates {

public:
    /**
     * Compute the value of a 2x2 det of the form
     *      a b
     *      c d
     * @param a row 1 col 1 entry of the matrix
     * @param b row 1 col 2 entry of the matrix
     * @param c row 2 col 1 entry of the matrix
     * @param d row 2 col 2 entry of the matrix
     * @return the determinant of the matrix
     */
    template <class NT>
    static NT det2x2(
                    NT a, NT b,
                    NT c, NT d) { return a*d-b*c; }


    /**
     * Compute the value of a 3x3 det of a the form
     *      a b c
     *      d e f
     *      g h i
     * @param a row 1 col 1 entry of the matrix
     * @param b row 1 col 2 entry of the matrix
     * @param c row 1 col 3 entry of the matrix
     * @param d row 2 col 1 entry of the matrix
     * @param e row 2 col 2 entry of the matrix
     * @param f row 2 col 3 entry of the matrix
     * @param g row 3 col 1 entry of the matrix
     * @param h row 3 col 2 entry of the matrix
     * @param i row 3 col 3 entry of the matrix
     * @return the determinant of the matrix
     */
    template <class NT>
    static NT det3x3(
                    NT a, NT b, NT c,
                    NT d, NT e, NT f,
                    NT g, NT h, NT i) {
        return a*det2x2(e,f,h,i) - b*det2x2(d,f,g,i) + c*det2x2(d,e,g,h);
    }

    /**
     * Evaluate and returnthe sign of dot product of the vectors
     * v1 = (x1, y1) and w = (x2,y2)
     *
     * @param x1 x coordinate of vector v1
     * @param y1 y coordinate of vector v1
     * @param x2 x coordinate of vector v2
     * @param y2 y coordinate of vector v2
     * @return the sign of the dot product v1 and v2
     */
    template <class NT>
    static DDAD::Sign dot(NT x1, NT y1, NT x2, NT y2) {
        return DDAD::sign( x1*x2 + y1*y2 );
    }

};

#endif
