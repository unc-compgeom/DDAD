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

/*!
 * @brief Integer, floating-point, and rational vector types for 2D and 3D.
 */

#ifndef GE_VECTOR_H
#define GE_VECTOR_H

#include "common.h"
#include "point.h"

namespace DDAD {

//=============================================================================
// Interface: Vector_2i
//=============================================================================

class Vector_2i {
public:
    Vector_2i();
    Vector_2i(const Vector_2i& v);
    Vector_2i(const integer& x, const integer& y);

    Vector_2i& operator=(const Vector_2i& rhs);
    const integer& operator[](const size_t i) const;

    const integer& x() const;
    const integer& y() const;
    const std::array<integer, 2>& elements() const;
    void set_x(const integer& x);
    void set_y(const integer& y);
    void set_elements(const std::array<integer, 2>& elements);

private:
    std::array<integer, 2> elements_;
};

bool operator==(const Vector_2i& lhs, const Vector_2i& rhs);
bool operator!=(const Vector_2i& lhs, const Vector_2i& rhs);
Vector_2i operator+(const Vector_2i& lhs, const Vector_2i& rhs);
Vector_2i operator-(const Vector_2i& lhs, const Vector_2i& rhs);
Vector_2i operator-(const Vector_2i& v);
Vector_2i operator*(const Vector_2i& v, const integer& i);
Vector_2i operator*(const integer& i, const Vector_2i& v);
Vector_2i& operator+=(Vector_2i& lhs, const Vector_2i& rhs);
Vector_2i& operator-=(Vector_2i& lhs, const Vector_2i& rhs);
Vector_2i& operator*=(Vector_2i& v, const integer& i);
std::ostream& operator<<(std::ostream& o, const Vector_2i& v);
integer Dot(const Vector_2i& lhs, const Vector_2i& rhs);
Vector_2i RemoveFactor(const Vector_2i& v, const integer& f);
integer GCD(const Vector_2i& v);
bool IsCoprime(const Vector_2i& v);
std::string to_string(const Vector_2i& v);

//=============================================================================
// Interface: Mixed Point_2i/Vector_2i arithmetic
//=============================================================================

Point_2i operator+(const Point_2i& lhs, const Vector_2i& rhs);
Vector_2i operator-(const Point_2i& lhs, const Point_2i& rhs);
Point_2i operator-(const Point_2i& lhs, const Vector_2i& rhs);
Point_2i& operator+=(Point_2i& lhs, const Vector_2i& rhs);
Point_2i& operator-=(Point_2i& lhs, const Vector_2i& rhs);
integer Dot(const Vector_2i& lhs, const Point_2i& rhs);

//=============================================================================
// Interface: Vector_2f
//=============================================================================

class Vector_2f {
public:
    Vector_2f();
    Vector_2f(const Vector_2f& v);
    Vector_2f(const float x, const float y);

    Vector_2f& operator=(const Vector_2f& rhs);
    const float operator[](const size_t i) const;

    const float x() const;
    const float y() const;
    const std::array<float, 2>& elements() const;
    void set_x(const float x);
    void set_y(const float y);
    void set_elements(const std::array<float, 2>& elements);

private:
    std::array<float, 2> elements_;
};

bool operator==(const Vector_2f& lhs, const Vector_2f& rhs);
bool operator!=(const Vector_2f& lhs, const Vector_2f& rhs);
Vector_2f operator+(const Vector_2f& lhs, const Vector_2f& rhs);
Vector_2f operator-(const Vector_2f& lhs, const Vector_2f& rhs);
Vector_2f operator-(const Vector_2f& v);
Vector_2f operator*(const Vector_2f& v, const float r);
Vector_2f operator/(const Vector_2f& v, const float r);
Vector_2f& operator+=(Vector_2f& lhs, const Vector_2f& rhs);
Vector_2f& operator-=(Vector_2f& lhs, const Vector_2f& rhs);
Vector_2f& operator*=(Vector_2f& v, const float r);
Vector_2f& operator/=(Vector_2f& v, const float r);
std::ostream& operator<<(std::ostream& o, const Vector_2f& v);
float Dot(const Vector_2f &lhs, const Vector_2f &rhs);
std::string to_string(const Vector_2f& v);

//=============================================================================
// Interface: Mixed Point_2f/Vector_2f arithmetic
//=============================================================================

Point_2f operator+(const Point_2f& lhs, const Vector_2f& rhs);
Vector_2f operator-(const Point_2f& lhs, const Point_2f& rhs);
Point_2f operator-(const Point_2f& lhs, const Vector_2f& rhs);
Point_2f& operator+=(Point_2f& lhs, const Vector_2f& rhs);
Point_2f& operator-=(Point_2f& lhs, const Vector_2f& rhs);
float Dot(const Vector_2f& lhs, const Point_2f& rhs);

//=============================================================================
// Interface: Vector_2r
//=============================================================================

class Vector_2r {
public:
    Vector_2r();
    Vector_2r(const Vector_2r& v);
    Vector_2r(const Vector_2i& v);
    Vector_2r(const Vector_2f& v);
    Vector_2r(const rational& x, const rational& y);

    Vector_2r& operator=(const Vector_2r& rhs);
    const rational& operator[](const size_t i) const;

    const rational& x() const;
    const rational& y() const;
    const std::array<rational, 2>& elements() const;
    void set_x(const rational& x);
    void set_y(const rational& y);
    void set_elements(const std::array<rational, 2>& elements);

private:
    std::array<rational, 2> elements_;
};

bool operator==(const Vector_2r& lhs, const Vector_2r& rhs);
bool operator!=(const Vector_2r& lhs, const Vector_2r& rhs);
Vector_2r operator+(const Vector_2r& lhs, const Vector_2r& rhs);
Vector_2r operator-(const Vector_2r& lhs, const Vector_2r& rhs);
Vector_2r operator-(const Vector_2r& v);
Vector_2r operator*(const Vector_2r& v, const rational& r);
Vector_2r operator/(const Vector_2r& v, const rational& r);
Vector_2r& operator+=(Vector_2r& lhs, const Vector_2r& rhs);
Vector_2r& operator-=(Vector_2r& lhs, const Vector_2r& rhs);
Vector_2r& operator*=(Vector_2r& v, const rational& r);
Vector_2r& operator/=(Vector_2r& v, const rational& r);
std::ostream& operator<<(std::ostream& o, const Vector_2r& v);
rational Dot(const Vector_2r &lhs, const Vector_2r &rhs);
std::string to_string(const Vector_2r& v);

//=============================================================================
// Interface: Mixed Point_2r/Vector_2r arithmetic
//=============================================================================

Point_2r operator+(const Point_2r& lhs, const Vector_2r& rhs);
Vector_2r operator-(const Point_2r& lhs, const Point_2r& rhs);
Point_2r operator-(const Point_2r& lhs, const Vector_2r& rhs);
Point_2r& operator+=(Point_2r& lhs, const Vector_2r& rhs);
Point_2r& operator-=(Point_2r& lhs, const Vector_2r& rhs);
rational Dot(const Vector_2r& lhs, const Point_2r& rhs);

//=============================================================================
// Implementation: Vector_2i
//=============================================================================

inline Vector_2i::Vector_2i() {
    set_x(0);
    set_y(0);
}

inline Vector_2i::Vector_2i(const Vector_2i& v) {
    set_x(v.x());
    set_y(v.y());
}

inline Vector_2i::Vector_2i(const integer& x, const integer& y) {
    set_x(x);
    set_y(y);
}

inline Vector_2i& Vector_2i::operator=(const Vector_2i& rhs) {
    set_x(rhs.x());
    set_y(rhs.y());
    return *this;
}

//! @brief Index-based access operator without elegant out-of-bounds handling.
inline const integer& Vector_2i::operator[](const size_t i) const {
    assert(i < 2);
    return elements_[i];
}

//! @brief Sequentially calls operator== element-wise.
inline bool operator==(const Vector_2i& lhs, const Vector_2i& rhs) {
    return lhs.elements() == rhs.elements();
}

//! @brief Sequentially calls operator!= element-wise.
inline bool operator!=(const Vector_2i& lhs, const Vector_2i& rhs) {
    return lhs.elements() != rhs.elements();
}

inline Vector_2i operator+(const Vector_2i& lhs, const Vector_2i& rhs) {
    return Vector_2i(lhs.x()+rhs.x(), lhs.y()+rhs.y());
}

inline Vector_2i operator-(const Vector_2i& lhs, const Vector_2i& rhs) {
    return Vector_2i(lhs.x()-rhs.x(), lhs.y()-rhs.y());
}

inline Vector_2i operator-(const Vector_2i& v) {
    return Vector_2i(-v.x(), -v.y());
}

inline Vector_2i operator*(const Vector_2i& v, const integer& i) {
    return Vector_2i(v.x()*i, v.y()*i);
}

inline Vector_2i operator*(const integer& i, const Vector_2i& v) {
    return Vector_2i(v.x()*i, v.y()*i);
}

inline Vector_2i& operator+=(Vector_2i& lhs, const Vector_2i& rhs) {
    return lhs = lhs+rhs;
}

inline Vector_2i& operator-=(Vector_2i& lhs, const Vector_2i& rhs) {
    return lhs = lhs-rhs;
}

inline Vector_2i& operator*=(Vector_2i& v, const integer& i) {
    return v = v*i;
}

inline std::ostream& operator<<(std::ostream& o, const Vector_2i& v) {
    return o << to_string(v);
}

//! @brief Standard inner product of two vectors.
inline integer Dot(const Vector_2i &lhs, const Vector_2i &rhs) {
    return lhs.x()*rhs.x()+lhs.y()*rhs.y();
}

inline Vector_2i RemoveFactor(const Vector_2i& v, const integer& f) {
    return Vector_2i(v.x()/f, v.y()/f);
}

inline integer GCD(const Vector_2i& v) {
    integer gcd;
    mpz_gcd(gcd.get_mpz_t(), v.x().get_mpz_t(), v.y().get_mpz_t());
    return gcd;
}

//! \todo Implement and move to cpp
inline bool IsCoprime(const Vector_2i& v) {
    return false;
}

//! @brief Represents this vector using angle brackets <x, y>.
inline std::string to_string(const Vector_2i& v) {
    std::stringstream ss;
    ss << "<" << v.x() << ", " << v.y() << ">";
    return ss.str();
}

// Accessors/Mutators =========================================================

inline const integer& Vector_2i::x() const {
    return elements_[0];
}
inline const integer& Vector_2i::y() const {
    return elements_[1];
}
inline const std::array<integer, 2>& Vector_2i::elements() const {
    return elements_;
}
inline void Vector_2i::set_x(const integer& x) {
    elements_[0] = x;
}
inline void Vector_2i::set_y(const integer& y) {
    elements_[1] = y;
}
inline void Vector_2i::set_elements(const std::array<integer, 2>& elements) {
    elements_ = elements;
}

//=============================================================================
// Implementation: Mixed Point_2i/Vector_2i arithmetic
//=============================================================================

//! @brief Point + Vector = Point
inline Point_2i operator+(const Point_2i& lhs, const Vector_2i& rhs) {
    return Point_2i(lhs.x()+rhs.x(), lhs.y()+rhs.y());
}

//! @brief Point - Vector = Point
inline Point_2i operator-(const Point_2i& lhs, const Vector_2i& rhs) {
    return Point_2i(lhs.x()-rhs.x(), lhs.y()-rhs.y());
}

//! @brief Point - Point = Vector
inline Vector_2i operator-(const Point_2i& lhs, const Point_2i& rhs) {
    return Vector_2i(lhs.x()-rhs.x(), lhs.y()-rhs.y());
}

inline Point_2i& operator+=(Point_2i& lhs, const Vector_2i& rhs) {
    return lhs = lhs+rhs;
}

inline Point_2i& operator-=(Point_2i& lhs, const Vector_2i& rhs) {
    return lhs = lhs-rhs;
}

//! @brief Projection of a point onto a vector.
inline integer Dot(const Vector_2i& lhs, const Point_2i& rhs) {
    return lhs.x()*rhs.x()+lhs.y()*rhs.y();
}

//=============================================================================
// Implementation: Vector_2f
//=============================================================================

inline Vector_2f::Vector_2f() {
    set_x(0);
    set_y(0);
}

inline Vector_2f::Vector_2f(const Vector_2f& v) {
    set_x(v.x());
    set_y(v.y());
}

inline Vector_2f::Vector_2f(const float x, const float y) {
    set_x(x);
    set_y(y);
}

inline Vector_2f& Vector_2f::operator=(const Vector_2f& rhs) {
    set_x(rhs.x());
    set_y(rhs.y());
    return *this;
}

//! @brief Index-based access operator without elegant out-of-bounds handling.
inline const float Vector_2f::operator[](const size_t i) const {
    assert(i < 2);
    return elements_[i];
}

//! @brief Sequentially calls operator== element-wise.
inline bool operator==(const Vector_2f& lhs, const Vector_2f& rhs) {
    return lhs.elements() == rhs.elements();
}

//! @brief Sequentially calls operator!= element-wise.
inline bool operator!=(const Vector_2f& lhs, const Vector_2f& rhs) {
    return lhs.elements() != rhs.elements();
}

inline Vector_2f operator+(const Vector_2f& lhs, const Vector_2f& rhs) {
    return Vector_2f(lhs.x()+rhs.x(), lhs.y()+rhs.y());
}

inline Vector_2f operator-(const Vector_2f& lhs, const Vector_2f& rhs) {
    return Vector_2f(lhs.x()-rhs.x(), lhs.y()-rhs.y());
}

inline Vector_2f operator-(const Vector_2f& v) {
    return Vector_2f(-v.x(), -v.y());
}

inline Vector_2f operator*(const Vector_2f& v, const float r) {
    return Vector_2f(v.x()*r, v.y()*r);
}

inline Vector_2f operator/(const Vector_2f& v, const float r) {
    return Vector_2f(v.x()/r, v.y()/r);
}

inline Vector_2f& operator+=(Vector_2f& lhs, const Vector_2f& rhs) {
    return lhs = lhs+rhs;
}

inline Vector_2f& operator-=(Vector_2f& lhs, const Vector_2f& rhs) {
    return lhs = lhs-rhs;
}

inline Vector_2f& operator*=(Vector_2f& v, const float r) {
    return v = v*r;
}

inline Vector_2f& operator/=(Vector_2f& v, const float r) {
    return v = v/r;
}

inline std::ostream& operator<<(std::ostream& o, const Vector_2f& v) {
    return o << to_string(v);
}

//! @brief Standard inner product of two vectors.
inline float Dot(const Vector_2f &lhs, const Vector_2f &rhs) {
    return lhs.x()*rhs.x()+lhs.y()*rhs.y();
}

//! @brief Represents this vector using angle brackets <x, y>.
inline std::string to_string(const Vector_2f& v) {
    std::stringstream ss;
    ss << "<" << v.x() << ", " << v.y() << ">";
    return ss.str();
}

// Accessors/Mutators =========================================================

inline const float Vector_2f::x() const {
    return elements_[0];
}
inline const float Vector_2f::y() const {
    return elements_[1];
}
inline const std::array<float, 2>& Vector_2f::elements() const {
    return elements_;
}
inline void Vector_2f::set_x(const float x) {
    elements_[0] = x;
}
inline void Vector_2f::set_y(const float y) {
    elements_[1] = y;
}
inline void Vector_2f::set_elements(const std::array<float, 2>& elements) {
    elements_ = elements;
}

//=============================================================================
// Implementation: Mixed Point_2f/Vector_2f arithmetic
//=============================================================================

//! @brief Point + Vector = Point
inline Point_2f operator+(const Point_2f& lhs, const Vector_2f& rhs) {
    return Point_2f(lhs.x()+rhs.x(), lhs.y()+rhs.y());
}

//! @brief Point - Vector = Point
inline Point_2f operator-(const Point_2f& lhs, const Vector_2f& rhs) {
    return Point_2f(lhs.x()-rhs.x(), lhs.y()-rhs.y());
}

//! @brief Point - Point = Vector
inline Vector_2f operator-(const Point_2f& lhs, const Point_2f& rhs) {
    return Vector_2f(lhs.x()-rhs.x(), lhs.y()-rhs.y());
}

inline Point_2f& operator+=(Point_2f& lhs, const Vector_2f& rhs) {
    return lhs = lhs+rhs;
}

inline Point_2f& operator-=(Point_2f& lhs, const Vector_2f& rhs) {
    return lhs = lhs-rhs;
}

//! @brief Projection of a point onto a vector.
inline float Dot(const Vector_2f& lhs, const Point_2f& rhs) {
    return lhs.x()*rhs.x()+lhs.y()*rhs.y();
}

//=============================================================================
// Implementation: Vector_2r
//=============================================================================

inline Vector_2r::Vector_2r() {
    set_x(0);
    set_y(0);
}

inline Vector_2r::Vector_2r(const Vector_2r& v) {
    set_x(v.x());
    set_y(v.y());
}

//! @brief Allow implicit conversion from integer vector.
inline Vector_2r::Vector_2r(const Vector_2i& v) {
    set_x(v.x());
    set_y(v.y());
}

//! @brief Allow implicit conversion from float vector.
inline Vector_2r::Vector_2r(const Vector_2f& v) {
    set_x(v.x());
    set_y(v.y());
}

inline Vector_2r::Vector_2r(const rational& x, const rational& y) {
    set_x(x);
    set_y(y);
}

inline Vector_2r& Vector_2r::operator=(const Vector_2r& rhs) {
    set_x(rhs.x());
    set_y(rhs.y());
    return *this;
}

//! @brief Index-based access operator without elegant out-of-bounds handling.
inline const rational& Vector_2r::operator[](const size_t i) const {
    assert(i < 2);
    return elements_[i];
}

//! @brief Sequentially calls operator== element-wise.
inline bool operator==(const Vector_2r& lhs, const Vector_2r& rhs) {
    return lhs.elements() == rhs.elements();
}

//! @brief Sequentially calls operator!= element-wise.
inline bool operator!=(const Vector_2r& lhs, const Vector_2r& rhs) {
    return lhs.elements() != rhs.elements();
}

inline Vector_2r operator+(const Vector_2r& lhs, const Vector_2r& rhs) {
    return Vector_2r(lhs.x()+rhs.x(), lhs.y()+rhs.y());
}

inline Vector_2r operator-(const Vector_2r& lhs, const Vector_2r& rhs) {
    return Vector_2r(lhs.x()-rhs.x(), lhs.y()-rhs.y());
}

inline Vector_2r operator-(const Vector_2r& v) {
    return Vector_2r(-v.x(), -v.y());
}

inline Vector_2r operator*(const Vector_2r& v, const rational& r) {
    return Vector_2r(v.x()*r, v.y()*r);
}

inline Vector_2r operator/(const Vector_2r& v, const rational& r) {
    return Vector_2r(v.x()/r, v.y()/r);
}

inline Vector_2r& operator+=(Vector_2r& lhs, const Vector_2r& rhs) {
    return lhs = lhs+rhs;
}

inline Vector_2r& operator-=(Vector_2r& lhs, const Vector_2r& rhs) {
    return lhs = lhs-rhs;
}

inline Vector_2r& operator*=(Vector_2r& v, const rational& r) {
    return v = v*r;
}

inline Vector_2r& operator/=(Vector_2r& v, const rational& r) {
    return v = v/r;
}

inline std::ostream& operator<<(std::ostream& o, const Vector_2r& v) {
    return o << to_string(v);
}

//! @brief Standard inner product of two vectors.
inline rational Dot(const Vector_2r &lhs, const Vector_2r &rhs) {
    return lhs.x()*rhs.x()+lhs.y()*rhs.y();
}

//! @brief Represents this vector using angle brackets <x, y>.
inline std::string to_string(const Vector_2r& v) {
    std::stringstream ss;
    ss << "<" << v.x() << ", " << v.y() << ">";
    return ss.str();
}

// Accessors/Mutators =========================================================

inline const rational& Vector_2r::x() const {
    return elements_[0];
}
inline const rational& Vector_2r::y() const {
    return elements_[1];
}
inline const std::array<rational, 2>& Vector_2r::elements() const {
    return elements_;
}
inline void Vector_2r::set_x(const rational& x) {
    elements_[0] = x;
}
inline void Vector_2r::set_y(const rational& y) {
    elements_[1] = y;
}
inline void Vector_2r::set_elements(const std::array<rational, 2>& elements) {
    elements_ = elements;
}

//=============================================================================
// Implementation: Mixed Point_2r/Vector_2r arithmetic
//=============================================================================

//! @brief Point + Vector = Point
inline Point_2r operator+(const Point_2r& lhs, const Vector_2r& rhs) {
    return Point_2r(lhs.x()+rhs.x(), lhs.y()+rhs.y());
}

//! @brief Point - Vector = Point
inline Point_2r operator-(const Point_2r& lhs, const Vector_2r& rhs) {
    return Point_2r(lhs.x()-rhs.x(), lhs.y()-rhs.y());
}

//! @brief Point - Point = Vector
inline Vector_2r operator-(const Point_2r& lhs, const Point_2r& rhs) {
    return Vector_2r(lhs.x()-rhs.x(), lhs.y()-rhs.y());
}

inline Point_2r& operator+=(Point_2r& lhs, const Vector_2r& rhs) {
    return lhs = lhs+rhs;
}

inline Point_2r& operator-=(Point_2r& lhs, const Vector_2r& rhs) {
    return lhs = lhs-rhs;
}

//! @brief Projection of a point onto a vector.
inline rational Dot(const Vector_2r& lhs, const Point_2r& rhs) {
    return lhs.x()*rhs.x()+lhs.y()*rhs.y();
}

//=============================================================================
// Interface: Vector_3i
//=============================================================================

class Vector_3i {
public:
    Vector_3i();
    Vector_3i(const Vector_3i& v);
    Vector_3i(const integer& x, const integer& y, const integer& z);

    Vector_3i& operator=(const Vector_3i& rhs);
    const integer& operator[](const size_t i) const;

    const integer& x() const;
    const integer& y() const;
    const integer& z() const;
    const std::array<integer, 3>& elements() const;
    void set_x(const integer& x);
    void set_y(const integer& y);
    void set_z(const integer& z);
    void set_elements(const std::array<integer, 3>& elements);

private:
    std::array<integer, 3> elements_;
};

bool operator==(const Vector_3i& lhs, const Vector_3i& rhs);
bool operator!=(const Vector_3i& lhs, const Vector_3i& rhs);
Vector_3i operator+(const Vector_3i& lhs, const Vector_3i& rhs);
Vector_3i operator-(const Vector_3i& lhs, const Vector_3i& rhs);
Vector_3i operator-(const Vector_3i& v);
Vector_3i operator*(const Vector_3i& v, const integer& i);
Vector_3i operator*(const integer& i, const Vector_3i& v);
Vector_3i& operator+=(Vector_3i& lhs, const Vector_3i& rhs);
Vector_3i& operator-=(Vector_3i& lhs, const Vector_3i& rhs);
Vector_3i& operator*=(Vector_3i& v, const integer& i);
std::ostream& operator<<(std::ostream& o, const Vector_3i& v);
integer Dot(const Vector_3i& lhs, const Vector_3i& rhs);
Vector_3i Cross(const Vector_3i& lhs, const Vector_3i& rhs);
Vector_3i RemoveFactor(const Vector_3i& v, const integer& f);
integer GCD(const Vector_3i& v);
bool IsCoprime(const Vector_3i& v);
std::string to_string(const Vector_3i& v);

//=============================================================================
// Interface: Mixed Point_3i/Vector_3i arithmetic
//=============================================================================

Point_3i operator+(const Point_3i& lhs, const Vector_3i& rhs);
Vector_3i operator-(const Point_3i& lhs, const Point_3i& rhs);
Point_3i operator-(const Point_3i& lhs, const Vector_3i& rhs);
Point_3i& operator+=(Point_3i& lhs, const Vector_3i& rhs);
Point_3i& operator-=(Point_3i& lhs, const Vector_3i& rhs);
integer Dot(const Vector_3i& lhs, const Point_3i& rhs);

//=============================================================================
// Interface: Vector_3f
//=============================================================================

class Vector_3f {
public:
    Vector_3f();
    Vector_3f(const Vector_3f& v);
    Vector_3f(const float x, const float y, const float z);

    Vector_3f& operator=(const Vector_3f& rhs);
    const float operator[](const size_t i) const;

    const float x() const;
    const float y() const;
    const float z() const;
    const std::array<float, 3>& elements() const;
    void set_x(const float x);
    void set_y(const float y);
    void set_z(const float z);
    void set_elements(const std::array<float, 3>& elements);

private:
    std::array<float, 3> elements_;
};

bool operator==(const Vector_3f& lhs, const Vector_3f& rhs);
bool operator!=(const Vector_3f& lhs, const Vector_3f& rhs);
Vector_3f operator+(const Vector_3f& lhs, const Vector_3f& rhs);
Vector_3f operator-(const Vector_3f& lhs, const Vector_3f& rhs);
Vector_3f operator-(const Vector_3f& v);
Vector_3f operator*(const Vector_3f& v, const float r);
Vector_3f operator/(const Vector_3f& v, const float r);
Vector_3f& operator+=(Vector_3f& lhs, const Vector_3f& rhs);
Vector_3f& operator-=(Vector_3f& lhs, const Vector_3f& rhs);
Vector_3f& operator*=(Vector_3f& v, const float r);
Vector_3f& operator/=(Vector_3f& v, const float r);
std::ostream& operator<<(std::ostream& o, const Vector_3f& v);
float Dot(const Vector_3f &lhs, const Vector_3f &rhs);
float Length(const Vector_3f& v);
float LengthSqr(const Vector_3f& v);
Vector_3f Normalized(const Vector_3f& v);
Vector_3f Cross(const Vector_3f& lhs, const Vector_3f& rhs);
std::string to_string(const Vector_3f& v);

//=============================================================================
// Interface: Mixed Point_3f/Vector_3f arithmetic
//=============================================================================

Point_3f operator+(const Point_3f& lhs, const Vector_3f& rhs);
Vector_3f operator-(const Point_3f& lhs, const Point_3f& rhs);
Point_3f operator-(const Point_3f& lhs, const Vector_3f& rhs);
Point_3f& operator+=(Point_3f& lhs, const Vector_3f& rhs);
Point_3f& operator-=(Point_3f& lhs, const Vector_3f& rhs);
float Dot(const Vector_3f& lhs, const Point_3f& rhs);

//=============================================================================
// Interface: Vector_3r
//=============================================================================

class Vector_3r {
public:
    Vector_3r();
    Vector_3r(const Vector_3r& v);
    Vector_3r(const Vector_3i& v);
    Vector_3r(const Vector_3f& v);
    Vector_3r(const rational& x, const rational& y, const rational& z);

    Vector_3r& operator=(const Vector_3r& rhs);
    const rational& operator[](const size_t i) const;

    const rational& x() const;
    const rational& y() const;
    const rational& z() const;
    const std::array<rational, 3>& elements() const;
    void set_x(const rational& x);
    void set_y(const rational& y);
    void set_z(const rational& z);
    void set_elements(const std::array<rational, 3>& elements);

private:
    std::array<rational, 3> elements_;
};

bool operator==(const Vector_3r& lhs, const Vector_3r& rhs);
bool operator!=(const Vector_3r& lhs, const Vector_3r& rhs);
Vector_3r operator+(const Vector_3r& lhs, const Vector_3r& rhs);
Vector_3r operator-(const Vector_3r& lhs, const Vector_3r& rhs);
Vector_3r operator-(const Vector_3r& v);
Vector_3r operator*(const Vector_3r& v, const rational& r);
Vector_3r operator/(const Vector_3r& v, const rational& r);
Vector_3r& operator+=(Vector_3r& lhs, const Vector_3r& rhs);
Vector_3r& operator-=(Vector_3r& lhs, const Vector_3r& rhs);
Vector_3r& operator*=(Vector_3r& v, const rational& r);
Vector_3r& operator/=(Vector_3r& v, const rational& r);
std::ostream& operator<<(std::ostream& o, const Vector_3r& v);
rational Dot(const Vector_3r& lhs, const Vector_3r& rhs);
Vector_3r Cross(const Vector_3r& lhs, const Vector_3r& rhs);
std::string to_string(const Vector_3r& v);

//=============================================================================
// Interface: Mixed Point_3r/Vector_3r arithmetic
//=============================================================================

Point_3r operator+(const Point_3r& lhs, const Vector_3r& rhs);
Vector_3r operator-(const Point_3r& lhs, const Point_3r& rhs);
Point_3r operator-(const Point_3r& lhs, const Vector_3r& rhs);
Point_3r& operator+=(Point_3r& lhs, const Vector_3r& rhs);
Point_3r& operator-=(Point_3r& lhs, const Vector_3r& rhs);
rational Dot(const Vector_3r& lhs, const Point_3r& rhs);

//=============================================================================
// Implementation: Vector_3i
//=============================================================================

inline Vector_3i::Vector_3i() {
    set_x(0);
    set_y(0);
    set_z(0);
}

inline Vector_3i::Vector_3i(const Vector_3i& v) {
    set_x(v.x());
    set_y(v.y());
    set_z(v.z());
}

inline Vector_3i::Vector_3i(const integer& x, const integer& y,
                            const integer& z) {
    set_x(x);
    set_y(y);
    set_z(z);
}

inline Vector_3i& Vector_3i::operator=(const Vector_3i& rhs) {
    set_x(rhs.x());
    set_y(rhs.y());
    set_z(rhs.z());
    return *this;
}

//! @brief Index-based access operator without elegant out-of-bounds handling.
inline const integer& Vector_3i::operator[](const size_t i) const {
    assert(i < 3);
    return elements_[i];
}

//! @brief Sequentially calls operator== element-wise.
inline bool operator==(const Vector_3i& lhs, const Vector_3i& rhs) {
    return lhs.elements() == rhs.elements();
}

//! @brief Sequentially calls operator!= element-wise.
inline bool operator!=(const Vector_3i& lhs, const Vector_3i& rhs) {
    return lhs.elements() != rhs.elements();
}

inline Vector_3i operator+(const Vector_3i& lhs, const Vector_3i& rhs) {
    return Vector_3i(lhs.x()+rhs.x(), lhs.y()+rhs.y(), lhs.z()+rhs.z());
}

inline Vector_3i operator-(const Vector_3i& lhs, const Vector_3i& rhs) {
    return Vector_3i(lhs.x()-rhs.x(), lhs.y()-rhs.y(), lhs.z()-rhs.z());
}

inline Vector_3i operator-(const Vector_3i& v) {
    return Vector_3i(-v.x(), -v.y(), -v.z());
}

inline Vector_3i operator*(const Vector_3i& v, const integer& i) {
    return Vector_3i(v.x()*i, v.y()*i, v.z()*i);
}

inline Vector_3i operator*(const integer& i, const Vector_3i& v) {
    return Vector_3i(v.x()*i, v.y()*i, v.z()*i);
}

inline Vector_3i& operator+=(Vector_3i& lhs, const Vector_3i& rhs) {
    return lhs = lhs+rhs;
}

inline Vector_3i& operator-=(Vector_3i& lhs, const Vector_3i& rhs) {
    return lhs = lhs-rhs;
}

inline Vector_3i& operator*=(Vector_3i& v, const integer& i) {
    return v = v*i;
}

inline std::ostream& operator<<(std::ostream& o, const Vector_3i& v) {
    return o << to_string(v);
}

//! @brief Standard inner product of two vectors.
inline integer Dot(const Vector_3i &lhs, const Vector_3i &rhs) {
    return lhs.x()*rhs.x()+lhs.y()*rhs.y()+lhs.z()*rhs.z();
}

inline Vector_3i Cross(const Vector_3i& lhs, const Vector_3i& rhs) {
    return Vector_3i(lhs.y()*rhs.z()-lhs.z()*rhs.y(),
                     lhs.z()*rhs.x()-lhs.x()*rhs.z(),
                     lhs.x()*rhs.y()-lhs.y()*rhs.x());
}

inline Vector_3i RemoveFactor(const Vector_3i& v, const integer& f) {
    return Vector_3i(v.x()/f, v.y()/f, v.z()/f);
}

inline integer GCD(const Vector_3i& v) {
    integer gcdXY, gcdXYZ;
    mpz_gcd(gcdXY.get_mpz_t(), v.x().get_mpz_t(), v.y().get_mpz_t());
    mpz_gcd(gcdXYZ.get_mpz_t(), gcdXY.get_mpz_t(), v.z().get_mpz_t());
    return gcdXYZ;
}

//! \todo Implement and move to cpp
inline bool IsCoprime(const Vector_3i& v) {
    return false;
}

//! @brief Represents this vector using angle brackets <x, y, z>.
inline std::string to_string(const Vector_3i& v) {
    std::stringstream ss;
    ss << "<" << v.x() << ", " << v.y() << ", " << v.z() << ">";
    return ss.str();
}

// Accessors/Mutators =========================================================

inline const integer& Vector_3i::x() const {
    return elements_[0];
}
inline const integer& Vector_3i::y() const {
    return elements_[1];
}
inline const integer& Vector_3i::z() const {
    return elements_[2];
}
inline const std::array<integer, 3>& Vector_3i::elements() const {
    return elements_;
}
inline void Vector_3i::set_x(const integer& x) {
    elements_[0] = x;
}
inline void Vector_3i::set_y(const integer& y) {
    elements_[1] = y;
}
inline void Vector_3i::set_z(const integer& z) {
    elements_[2] = z;
}
inline void Vector_3i::set_elements(const std::array<integer, 3>& elements) {
    elements_ = elements;
}

//=============================================================================
// Implementation: Mixed Point_3i/Vector_3i arithmetic
//=============================================================================

//! @brief Point + Vector = Point
inline Point_3i operator+(const Point_3i& lhs, const Vector_3i& rhs) {
    return Point_3i(lhs.x()+rhs.x(), lhs.y()+rhs.y(), lhs.z()+rhs.z());
}

//! @brief Point - Vector = Point
inline Point_3i operator-(const Point_3i& lhs, const Vector_3i& rhs) {
    return Point_3i(lhs.x()-rhs.x(), lhs.y()-rhs.y(), lhs.z()-rhs.z());
}

//! @brief Point - Point = Vector
inline Vector_3i operator-(const Point_3i& lhs, const Point_3i& rhs) {
    return Vector_3i(lhs.x()-rhs.x(), lhs.y()-rhs.y(), lhs.z()-rhs.z());
}

inline Point_3i& operator+=(Point_3i& lhs, const Vector_3i& rhs) {
    return lhs = lhs+rhs;
}

inline Point_3i& operator-=(Point_3i& lhs, const Vector_3i& rhs) {
    return lhs = lhs-rhs;
}

//! @brief Projection of a point onto a vector.
inline integer Dot(const Vector_3i& lhs, const Point_3i& rhs) {
    return lhs.x()*rhs.x()+lhs.y()*rhs.y()+lhs.z()*rhs.z();
}

//=============================================================================
// Implementation: Vector_3f
//=============================================================================

inline Vector_3f::Vector_3f() {
    set_x(0);
    set_y(0);
    set_z(0);
}

inline Vector_3f::Vector_3f(const Vector_3f& v) {
    set_x(v.x());
    set_y(v.y());
    set_z(v.z());
}

inline Vector_3f::Vector_3f(const float x, const float y, const float z) {
    set_x(x);
    set_y(y);
    set_z(z);
}

inline Vector_3f& Vector_3f::operator=(const Vector_3f& rhs) {
    set_x(rhs.x());
    set_y(rhs.y());
    set_z(rhs.z());
    return *this;
}

//! @brief Index-based access operator without elegant out-of-bounds handling.
inline const float Vector_3f::operator[](const size_t i) const {
    assert(i < 3);
    return elements_[i];
}

//! @brief Sequentially calls operator== element-wise.
inline bool operator==(const Vector_3f& lhs, const Vector_3f& rhs) {
    return lhs.elements() == rhs.elements();
}

//! @brief Sequentially calls operator!= element-wise.
inline bool operator!=(const Vector_3f& lhs, const Vector_3f& rhs) {
    return lhs.elements() != rhs.elements();
}

inline Vector_3f operator+(const Vector_3f& lhs, const Vector_3f& rhs) {
    return Vector_3f(lhs.x()+rhs.x(), lhs.y()+rhs.y(), lhs.z()+rhs.z());
}

inline Vector_3f operator-(const Vector_3f& lhs, const Vector_3f& rhs) {
    return Vector_3f(lhs.x()-rhs.x(), lhs.y()-rhs.y(), lhs.z()-rhs.z());
}

inline Vector_3f operator-(const Vector_3f& v) {
    return Vector_3f(-v.x(), -v.y(), -v.z());
}

inline Vector_3f operator*(const Vector_3f& v, const float f) {
    return Vector_3f(v.x()*f, v.y()*f, v.z()*f);
}

inline Vector_3f operator/(const Vector_3f& v, const float f) {
    return Vector_3f(v.x()/f, v.y()/f, v.z()/f);
}

inline Vector_3f& operator+=(Vector_3f& lhs, const Vector_3f& rhs) {
    return lhs = lhs+rhs;
}

inline Vector_3f& operator-=(Vector_3f& lhs, const Vector_3f& rhs) {
    return lhs = lhs-rhs;
}

inline Vector_3f& operator*=(Vector_3f& v, const float f) {
    return v = v*f;
}

inline Vector_3f& operator/=(Vector_3f& v, const float f) {
    return v = v/f;
}

inline std::ostream& operator<<(std::ostream& o, const Vector_3f& v) {
    return o << to_string(v);
}

//! @brief Standard inner product of two vectors.
inline float Dot(const Vector_3f &lhs, const Vector_3f &rhs) {
    return lhs.x()*rhs.x()+lhs.y()*rhs.y()+lhs.z()*rhs.z();
}

inline float Length(const Vector_3f &v) {
    return sqrt(LengthSqr(v));
}

inline float LengthSqr(const Vector_3f &v) {
    return Dot(v, v);
}

inline Vector_3f Normalized(const Vector_3f &v) {
    return v/Length(v);
}

inline Vector_3f Cross(const Vector_3f& lhs, const Vector_3f& rhs) {
    return Vector_3f(lhs.y()*rhs.z()-lhs.z()*rhs.y(),
                     lhs.z()*rhs.x()-lhs.x()*rhs.z(),
                     lhs.x()*rhs.y()-lhs.y()*rhs.x());
}

//! @brief Represents this vector using angle brackets <x, y, z>.
inline std::string to_string(const Vector_3f& v) {
    std::stringstream ss;
    ss << "<" << v.x() << ", " << v.y() << ", " << v.z() << ">";
    return ss.str();
}

// Accessors/Mutators =========================================================

inline const float Vector_3f::x() const {
    return elements_[0];
}
inline const float Vector_3f::y() const {
    return elements_[1];
}
inline const float Vector_3f::z() const {
    return elements_[2];
}
inline const std::array<float, 3>& Vector_3f::elements() const {
    return elements_;
}
inline void Vector_3f::set_x(const float x) {
    elements_[0] = x;
}
inline void Vector_3f::set_y(const float y) {
    elements_[1] = y;
}
inline void Vector_3f::set_z(const float z) {
    elements_[2] = z;
}
inline void Vector_3f::set_elements(const std::array<float, 3>& elements) {
    elements_ = elements;
}

//=============================================================================
// Implementation: Mixed Point_3f/Vector_3f arithmetic
//=============================================================================

//! @brief Point + Vector = Point
inline Point_3f operator+(const Point_3f& lhs, const Vector_3f& rhs) {
    return Point_3f(lhs.x()+rhs.x(), lhs.y()+rhs.y(), lhs.z()+rhs.z());
}

//! @brief Point - Vector = Point
inline Point_3f operator-(const Point_3f& lhs, const Vector_3f& rhs) {
    return Point_3f(lhs.x()-rhs.x(), lhs.y()-rhs.y(), lhs.z()-rhs.z());
}

//! @brief Point - Point = Vector
inline Vector_3f operator-(const Point_3f& lhs, const Point_3f& rhs) {
    return Vector_3f(lhs.x()-rhs.x(), lhs.y()-rhs.y(), lhs.z()-rhs.z());
}

inline Point_3f& operator+=(Point_3f& lhs, const Vector_3f& rhs) {
    return lhs = lhs+rhs;
}

inline Point_3f& operator-=(Point_3f& lhs, const Vector_3f& rhs) {
    return lhs = lhs-rhs;
}

//! @brief Projection of a point onto a vector.
inline float Dot(const Vector_3f& lhs, const Point_3f& rhs) {
    return lhs.x()*rhs.x()+lhs.y()*rhs.y()+lhs.z()*rhs.z();
}

//=============================================================================
// Implementation: Vector_3r
//=============================================================================

inline Vector_3r::Vector_3r() {
    set_x(0);
    set_y(0);
    set_z(0);
}

inline Vector_3r::Vector_3r(const Vector_3r& v) {
    set_x(v.x());
    set_y(v.y());
    set_z(v.z());
}

//! @brief Allow implicit conversion from integer vector.
inline Vector_3r::Vector_3r(const Vector_3i& v) {
    set_x(v.x());
    set_y(v.y());
    set_z(v.z());
}

//! @brief Allow implicit conversion from float vector.
inline Vector_3r::Vector_3r(const Vector_3f& v) {
    set_x(v.x());
    set_y(v.y());
    set_z(v.z());
}

inline Vector_3r::Vector_3r(const rational& x, const rational& y,
                            const rational& z) {
    set_x(x);
    set_y(y);
    set_z(z);
}

inline Vector_3r& Vector_3r::operator=(const Vector_3r& rhs) {
    set_x(rhs.x());
    set_y(rhs.y());
    set_z(rhs.z());
    return *this;
}

//! @brief Index-based access operator without elegant out-of-bounds handling.
inline const rational& Vector_3r::operator[](const size_t i) const {
    assert(i < 3);
    return elements_[i];
}

//! @brief Sequentially calls operator== element-wise.
inline bool operator==(const Vector_3r& lhs, const Vector_3r& rhs) {
    return lhs.elements() == rhs.elements();
}

//! @brief Sequentially calls operator!= element-wise.
inline bool operator!=(const Vector_3r& lhs, const Vector_3r& rhs) {
    return lhs.elements() != rhs.elements();
}

inline Vector_3r operator+(const Vector_3r& lhs, const Vector_3r& rhs) {
    return Vector_3r(lhs.x()+rhs.x(), lhs.y()+rhs.y(), lhs.z()+rhs.z());
}

inline Vector_3r operator-(const Vector_3r& lhs, const Vector_3r& rhs) {
    return Vector_3r(lhs.x()-rhs.x(), lhs.y()-rhs.y(), lhs.z()-rhs.z());
}

inline Vector_3r operator-(const Vector_3r& v) {
    return Vector_3r(-v.x(), -v.y(), -v.z());
}

inline Vector_3r operator*(const Vector_3r& v, const rational& r) {
    return Vector_3r(v.x()*r, v.y()*r, v.z()*r);
}

inline Vector_3r operator/(const Vector_3r& v, const rational& r) {
    return Vector_3r(v.x()/r, v.y()/r, v.z()/r);
}

inline Vector_3r& operator+=(Vector_3r& lhs, const Vector_3r& rhs) {
    return lhs = lhs+rhs;
}

inline Vector_3r& operator-=(Vector_3r& lhs, const Vector_3r& rhs) {
    return lhs = lhs-rhs;
}

inline Vector_3r& operator*=(Vector_3r& v, const rational& r) {
    return v = v*r;
}

inline Vector_3r& operator/=(Vector_3r& v, const rational& r) {
    return v = v/r;
}

inline std::ostream& operator<<(std::ostream& o, const Vector_3r& v) {
    return o << to_string(v);
}

//! @brief Standard inner product of two vectors.
inline rational Dot(const Vector_3r &lhs, const Vector_3r &rhs) {
    return lhs.x()*rhs.x()+lhs.y()*rhs.y()+lhs.z()*rhs.z();
}

inline Vector_3r Cross(const Vector_3r& lhs, const Vector_3r& rhs) {
    return Vector_3r(lhs.y()*rhs.z()-lhs.z()*rhs.y(),
                     lhs.z()*rhs.x()-lhs.x()*rhs.z(),
                     lhs.x()*rhs.y()-lhs.y()*rhs.x());
}

//! @brief Represents this vector using angle brackets <x, y, z>.
inline std::string to_string(const Vector_3r& v) {
    std::stringstream ss;
    ss << "<" << v.x() << ", " << v.y() << ", " << v.z() << ">";
    return ss.str();
}

// Accessors/Mutators =========================================================

inline const rational& Vector_3r::x() const {
    return elements_[0];
}
inline const rational& Vector_3r::y() const {
    return elements_[1];
}
inline const rational& Vector_3r::z() const {
    return elements_[2];
}
inline const std::array<rational, 3>& Vector_3r::elements() const {
    return elements_;
}
inline void Vector_3r::set_x(const rational& x) {
    elements_[0] = x;
}
inline void Vector_3r::set_y(const rational& y) {
    elements_[1] = y;
}
inline void Vector_3r::set_z(const rational& z) {
    elements_[2] = z;
}
inline void Vector_3r::set_elements(const std::array<rational, 3>& elements) {
    elements_ = elements;
}

//=============================================================================
// Implementation: Mixed Point_3r/Vector_3r arithmetic
//=============================================================================

//! @brief Point + Vector = Point
inline Point_3r operator+(const Point_3r& lhs, const Vector_3r& rhs) {
    return Point_3r(lhs.x()+rhs.x(), lhs.y()+rhs.y(), lhs.z()+rhs.z());
}

//! @brief Point - Vector = Point
inline Point_3r operator-(const Point_3r& lhs, const Vector_3r& rhs) {
    return Point_3r(lhs.x()-rhs.x(), lhs.y()-rhs.y(), lhs.z()-rhs.z());
}

//! @brief Point - Point = Vector
inline Vector_3r operator-(const Point_3r& lhs, const Point_3r& rhs) {
    return Vector_3r(lhs.x()-rhs.x(), lhs.y()-rhs.y(), lhs.z()-rhs.z());
}

inline Point_3r& operator+=(Point_3r& lhs, const Vector_3r& rhs) {
    return lhs = lhs+rhs;
}

inline Point_3r& operator-=(Point_3r& lhs, const Vector_3r& rhs) {
    return lhs = lhs-rhs;
}

//! @brief Projection of a point onto a vector.
inline rational Dot(const Vector_3r& lhs, const Point_3r& rhs) {
    return lhs.x()*rhs.x()+lhs.y()*rhs.y()+lhs.z()*rhs.z();
}

} // namespace DDAD

#endif // GE_VECTOR_H
