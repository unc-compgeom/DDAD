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
 * @brief Integer, floating-point, and rational point types for 2D and 3D.
 */

#ifndef GE_POINT_H
#define GE_POINT_H

#include "common.h"
#include "arithmetic.h"

namespace DDAD {

class Point_2i;
class Point_2f;
class Point_2r;
class Point_3i;
class Point_3f;
class Point_3r;

typedef std::shared_ptr<Point_2i> SharedPoint_2i;
typedef std::shared_ptr<Point_2f> SharedPoint_2f;
typedef std::shared_ptr<Point_2r> SharedPoint_2r;
typedef std::shared_ptr<Point_3i> SharedPoint_3i;
typedef std::shared_ptr<Point_3f> SharedPoint_3f;
typedef std::shared_ptr<Point_3r> SharedPoint_3r;

struct IPointObserver {
    virtual void SlotPositionChanged_2r(const Point_2r& p) = 0;
    virtual void SlotPositionChanged_3r(const Point_3r& p) = 0;
};

//=============================================================================
// Interface: Point_2i
//=============================================================================

class Point_2i {
public:
    Point_2i();
    Point_2i(const Point_2i& p);
    Point_2i(const integer& x, const integer& y);

    Point_2i& operator=(const Point_2i& rhs);
    const integer& operator[](const size_t i) const;

    const integer& x() const;
    const integer& y() const;
    const std::array<integer, 2>& elements() const;
    const uint32_t unique_id() const;
    void set_x(const integer& x);
    void set_y(const integer& y);
    void set_elements(const std::array<integer, 2>& elements);
    void set_unique_id(const uint32_t unique_id);

private:
    std::array<integer, 2> elements_;
    uint32_t unique_id_;
};

bool operator==(const Point_2i& lhs, const Point_2i& rhs);
bool operator!=(const Point_2i& lhs, const Point_2i& rhs);
std::ostream& operator<<(std::ostream& o, const Point_2i& p);
std::string to_string(const Point_2i& p);

//=============================================================================
// Interface: Point_2f
//=============================================================================

class Point_2f {
public:
    Point_2f();
    Point_2f(const Point_2f& p);
    Point_2f(const float x, const float y);

    Point_2f& operator=(const Point_2f& rhs);
    const float operator[](const size_t i) const;

    const float x() const;
    const float y() const;
    const std::array<float, 2>& elements() const;
    const uint32_t unique_id() const;
    void set_x(const float x);
    void set_y(const float y);
    void set_elements(const std::array<float, 2>& elements);
    void set_unique_id(const uint32_t unique_id);

private:
    std::array<float, 2> elements_;
    uint32_t unique_id_;
};

bool operator==(const Point_2f& lhs, const Point_2f& rhs);
bool operator!=(const Point_2f& lhs, const Point_2f& rhs);
std::ostream& operator<<(std::ostream& o, const Point_2f& p);
std::string to_string(const Point_2f& v);

//=============================================================================
// Interface: Point_2r
//=============================================================================

class Point_2r {
public:
    Point_2r();
    Point_2r(const Point_2r& p);
    Point_2r(const Point_2i& p);
    Point_2r(const Point_2f& p);
    Point_2r(const rational& x, const rational& y);

    void AddObserver(IPointObserver* o);
    void RemoveObserver(IPointObserver* o);
    void SigPositionChanged() const;

    Point_2r& operator=(const Point_2r& rhs);
    const rational& operator[](const size_t i) const;

    const rational& x() const;
    const rational& y() const;
    const std::array<rational, 2>& elements() const;
    const uint32_t unique_id() const;
    void set_x(const rational& x);
    void set_y(const rational& y);
    void set_elements(const std::array<rational, 2>& elements);
    void set_unique_id(const uint32_t unique_id);

private:
    std::array<rational, 2> elements_;
    std::vector<IPointObserver*> observers_;
    uint32_t unique_id_;
};

bool operator==(const Point_2r& lhs, const Point_2r& rhs);
bool operator!=(const Point_2r& lhs, const Point_2r& rhs);
std::ostream& operator<<(std::ostream& o, const Point_2r& p);
std::string to_string(const Point_2r& p);

namespace Construction {
Point_2r Z2Neighbor(const Point_2r& p, Quadrant quad);
}

//=============================================================================
// Implementation: Point_2i
//=============================================================================

inline Point_2i::Point_2i() {
    set_x(0);
    set_y(0);
    set_unique_id(0);
}

inline Point_2i::Point_2i(const Point_2i& p) {
    *this = p;
}

inline Point_2i::Point_2i(const integer& x, const integer& y) {
    set_x(x);
    set_y(y);
    set_unique_id(0);
}

inline Point_2i& Point_2i::operator=(const Point_2i& rhs) {
    set_x(rhs.x());
    set_y(rhs.y());
    set_unique_id(rhs.unique_id());
    return *this;
}

//! @brief Index-based access operator without elegant out-of-bounds handling.
inline const integer& Point_2i::operator[](const size_t i) const {
    assert(i < 2);
    return elements_[i];
}

//! @brief Sequentially calls operator== element-wise.
inline bool operator==(const Point_2i& lhs, const Point_2i& rhs) {
    return lhs.elements() == rhs.elements();
}

//! @brief Sequentially calls operator!= element-wise.
inline bool operator!=(const Point_2i& lhs, const Point_2i& rhs) {
    return lhs.elements() != rhs.elements();
}

inline std::ostream& operator<<(std::ostream& o, const Point_2i& p) {
    return o << to_string(p);
}

//! @brief Represents this point using parentheses (x, y).
inline std::string to_string(const Point_2i& p) {
    std::stringstream ss;
    ss << "(" << p.x() << ", " << p.y() << ")";
    return ss.str();
}

// Accessors/Mutators =========================================================

inline const integer& Point_2i::x() const {
    return elements_[0];
}
inline const integer& Point_2i::y() const {
    return elements_[1];
}
inline const std::array<integer, 2>& Point_2i::elements() const {
    return elements_;
}
inline const uint32_t Point_2i::unique_id() const {
    return unique_id_;
}
inline void Point_2i::set_x(const integer& x) {
    elements_[0] = x;
}
inline void Point_2i::set_y(const integer& y) {
    elements_[1] = y;
}
inline void Point_2i::set_elements(const std::array<integer, 2>& elements) {
    elements_ = elements;
}
inline void Point_2i::set_unique_id(const uint32_t unique_id) {
    unique_id_ = unique_id;
}

//=============================================================================
// Implementation: Point_2f
//=============================================================================

inline Point_2f::Point_2f() {
    set_x(0);
    set_y(0);
    set_unique_id(0);
}

inline Point_2f::Point_2f(const Point_2f& p) {
    *this = p;
}

inline Point_2f::Point_2f(const float x, const float y) {
    set_x(x);
    set_y(y);
    set_unique_id(0);
}

inline Point_2f& Point_2f::operator=(const Point_2f& rhs) {
    set_x(rhs.x());
    set_y(rhs.y());
    set_unique_id(rhs.unique_id());
    return *this;
}

//! @brief Index-based access operator without elegant out-of-bounds handling.
inline const float Point_2f::operator[](const size_t i) const {
    assert(i < 2);
    return elements_[i];
}

//! @brief Sequentially calls operator== element-wise.
inline bool operator==(const Point_2f& lhs, const Point_2f& rhs) {
    return lhs.elements() == rhs.elements();
}

//! @brief Sequentially calls operator!= element-wise.
inline bool operator!=(const Point_2f& lhs, const Point_2f& rhs) {
    return lhs.elements() != rhs.elements();
}

inline std::ostream& operator<<(std::ostream& o, const Point_2f& p) {
    return o << to_string(p);
}

//! @brief Represents this point using parentheses (x, y).
inline std::string to_string(const Point_2f& p) {
    std::stringstream ss;
    ss << "(" << p.x() << ", " << p.y() << ")";
    return ss.str();
}

// Accessors/Mutators =========================================================

inline const float Point_2f::x() const {
    return elements_[0];
}
inline const float Point_2f::y() const {
    return elements_[1];
}
inline const std::array<float, 2>& Point_2f::elements() const {
    return elements_;
}
inline const uint32_t Point_2f::unique_id() const {
    return unique_id_;
}
inline void Point_2f::set_x(const float x) {
    elements_[0] = x;
}
inline void Point_2f::set_y(const float y) {
    elements_[1] = y;
}
inline void Point_2f::set_elements(const std::array<float, 2>& elements) {
    elements_ = elements;
}
inline void Point_2f::set_unique_id(const uint32_t unique_id) {
    unique_id_ = unique_id;
}

//=============================================================================
// Implementation: Point_2r
//=============================================================================

inline Point_2r::Point_2r() {
    set_x(0);
    set_y(0);
    set_unique_id(0);
}

inline Point_2r::Point_2r(const Point_2r& p) {
    *this = p;
}

//! @brief Allow implicit conversion from integer point.
inline Point_2r::Point_2r(const Point_2i& p) {
    set_x(p.x());
    set_y(p.y());
    set_unique_id(p.unique_id());
}

//! @brief Allow implicit conversion from float point.
inline Point_2r::Point_2r(const Point_2f& p) {
    set_x(p.x());
    set_y(p.y());
    set_unique_id(p.unique_id());
}

inline Point_2r::Point_2r(const rational& x, const rational& y) {
    set_x(x);
    set_y(y);
    set_unique_id(0);
}

inline void Point_2r::AddObserver(IPointObserver* o) {
    observers_.push_back(o);
}

inline void Point_2r::RemoveObserver(IPointObserver* o) {
    std::remove(begin(observers_), end(observers_), o);
}

inline void Point_2r::SigPositionChanged() const {
    for(auto i = begin(observers_); i != end(observers_); ++i) {
        (*i)->SlotPositionChanged_2r(*this);
    }
}

inline Point_2r& Point_2r::operator=(const Point_2r& rhs) {
    set_x(rhs.x());
    set_y(rhs.y());
    set_unique_id(rhs.unique_id());
    return *this;
}

//! @brief Index-based access operator without elegant out-of-bounds handling.
inline const rational& Point_2r::operator[](const size_t i) const {
    assert(i < 2);
    return elements_[i];
}

//! @brief Sequentially calls operator== element-wise.
inline bool operator==(const Point_2r& lhs, const Point_2r& rhs) {
    return lhs.elements() == rhs.elements();
}

//! @brief Sequentially calls operator!= element-wise.
inline bool operator!=(const Point_2r& lhs, const Point_2r& rhs) {
    return lhs.elements() != rhs.elements();
}

inline std::ostream& operator<<(std::ostream& o, const Point_2r& p) {
    return o << to_string(p);
}

//! @brief Represents this point using parentheses (x, y).
inline std::string to_string(const Point_2r& p) {
    std::stringstream ss;
    ss << "(" << p.x() << ", " << p.y() << ")";
    return ss.str();
}

// Constructions ==============================================================

inline Point_2r Construction::Z2Neighbor(const Point_2r& p, Quadrant quad) {
    switch(quad) {
    case QUADRANT_NORTHEAST: return Point_2r(Ceil(p.x()), Ceil(p.y()));
    case QUADRANT_NORTHWEST: return Point_2r(Floor(p.x()), Ceil(p.y()));
    case QUADRANT_SOUTHEAST: return Point_2r(Ceil(p.x()), Floor(p.y()));
    case QUADRANT_SOUTHWEST: return Point_2r(Floor(p.x()), Floor(p.y()));
    default: return p;
    }
}

// Accessors/Mutators =========================================================

inline const rational& Point_2r::x() const {
    return elements_[0];
}
inline const rational& Point_2r::y() const {
    return elements_[1];
}
inline const std::array<rational, 2>& Point_2r::elements() const {
    return elements_;
}
inline const uint32_t Point_2r::unique_id() const {
    return unique_id_;
}
inline void Point_2r::set_x(const rational& x) {
    elements_[0] = x;
}
inline void Point_2r::set_y(const rational& y) {
    elements_[1] = y;
}
inline void Point_2r::set_elements(const std::array<rational, 2>& elements) {
    elements_ = elements;
}
inline void Point_2r::set_unique_id(const uint32_t unique_id) {
    unique_id_ = unique_id;
}

//=============================================================================
// Interface: Point_3i
//=============================================================================

class Point_3i {
public:
    Point_3i();
    Point_3i(const Point_3i& p);
    Point_3i(const Point_2i& p);
    Point_3i(const integer& x, const integer& y, const integer& z);

    Point_3i& operator=(const Point_3i& rhs);
    const integer& operator[](const size_t i) const;

    const integer& x() const;
    const integer& y() const;
    const integer& z() const;
    const std::array<integer, 3>& elements() const;
    const uint32_t unique_id() const;
    void set_x(const integer& x);
    void set_y(const integer& y);
    void set_z(const integer& z);
    void set_elements(const std::array<integer, 3>& elements);
    void set_unique_id(const uint32_t unique_id);

private:
    std::array<integer, 3> elements_;
    uint32_t unique_id_;
};

bool operator==(const Point_3i& lhs, const Point_3i& rhs);
bool operator!=(const Point_3i& lhs, const Point_3i& rhs);
std::ostream& operator<<(std::ostream& o, const Point_3i& p);
std::string to_string(const Point_3i& p);

//=============================================================================
// Interface: Point_3f
//=============================================================================

class Point_3f {
public:
    Point_3f();
    Point_3f(const Point_3f& p);
    Point_3f(const Point_2f& p);
    Point_3f(const float x, const float y, const float z);

    Point_3f& operator=(const Point_3f& rhs);
    const float operator[](const size_t i) const;

    const float x() const;
    const float y() const;
    const float z() const;
    const std::array<float, 3>& elements() const;
    const uint32_t unique_id() const;
    void set_x(const float x);
    void set_y(const float y);
    void set_z(const float z);
    void set_elements(const std::array<float, 3>& elements);
    void set_unique_id(const uint32_t unique_id);

private:
    std::array<float, 3> elements_;
    uint32_t unique_id_;
};

bool operator==(const Point_3f& lhs, const Point_3f& rhs);
bool operator!=(const Point_3f& lhs, const Point_3f& rhs);
std::ostream& operator<<(std::ostream& o, const Point_3f& p);
std::string to_string(const Point_3f& p);

//=============================================================================
// Interface: Point_3r
//=============================================================================

class Point_3r {
public:
    Point_3r();
    Point_3r(const Point_3r& p);
    Point_3r(const Point_2r& p);
    Point_3r(const Point_3i& p);
    Point_3r(const Point_3f& p);
    Point_3r(const rational& x, const rational& y, const rational& z);

    void AddObserver(IPointObserver* o);
    void RemoveObserver(IPointObserver* o);
    void SigPositionChanged() const;

    Point_3r& operator=(const Point_3r& rhs);
    const rational& operator[](const size_t i) const;

    const rational& x() const;
    const rational& y() const;
    const rational& z() const;
    const std::array<rational, 3>& elements() const;
    const uint32_t unique_id() const;
    void set_x(const rational& x);
    void set_y(const rational& y);
    void set_z(const rational& z);
    void set_elements(const std::array<rational, 3>& elements);
    void set_unique_id(const uint32_t unique_id);

private:
    std::array<rational, 3> elements_;
    std::vector<IPointObserver*> observers_;
    uint32_t unique_id_;
};

bool operator==(const Point_3r& lhs, const Point_3r& rhs);
bool operator!=(const Point_3r& lhs, const Point_3r& rhs);
std::ostream& operator<<(std::ostream& o, const Point_3r& p);
std::string to_string(const Point_3r& p);

//=============================================================================
// Implementation: Point_3i
//=============================================================================

//! @brief Sets both elements and unique id to zero.
inline Point_3i::Point_3i() {
    set_x(0);
    set_y(0);
    set_z(0);
    set_unique_id(0);
}

inline Point_3i::Point_3i(const Point_3i& p) {
    *this = p;
}

inline Point_3i::Point_3i(const Point_2i& p) {
    set_x(p.x());
    set_y(p.y());
    set_z(0);
    set_unique_id(p.unique_id());
}

inline Point_3i::Point_3i(const integer& x, const integer& y,
                          const integer& z) {
    set_x(x);
    set_y(y);
    set_z(z);
    set_unique_id(0);
}

inline Point_3i& Point_3i::operator=(const Point_3i& rhs) {
    set_x(rhs.x());
    set_y(rhs.y());
    set_z(rhs.z());
    set_unique_id(rhs.unique_id());
    return *this;
}

//! @brief Index-based access operator without elegant out-of-bounds handling.
inline const integer& Point_3i::operator[](const size_t i) const {
    assert(i < 3);
    return elements_[i];
}

//! @brief Sequentially calls operator== element-wise.
inline bool operator==(const Point_3i& lhs, const Point_3i& rhs) {
    return lhs.elements() == rhs.elements();
}

//! @brief Sequentially calls operator!= element-wise.
inline bool operator!=(const Point_3i& lhs, const Point_3i& rhs) {
    return lhs.elements() != rhs.elements();
}

inline std::ostream& operator<<(std::ostream& o, const Point_3i& p) {
    return o << to_string(p);
}

//! @brief Represents this point using parentheses (x, y).
inline std::string to_string(const Point_3i& p) {
    std::stringstream ss;
    ss << "(" << p.x() << ", " << p.y() << ", " << p.z() << ")";
    return ss.str();
}

// Accessors/Mutators =========================================================

inline const integer& Point_3i::x() const {
    return elements_[0];
}
inline const integer& Point_3i::y() const {
    return elements_[1];
}
inline const integer& Point_3i::z() const {
    return elements_[2];
}
inline const std::array<integer, 3>& Point_3i::elements() const {
    return elements_;
}
inline const uint32_t Point_3i::unique_id() const {
    return unique_id_;
}
inline void Point_3i::set_x(const integer& x) {
    elements_[0] = x;
}
inline void Point_3i::set_y(const integer& y) {
    elements_[1] = y;
}
inline void Point_3i::set_z(const integer& z) {
    elements_[2] = z;
}
inline void Point_3i::set_elements(const std::array<integer, 3>& elements) {
    elements_ = elements;
}
inline void Point_3i::set_unique_id(const uint32_t unique_id) {
    unique_id_ = unique_id;
}

//=============================================================================
// Implementation: Point_3f
//=============================================================================

inline Point_3f::Point_3f() {
    set_x(0);
    set_y(0);
    set_z(0);
    set_unique_id(0);
}

inline Point_3f::Point_3f(const Point_3f& p) {
    *this = p;
}

inline Point_3f::Point_3f(const Point_2f& p) {
    set_x(p.x());
    set_y(p.y());
    set_z(0);
    set_unique_id(p.unique_id());
}

inline Point_3f::Point_3f(const float x, const float y, const float z) {
    set_x(x);
    set_y(y);
    set_z(z);
    set_unique_id(0);
}

inline Point_3f& Point_3f::operator=(const Point_3f& rhs) {
    set_x(rhs.x());
    set_y(rhs.y());
    set_z(rhs.z());
    set_unique_id(rhs.unique_id());
    return *this;
}

//! @brief Index-based access operator without elegant out-of-bounds handling.
inline const float Point_3f::operator[](const size_t i) const {
    assert(i < 3);
    return elements_[i];
}

//! @brief Sequentially calls operator== element-wise.
inline bool operator==(const Point_3f& lhs, const Point_3f& rhs) {
    return lhs.elements() == rhs.elements();
}

//! @brief Sequentially calls operator!= element-wise.
inline bool operator!=(const Point_3f& lhs, const Point_3f& rhs) {
    return lhs.elements() != rhs.elements();
}

inline std::ostream& operator<<(std::ostream& o, const Point_3f& p) {
    return o << to_string(p);
}

//! @brief Represents this point using parentheses (x, y).
inline std::string to_string(const Point_3f& p) {
    std::stringstream ss;
    ss << "(" << p.x() << ", " << p.y() << ", " << p.z() << ")";
    return ss.str();
}

// Accessors/Mutators =========================================================

inline const float Point_3f::x() const {
    return elements_[0];
}
inline const float Point_3f::y() const {
    return elements_[1];
}
inline const float Point_3f::z() const {
    return elements_[2];
}
inline const std::array<float, 3>& Point_3f::elements() const {
    return elements_;
}
inline const uint32_t Point_3f::unique_id() const {
    return unique_id_;
}
inline void Point_3f::set_x(const float x) {
    elements_[0] = x;
}
inline void Point_3f::set_y(const float y) {
    elements_[1] = y;
}
inline void Point_3f::set_z(const float z) {
    elements_[2] = z;
}
inline void Point_3f::set_elements(const std::array<float, 3>& elements) {
    elements_ = elements;
}
inline void Point_3f::set_unique_id(const uint32_t unique_id) {
    unique_id_ = unique_id;
}

//=============================================================================
// Implementation: Point_3r
//=============================================================================

inline Point_3r::Point_3r() {
    set_x(0);
    set_y(0);
    set_z(0);
    set_unique_id(0);
}

inline Point_3r::Point_3r(const Point_3r& p) {
    *this = p;
}

inline Point_3r::Point_3r(const Point_2r& p) {
    set_x(p.x());
    set_y(p.y());
    set_z(0);
    set_unique_id(p.unique_id());
}

//! @brief Allow implicit conversion from integer point.
inline Point_3r::Point_3r(const Point_3i& p) {
    set_x(p.x());
    set_y(p.y());
    set_z(p.z());
    set_unique_id(p.unique_id());
}

//! @brief Allow implicit conversion from float point.
inline Point_3r::Point_3r(const Point_3f& p) {
    set_x(p.x());
    set_y(p.y());
    set_z(p.z());
    set_unique_id(p.unique_id());
}

inline Point_3r::Point_3r(const rational& x, const rational& y,
                          const rational& z) {
    set_x(x);
    set_y(y);
    set_z(z);
    set_unique_id(0);
}

inline void Point_3r::AddObserver(IPointObserver* o) {
    observers_.push_back(o);
}

inline void Point_3r::RemoveObserver(IPointObserver* o) {
    std::remove(begin(observers_), end(observers_), o);
}

inline void Point_3r::SigPositionChanged() const {
    for(auto i = begin(observers_); i != end(observers_); ++i) {
        (*i)->SlotPositionChanged_3r(*this);
    }
}

inline Point_3r& Point_3r::operator=(const Point_3r& rhs) {
    set_x(rhs.x());
    set_y(rhs.y());
    set_z(rhs.z());
    set_unique_id(rhs.unique_id());
    return *this;
}

//! @brief Index-based access operator without elegant out-of-bounds handling.
inline const rational& Point_3r::operator[](const size_t i) const {
    assert(i < 3);
    return elements_[i];
}

//! @brief Sequentially calls operator== element-wise.
inline bool operator==(const Point_3r& lhs, const Point_3r& rhs) {
    return lhs.elements() == rhs.elements();
}

//! @brief Sequentially calls operator!= element-wise.
inline bool operator!=(const Point_3r& lhs, const Point_3r& rhs) {
    return lhs.elements() != rhs.elements();
}


inline std::ostream& operator<<(std::ostream& o, const Point_3r& p) {
    return o << to_string(p);
}

//! @brief Represents this point using parentheses (x, y).
inline std::string to_string(const Point_3r& p) {
    std::stringstream ss;
    ss << "(" << p.x() << ", " << p.y() << ", " << p.z() << ")";
    return ss.str();
}

// Accessors/Mutators =========================================================

inline const rational& Point_3r::x() const {
    return elements_[0];
}
inline const rational& Point_3r::y() const {
    return elements_[1];
}
inline const rational& Point_3r::z() const {
    return elements_[2];
}
inline const std::array<rational, 3>& Point_3r::elements() const {
    return elements_;
}
inline const uint32_t Point_3r::unique_id() const {
    return unique_id_;
}
inline void Point_3r::set_x(const rational& x) {
    elements_[0] = x;
}
inline void Point_3r::set_y(const rational& y) {
    elements_[1] = y;
}
inline void Point_3r::set_z(const rational& z) {
    elements_[2] = z;
}
inline void Point_3r::set_elements(const std::array<rational, 3>& elements) {
    elements_ = elements;
}
inline void Point_3r::set_unique_id(const uint32_t unique_id) {
    unique_id_ = unique_id;
}

} // namespace DDAD

#endif // GE_POINT_H
