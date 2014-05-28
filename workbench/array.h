/*
 * This file is part of RationalCAD.
 *
 * RationalCAD is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * RationalCAD is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details. You should have received a copy of the GNU General Public
 * License along with RationalCAD. If not, see <http://www.gnu.org/licenses/>.
 */

/*!
 * @author Clinton Freeman <admin@freemancw.com>
 * @date 2013-03-02
 * @brief Generic, dynamic, 2D array type.
 *
 * Code adapted from:
 * http://www.codeproject.com/Articles/310822/Rock-Solid-Cplusplus-2D-Templatized-Array-Class
 */

#ifndef RC_ARRAY_H
#define RC_ARRAY_H

#include "rc_common.h"

BEGIN_NAMESPACE(RCAD)

//=============================================================================
// Interface: Array
//=============================================================================

template<typename T>
class Array {
public:
    Array();
    Array(const size_t n_rows, const size_t n_cols);
    ~Array();

    void resize(const size_t n_rows, const size_t n_cols);

    const T& operator()(const size_t i) const;
    T& operator()(const size_t i);
    const T& operator()(const size_t i, const size_t j) const;
    T& operator()(const size_t i, const size_t j);

    size_t n_rows() const;
    size_t n_cols() const;
    size_t n_elts() const;

    const T* const ptr() const;
    T* ptr();

private:
    size_t checkedIndex(size_t i) const;
    size_t checkedIndex(size_t i, size_t j) const;

    size_t n_rows_;
    size_t n_cols_;
    T* data_;
};

//=============================================================================
// Implementation: Array
//=============================================================================

template<typename T>
Array<T>::Array() :
    n_rows_(0),
    n_cols_(0),
    data_(NULL) {}

template<typename T>
Array<T>::Array(const size_t n_rows, const size_t n_cols) :
    n_rows_(n_rows),
    n_cols_(n_cols) {
    data_ = new T[n_rows * n_cols];
}

template<typename T>
Array<T>::~Array() {
    if (data_ != NULL) {
        delete[] data_;
    }
}

template<typename T>
void Array<T>::resize(const size_t n_rows, const size_t n_cols) {
    if (data_ != NULL) {
        delete[] data_;
    }

    n_rows_ = n_rows;
    n_cols_ = n_cols;
    data_ = new T[n_rows*n_cols];
}

template<typename T>
inline const T& Array<T>::operator()(const size_t i) const {
    return data_[checkedIndex(i)];
}

template<typename T>
inline T& Array<T>::operator()(const size_t i) {
    return data_[checkedIndex(i)];
}

template<typename T>
inline const T& Array<T>::operator()(const size_t i, const size_t j) const {
    return data_[checkedIndex(i, j)];
}

template<typename T>
inline T& Array<T>::operator()(const size_t i, const size_t j) {
    return data_[checkedIndex(i, j)];
}

// Accessors/Mutators =========================================================

template<typename T>
inline size_t Array<T>::n_rows() const {
    return n_rows_;
}
template<typename T>
inline size_t Array<T>::n_cols() const {
    return n_cols_;
}
template<typename T>
inline size_t Array<T>::n_elts() const {
    return n_rows_*n_cols_;
}
template<typename T>
inline const T* const Array<T>::ptr() const {
    return data_;
}
template<typename T>
inline T* Array<T>::ptr() {
    return data_;
}

// Private member functions ===================================================

template<typename T>
inline size_t Array<T>::checkedIndex(const size_t i) const {
    assert(i < n_elts());
    return i;
}

template<typename T>
inline size_t Array<T>::checkedIndex(const size_t i, const size_t j) const {
    size_t k = n_cols_*i+j;
    assert(k < n_elts());
    return k;
}

END_NAMESPACE(RCAD)

#endif // RC_ARRAY_H
