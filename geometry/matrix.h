/*
 * This file is part of DDAD.
 *
 * DDAD is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * DDAD is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details. You should have received a copy of the GNU General Public
 * License along with DDAD. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GE_MATRIX_H
#define GE_MATRIX_H

#include "common.h"
#include "point.h"
#include "vector.h"

namespace DDAD {

//=============================================================================
// Interface: Matrix_2x2i
//=============================================================================

class Matrix_2x2i {
public:
    Matrix_2x2i();
    Matrix_2x2i(const integer& a, const integer& b,
                const integer& c, const integer& d);

      const integer& operator()(const size_t row, const size_t col) const;
      integer& operator()(const size_t row, const size_t col);
      const std::array<integer, 4>& elements() const;

  private:
      std::array<integer, 4> elements_;
};

Vector_2i RowVec(const Matrix_2x2i& mat, const size_t row);
Vector_2i ColVec(const Matrix_2x2i& mat, const size_t col);

integer Determinant(const Matrix_2x2i& mat);
Matrix_2x2i Inverse(const Matrix_2x2i& mat);
Matrix_2x2i Transpose(const Matrix_2x2i& mat);
std::string to_string(const Matrix_2x2i& mat);

bool operator==(const Matrix_2x2i& lhs, const Matrix_2x2i& rhs);
bool operator!=(const Matrix_2x2i& lhs, const Matrix_2x2i& rhs);
Matrix_2x2i operator+(const Matrix_2x2i& lhs, const Matrix_2x2i& rhs);
Matrix_2x2i operator-(const Matrix_2x2i& lhs, const Matrix_2x2i& rhs);
Matrix_2x2i operator-(const Matrix_2x2i& mat);
Matrix_2x2i operator*(const Matrix_2x2i& lhs, const Matrix_2x2i& rhs);
Matrix_2x2i operator*(const Matrix_2x2i& lhs, const integer& rhs);
Matrix_2x2i operator*(const integer& lhs, const Matrix_2x2i& rhs);
//Matrix_2x2i operator/(const Matrix_2x2i& lhs, const integer rhs);
//Matrix_2x2i& operator+=(const Matrix_2x2i& lhs, const Matrix_2x2i& rhs);
//Matrix_2x2i& operator-=(const Matrix_2x2i& lhs, const Matrix_2x2i& rhs);
//Matrix_2x2i& operator*=(const Matrix_2x2i& lhs, const Matrix_2x2i& rhs);
//Matrix_2x2i& operator*=(const Matrix_2x2i& lhs, const integer rhs);
//Matrix_2x2i& operator/=(const Matrix_2x2i& lhs, const Matrix_2x2i& rhs);
//Matrix_2x2i& operator/=(const Matrix_2x2i& lhs, const integer rhs);
std::ostream& operator<<(std::ostream& o, const Matrix_2x2i& mat);

//=============================================================================
// Interface: Mixed Matrix_2x2i arithmetic
//=============================================================================

Point_2i operator*(const Point_2i& p, const Matrix_2x2i& mat);
Point_2i operator*(const Matrix_2x2i& mat, const Point_2i& p);
Vector_2i operator*(const Vector_2i& v, const Matrix_2x2i& mat);
Vector_2i operator*(const Matrix_2x2i& mat, const Vector_2i& v);

//=============================================================================
// Implementation: Matrix_2x2i
//=============================================================================

inline Matrix_2x2i::Matrix_2x2i() {
    Matrix_2x2i& mat = *this;
    mat(0,0) = 1;
    mat(0,1) = 0;
    mat(1,0) = 0;
    mat(1,1) = 1;
}

inline Matrix_2x2i::Matrix_2x2i(const integer& a, const integer& b,
                                const integer& c, const integer& d) {
    Matrix_2x2i& mat = *this;
    mat(0,0) = a;
    mat(0,1) = b;
    mat(1,0) = c;
    mat(1,1) = d;
}

inline const integer& Matrix_2x2i::operator()(const size_t row,
                                              const size_t col) const {
    return elements_[(col*2)+row];
}

inline integer& Matrix_2x2i::operator()(const size_t row, const size_t col) {
    return elements_[(col*2)+row];
}

inline Matrix_2x2i operator*(const Matrix_2x2i& lhs, const Matrix_2x2i& rhs) {
    Matrix_2x2i result;
    for(int r = 0; r < 2; ++r) {
        for(int c = 0; c < 2; ++c) {
            // it's not the other way around with transposing the rhs, right?
            result(r,c) = Transpose(lhs)(r,c) * rhs(r,c);
        }
    }
    return result;
}

inline Matrix_2x2i operator+(const Matrix_2x2i& lhs, const Matrix_2x2i& rhs) {
    Matrix_2x2i result = Matrix_2x2i();
    for(int r = 0; r < 2; ++r) {
        for(int c = 0; c < 2; ++c) {
            result(r,c) = lhs(r,c)+rhs(r,c);
        }
    }
    return result;
}

inline Matrix_2x2i operator-(const Matrix_2x2i& lhs, const Matrix_2x2i& rhs) {
    return lhs + (-rhs);
}

inline Matrix_2x2i operator-(const Matrix_2x2i& mat) {
      return Matrix_2x2i(-mat(0,0), -mat(0,1),
                         -mat(1,0), -mat(1,1));
}

inline Matrix_2x2i Transpose(const Matrix_2x2i& mat) {
    return Matrix_2x2i(mat(0,0), mat(1,0),
                       mat(0,1), mat(1,1));
}

inline bool operator==(const Matrix_2x2i& lhs, const Matrix_2x2i& rhs) {
    return lhs.elements() == rhs.elements();
}

inline bool operator!=(const Matrix_2x2i& lhs, const Matrix_2x2i& rhs) {
    return lhs.elements() != rhs.elements();
}

inline Matrix_2x2i operator*(const integer& lhs, const Matrix_2x2i& rhs) {
    return Matrix_2x2i(lhs*rhs(0,0), lhs*rhs(0,1),
                       lhs*rhs(1,0), lhs*rhs(1,1));
}

inline Matrix_2x2i operator*(const Matrix_2x2i& lhs, const integer& rhs) {
    return rhs*lhs;
}

inline std::ostream& operator<<(std::ostream& o, const Matrix_2x2i& mat) {
    o << to_string(mat);
    return o;
}

inline std::string to_string(const Matrix_2x2i& mat) {
    std::stringstream result;
    result << "[" << mat(0,0) << " " << mat(0,1) << "]\n"
           << "[" << mat(1,0) << " " << mat(1,1) << "]";
    return result.str();
}

inline Vector_2i RowVec(const Matrix_2x2i& mat, const size_t row) {
    return Vector_2i(mat(row, 0), mat(row, 1));
}

inline Vector_2i ColVec(const Matrix_2x2i& mat, const size_t col) {
    return Vector_2i(mat(0, col), mat(1, col));
}

inline integer Determinant(const Matrix_2x2i& mat) {
    return mat(0,0)*mat(1,1)-mat(0,1)*mat(1,0);
}

inline Matrix_2x2i Inverse(const Matrix_2x2i& mat) {
    integer det = Determinant(mat);
    return Matrix_2x2i( mat(1,1)/det, -mat(0,1)/det,
                       -mat(1,0)/det,  mat(0,0)/det);
}

// Accessors/Mutators =========================================================

inline const std::array<integer, 4>& Matrix_2x2i::elements() const {
    return elements_;
}

//=============================================================================
// Implementation: Mixed Matrix_2x2i arithmetic
//=============================================================================

inline Point_2i operator*(const Point_2i& p, const Matrix_2x2i& mat) {
    return Point_2i(Dot(ColVec(mat, 0), p),
                    Dot(ColVec(mat, 1), p));
}

inline Point_2i operator*(const Matrix_2x2i& mat, const Point_2i& p) {
    return Point_2i(Dot(RowVec(mat, 0), p),
                    Dot(RowVec(mat, 1), p));
}

inline Vector_2i operator*(const Vector_2i& v, const Matrix_2x2i& mat) {
    return Vector_2i(Dot(ColVec(mat, 0), v),
                     Dot(ColVec(mat, 1), v));
}

inline Vector_2i operator*(const Matrix_2x2i& mat, const Vector_2i& v) {
    return Vector_2i(Dot(RowVec(mat, 0), v),
                     Dot(RowVec(mat, 1), v));
}

//=============================================================================
// Interface: Matrix_2x2r
//=============================================================================

class Matrix_2x2r {
public:
    Matrix_2x2r();
    Matrix_2x2r(const rational& a, const rational& b,
                const rational& c, const rational& d);

    const rational& operator()(const size_t row, const size_t col) const;
    rational& operator()(const size_t row, const size_t col);
    const std::array<rational, 4>& elements() const;

private:
    std::array<rational, 4> elements_;
};

rational Determinant(const Matrix_2x2r& mat);

/*
bool operator==(const Matrix_2x2r& lhs, const Matrix_2x2r& rhs);
bool operator!=(const Matrix_2x2r& lhs, const Matrix_2x2r& rhs);
Matrix_2x2r operator+(const Matrix_2x2r& lhs, const Matrix_2x2r& rhs);
Matrix_2x2r operator-(const Matrix_2x2r& lhs, const Matrix_2x2r& rhs);
Matrix_2x2r operator-(const Matrix_2x2r& M);
Matrix_2x2r operator*(const Matrix_2x2r& lhs, const Matrix_2x2r& rhs);
Matrix_2x2r operator*(const Matrix_2x2r& lhs, const rational& rhs);
Matrix_2x2r operator/(const Matrix_2x2r& lhs, const rational& rhs);
Matrix_2x2r& operator+=(const Matrix_2x2r& lhs, const Matrix_2x2r& rhs);
Matrix_2x2r& operator-=(const Matrix_2x2r& lhs, const Matrix_2x2r& rhs);
Matrix_2x2r& operator*=(const Matrix_2x2r& lhs, const Matrix_2x2r& rhs);
Matrix_2x2r& operator*=(const Matrix_2x2r& lhs, const rational& rhs);
Matrix_2x2r& operator/=(const Matrix_2x2r& lhs, const Matrix_2x2r& rhs);
Matrix_2x2r& operator/=(const Matrix_2x2r& lhs, const rational& rhs);
std::ostream& operator<<(std::ostream& o, const Matrix_2x2r& mat);

Matrix_2x2r Inverse(const Matrix_2x2r& M);
Matrix_2x2r Transpose(const Matrix_2x2r& M);
std::string to_string(const Matrix_2x2r& M);
*/

//=============================================================================
// Implementation: Matrix_2x2r
//=============================================================================

inline Matrix_2x2r::Matrix_2x2r() {
    Matrix_2x2r& mat = *this;
    mat(0,0) = 1;
    mat(0,1) = 0;
    mat(1,0) = 0;
    mat(1,1) = 1;
}

inline Matrix_2x2r::Matrix_2x2r(const rational& a, const rational& b,
                                const rational& c, const rational& d) {
    Matrix_2x2r& mat = *this;
    mat(0,0) = a;
    mat(0,1) = b;
    mat(1,0) = c;
    mat(1,1) = d;
}

inline const rational& Matrix_2x2r::operator()(const size_t row,
                                               const size_t col) const {
    return elements_[(col*2)+row];
}

inline rational& Matrix_2x2r::operator()(const size_t row, const size_t col) {
    return elements_[(col*2)+row];
}

inline rational Determinant(const Matrix_2x2r& mat) {
    return mat(0,0)*mat(1,1)-mat(0,1)*mat(1,0);
}

// Accessors/Mutators =========================================================

inline const std::array<rational, 4>& Matrix_2x2r::elements() const {
    return elements_;
}

//=============================================================================
// Interface: Matrix_3x3i
//=============================================================================

class Matrix_3x3i {
public:
    Matrix_3x3i();
    Matrix_3x3i(const integer& m00, const integer& m01, const integer& m02,
                const integer& m10, const integer& m11, const integer& m12,
                const integer& m20, const integer& m21, const integer& m22);

    const integer& operator()(const size_t row, const size_t col) const;
    integer& operator()(const size_t row, const size_t col);
    const std::array<integer, 9>& elements() const;

private:
    std::array<integer, 9> elements_;
};

bool operator==(const Matrix_3x3i& lhs, const Matrix_3x3i& rhs);
bool operator!=(const Matrix_3x3i& lhs, const Matrix_3x3i& rhs);
Matrix_3x3i operator*(const Matrix_3x3i& lhs, const Matrix_3x3i& rhs);
std::ostream& operator<<(std::ostream& o, const Matrix_3x3i& mat);

Vector_3i RowVec(const Matrix_3x3i& mat, const int row);
Vector_3i ColVec(const Matrix_3x3i& mat, const int col);

integer Determinant(const Matrix_3x3i& mat);
Matrix_3x3i Inverse(const Matrix_3x3i& mat);
//Matrix_3x3i Transpose(const Matrix_3x3i& mat);
std::string to_string(const Matrix_3x3i& mat);

/*
bool operator==(const Matrix_3x3i& lhs, const Matrix_3x3i& rhs);
bool operator!=(const Matrix_3x3i& lhs, const Matrix_3x3i& rhs);
Matrix_3x3i operator+(const Matrix_3x3i& lhs, const Matrix_3x3i& rhs);
Matrix_3x3i operator-(const Matrix_3x3i& lhs, const Matrix_3x3i& rhs);
Matrix_3x3i operator-(const Matrix_3x3i& mat);
Matrix_3x3i operator*(const Matrix_3x3i& lhs, const Matrix_3x3i& rhs);
Matrix_3x3i operator*(const Matrix_3x3i& lhs, const integer& rhs);
Matrix_3x3i operator/(const Matrix_3x3i& lhs, const integer& rhs);
Matrix_3x3i& operator+=(const Matrix_3x3i& lhs, const Matrix_3x3i& rhs);
Matrix_3x3i& operator-=(const Matrix_3x3i& lhs, const Matrix_3x3i& rhs);
Matrix_3x3i& operator*=(const Matrix_3x3i& lhs, const Matrix_3x3i& rhs);
Matrix_3x3i& operator*=(const Matrix_3x3i& lhs, const integer& rhs);
Matrix_3x3i& operator/=(const Matrix_3x3i& lhs, const Matrix_3x3i& rhs);
Matrix_3x3i& operator/=(const Matrix_3x3i& lhs, const integer& rhs);
*/

//=============================================================================
// Interface: Mixed Matrix_3x3i arithmetic
//=============================================================================

Point_3i operator*(const Point_3i& lhs, const Matrix_3x3i& rhs);
Point_3i operator*(const Matrix_3x3i& lhs, const Point_3i& rhs);
Vector_3i operator*(const Vector_3i& lhs, const Matrix_3x3i& rhs);
Vector_3i operator*(const Matrix_3x3i& lhs, const Vector_3i& rhs);

//=============================================================================
// Implementation: Matrix_3x3i
//=============================================================================

inline Matrix_3x3i::Matrix_3x3i() {
    Matrix_3x3i& mat = *this;
    mat(0,0) = 1;
    mat(0,1) = 0;
    mat(0,2) = 0;
    mat(1,0) = 0;
    mat(1,1) = 1;
    mat(1,2) = 0;
    mat(2,0) = 0;
    mat(2,1) = 0;
    mat(2,2) = 1;
}

inline Matrix_3x3i::Matrix_3x3i(const integer& m00, const integer& m01,
                                const integer& m02, const integer& m10,
                                const integer& m11, const integer& m12,
                                const integer& m20, const integer& m21,
                                const integer& m22) {
    Matrix_3x3i& mat = *this;
    mat(0,0) = m00;
    mat(0,1) = m01;
    mat(0,2) = m02;
    mat(1,0) = m10;
    mat(1,1) = m11;
    mat(1,2) = m12;
    mat(2,0) = m20;
    mat(2,1) = m21;
    mat(2,2) = m22;
}

inline const integer& Matrix_3x3i::operator()(const size_t row,
                                               const size_t col) const {
    return elements_[(col*3)+row];
}

inline integer& Matrix_3x3i::operator()(const size_t row, const size_t col) {
    return elements_[(col*3)+row];
}

inline bool operator==(const Matrix_3x3i& lhs, const Matrix_3x3i& rhs) {
    return lhs.elements() == rhs.elements();
}

inline bool operator!=(const Matrix_3x3i& lhs, const Matrix_3x3i& rhs) {
    return lhs.elements() != rhs.elements();
}

inline Vector_3i RowVec(const Matrix_3x3i& mat, const int row) {
    return Vector_3i(mat(row,0), mat(row,1), mat(row,2));
}

inline Vector_3i ColVec(const Matrix_3x3i& mat, const int col) {
    return Vector_3i(mat(0,col), mat(1,col), mat(2,col));
}

inline Matrix_3x3i operator*(const Matrix_3x3i& lhs, const Matrix_3x3i& rhs) {
    auto row0 = RowVec(lhs, 0);
    auto row1 = RowVec(lhs, 1);
    auto row2 = RowVec(lhs, 2);
    auto col0 = ColVec(rhs, 0);
    auto col1 = ColVec(rhs, 1);
    auto col2 = ColVec(rhs, 2);
    return Matrix_3x3i(Dot(row0, col0), Dot(row0, col1), Dot(row0, col2),
                       Dot(row1, col0), Dot(row1, col1), Dot(row1, col2),
                       Dot(row2, col0), Dot(row2, col1), Dot(row2, col2));
}

inline std::ostream& operator<<(std::ostream& o, const Matrix_3x3i& mat) {
    return o << to_string(mat);
}

inline std::string to_string(const Matrix_3x3i& mat) {
    std::stringstream result;
    result << "[" << mat(0,0) << " " << mat(0,1) << " " << mat(0,2) << "]\n"
           << "[" << mat(1,0) << " " << mat(1,1) << " " << mat(1,2) << "]\n"
           << "[" << mat(2,0) << " " << mat(2,1) << " " << mat(2,2) << "]";
    return result.str();
}

// Accessors/Mutators =========================================================

inline const std::array<integer, 9>& Matrix_3x3i::elements() const {
    return elements_;
}

//=============================================================================
// Implementation: Mixed Matrix_3x3i arithmetic
//=============================================================================

inline Point_3i operator*(const Point_3i& p, const Matrix_3x3i& mat) {
    return Point_3i(Dot(ColVec(mat, 0), p),
                    Dot(ColVec(mat, 1), p),
                    Dot(ColVec(mat, 2), p));
}

inline Point_3i operator*(const Matrix_3x3i& mat, const Point_3i& p) {
    return Point_3i(Dot(RowVec(mat, 0), p),
                    Dot(RowVec(mat, 1), p),
                    Dot(RowVec(mat, 2), p));
}

inline Vector_3i operator*(const Vector_3i& v, const Matrix_3x3i& mat) {
    return Vector_3i(Dot(ColVec(mat, 0), v),
                     Dot(ColVec(mat, 1), v),
                     Dot(ColVec(mat, 2), v));
}

inline Vector_3i operator*(const Matrix_3x3i& mat, const Vector_3i& v) {
    return Vector_3i(Dot(RowVec(mat, 0), v),
                     Dot(RowVec(mat, 1), v),
                     Dot(RowVec(mat, 2), v));
}

//=============================================================================
// Interface: Matrix_3x3r
//=============================================================================

class Matrix_3x3r {
public:
    Matrix_3x3r();
    Matrix_3x3r(const rational& m00, const rational& m01, const rational& m02,
                const rational& m10, const rational& m11, const rational& m12,
                const rational& m20, const rational& m21, const rational& m22);
    Matrix_3x3r(const Matrix_3x3i& imat);

    const rational& operator()(const size_t row, const size_t col) const;
    rational& operator()(const size_t row, const size_t col);
    const std::array<rational, 9>& elements() const;

private:
    std::array<rational, 9> elements_;
};

bool operator==(const Matrix_3x3r& lhs, const Matrix_3x3r& rhs);
bool operator!=(const Matrix_3x3r& lhs, const Matrix_3x3r& rhs);
std::ostream& operator<<(std::ostream& o, const Matrix_3x3r& mat);

Vector_3r RowVec(const Matrix_3x3r& mat, const int row);
Vector_3r ColVec(const Matrix_3x3r& mat, const int col);

rational Determinant(const Matrix_3x3r& mat);
Matrix_3x3r Inverse(const Matrix_3x3r& mat);
//Matrix_3x3r Transpose(const Matrix_3x3r& mat);
std::string to_string(const Matrix_3x3r& mat);

/*
bool operator==(const Matrix_3x3r& lhs, const Matrix_3x3r& rhs);
bool operator!=(const Matrix_3x3r& lhs, const Matrix_3x3r& rhs);
Matrix_3x3r operator+(const Matrix_3x3r& lhs, const Matrix_3x3r& rhs);
Matrix_3x3r operator-(const Matrix_3x3r& lhs, const Matrix_3x3r& rhs);
Matrix_3x3r operator-(const Matrix_3x3r& mat);
Matrix_3x3r operator*(const Matrix_3x3r& lhs, const Matrix_3x3r& rhs);
Matrix_3x3r operator*(const Matrix_3x3r& lhs, const rational& rhs);
Matrix_3x3r operator/(const Matrix_3x3r& lhs, const rational& rhs);
Matrix_3x3r& operator+=(const Matrix_3x3r& lhs, const Matrix_3x3r& rhs);
Matrix_3x3r& operator-=(const Matrix_3x3r& lhs, const Matrix_3x3r& rhs);
Matrix_3x3r& operator*=(const Matrix_3x3r& lhs, const Matrix_3x3r& rhs);
Matrix_3x3r& operator*=(const Matrix_3x3r& lhs, const rational& rhs);
Matrix_3x3r& operator/=(const Matrix_3x3r& lhs, const Matrix_3x3r& rhs);
Matrix_3x3r& operator/=(const Matrix_3x3r& lhs, const rational& rhs);
*/

//=============================================================================
// Interface: Mixed Matrix_3x3r arithmetic
//=============================================================================

Point_3r operator*(const Point_3r& lhs, const Matrix_3x3r& rhs);
Vector_3r operator*(const Vector_3r& lhs, const Matrix_3x3r& rhs);

//=============================================================================
// Implementation: Matrix_3x3r
//=============================================================================

inline Matrix_3x3r::Matrix_3x3r() {
    Matrix_3x3r& mat = *this;
    mat(0,0) = 1;
    mat(0,1) = 0;
    mat(0,2) = 0;
    mat(1,0) = 0;
    mat(1,1) = 1;
    mat(1,2) = 0;
    mat(2,0) = 0;
    mat(2,1) = 0;
    mat(2,2) = 1;
}

inline Matrix_3x3r::Matrix_3x3r(const rational& m00, const rational& m01,
                                const rational& m02, const rational& m10,
                                const rational& m11, const rational& m12,
                                const rational& m20, const rational& m21,
                                const rational& m22) {
    Matrix_3x3r& mat = *this;
    mat(0,0) = m00; mat(0,0).canonicalize();
    mat(0,1) = m01; mat(0,1).canonicalize();
    mat(0,2) = m02; mat(0,2).canonicalize();
    mat(1,0) = m10; mat(1,0).canonicalize();
    mat(1,1) = m11; mat(1,1).canonicalize();
    mat(1,2) = m12; mat(1,2).canonicalize();
    mat(2,0) = m20; mat(2,0).canonicalize();
    mat(2,1) = m21; mat(2,1).canonicalize();
    mat(2,2) = m22; mat(2,2).canonicalize();
}

inline Matrix_3x3r::Matrix_3x3r(const Matrix_3x3i& imat) {
    Matrix_3x3r& mat = *this;
    mat(0,0) = imat(0,0);
    mat(0,1) = imat(0,1);
    mat(0,2) = imat(0,2);
    mat(1,0) = imat(1,0);
    mat(1,1) = imat(1,1);
    mat(1,2) = imat(1,2);
    mat(2,0) = imat(2,0);
    mat(2,1) = imat(2,1);
    mat(2,2) = imat(2,2);
}

inline const rational& Matrix_3x3r::operator()(const size_t row,
                                               const size_t col) const {
    return elements_[(col*3)+row];
}

inline rational& Matrix_3x3r::operator()(const size_t row, const size_t col) {
    return elements_[(col*3)+row];
}

inline bool operator==(const Matrix_3x3r& lhs, const Matrix_3x3r& rhs) {
    return lhs.elements() == rhs.elements();
}

inline bool operator!=(const Matrix_3x3r& lhs, const Matrix_3x3r& rhs) {
    return lhs.elements() != rhs.elements();
}

inline Vector_3r RowVec(const Matrix_3x3r& mat, const int row) {
    return Vector_3r(mat(row,0), mat(row,1), mat(row,2));
}

inline Vector_3r ColVec(const Matrix_3x3r& mat, const int col) {
    return Vector_3r(mat(0,col), mat(1,col), mat(2,col));
}

inline Matrix_3x3r operator*(const Matrix_3x3r& lhs, const Matrix_3x3r& rhs) {
    auto row0 = RowVec(lhs, 0);
    auto row1 = RowVec(lhs, 1);
    auto row2 = RowVec(lhs, 2);
    auto col0 = ColVec(rhs, 0);
    auto col1 = ColVec(rhs, 1);
    auto col2 = ColVec(rhs, 2);
    return Matrix_3x3r(Dot(row0, col0), Dot(row0, col1), Dot(row0, col2),
                       Dot(row1, col0), Dot(row1, col1), Dot(row1, col2),
                       Dot(row2, col0), Dot(row2, col1), Dot(row2, col2));
}

inline std::ostream& operator<<(std::ostream& o, const Matrix_3x3r& mat) {
    return o << to_string(mat);
}

inline std::string to_string(const Matrix_3x3r& mat) {
    std::stringstream result;
    result << "[" << mat(0,0) << " " << mat(0,1) << " " << mat(0,2) << "]\n"
           << "[" << mat(1,0) << " " << mat(1,1) << " " << mat(1,2) << "]\n"
           << "[" << mat(2,0) << " " << mat(2,1) << " " << mat(2,2) << "]";
    return result.str();
}

// Accessors/Mutators =========================================================

inline const std::array<rational, 9>& Matrix_3x3r::elements() const {
    return elements_;
}

//=============================================================================
// Implementation: Mixed Matrix_3x3r arithmetic
//=============================================================================

inline Point_3r operator*(const Point_3r& p, const Matrix_3x3r& mat) {
    return Point_3r(Dot(ColVec(mat, 0), p),
                    Dot(ColVec(mat, 1), p),
                    Dot(ColVec(mat, 2), p));
}

inline Vector_3r operator*(const Vector_3r& v, const Matrix_3x3r& mat) {
    return Vector_3r(Dot(ColVec(mat, 0), v),
                     Dot(ColVec(mat, 1), v),
                     Dot(ColVec(mat, 2), v));
}

} // namespace DDAD

#endif // GE_MATRIX_H
