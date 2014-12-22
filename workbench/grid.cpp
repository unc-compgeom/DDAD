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

// DDAD
#include "common.h"
#include "grid.h"
#include "scene.h"
#include "config.h"

namespace DDAD {

//=============================================================================
// Constructors / Destructors
//=============================================================================

IntegerGrid::IntegerGrid() {}

//=============================================================================
// Grid Creation
//=============================================================================

void IntegerGrid::InitializeGrid(const int min_pixel_spacing,
                                 const int major_line_spacing,
                                 const int n_major_lines,
                                 const int bit_complexity,
                                 QVector<GL::Vertex>& grid_verts) {
    Q_UNUSED(bit_complexity);

    //rInfo("Initializing coordinate grid.");

    min_pixel_spacing_ = min_pixel_spacing;
    major_line_spacing_ = major_line_spacing;
    n_major_lines_ = n_major_lines;
    global_mag_max_ = 5;
    local_mag_max_ = major_line_spacing_-1;
    global_mag_ = global_mag_max_-1;
    local_mag_ = local_mag_max_-1;

    // calculate how many lines in total (+1 accounts for lines through origin)
    int n_in_halfspace = n_major_lines_ * major_line_spacing_;
    int n_along_axis = n_in_halfspace * 2 + 1;
    int n_axes = 2;
    n_grid_lines_ = n_along_axis * n_axes;

    // need two verts per line
    n_grid_verts_ = n_grid_lines_ * 2;

    // calculate how far lines ought to extend from the origin
    //int line_half_length = min_pixel_spacing_ * n_in_halfspace;
    int line_half_length = n_in_halfspace;

    auto CreateVerts = [&](const int cur_pos, const QColor& color) {
        Point_3f v;

        // vertical top
        v.set_x(cur_pos);
        v.set_y(line_half_length);
        GL::Vertex v_top(v, color);
        // vertical bottom
        v.set_x(cur_pos);
        v.set_y(-line_half_length);
        GL::Vertex v_bottom(v, color);
        // horizontal left
        v.set_x(-line_half_length);
        v.set_y(cur_pos);
        GL::Vertex v_left(v, color);
        // horizontal right
        v.set_x(line_half_length);
        v.set_y(cur_pos);
        GL::Vertex v_right(v, color);

        grid_verts.push_back(v_top);
        grid_verts.push_back(v_bottom);
        grid_verts.push_back(v_left);
        grid_verts.push_back(v_right);
    };

    // create major lines first so that they'll render on top of minor lines

    // major lines
    for (int i = 0; i < n_along_axis; ++i) {
        if (i % major_line_spacing_) {
            continue;
        }

        CreateVerts(-line_half_length + i,
                    ConfigManager::get().grid_major_color());
    }

    // minor lines
    for (int i = 0; i < n_along_axis; ++i) {
        if (!(i % major_line_spacing_)) {
            continue;
        }

        CreateVerts(-line_half_length + i,
                    ConfigManager::get().grid_minor_color());
    }

    // setup modelview matrix
    WrapGrid();
}

/*!
 * @brief IntegerGrid::GetMajorXCoords
 * @param width
 * @return vector of (pixel, value) line labels for visible x-coords.
 */
QVector<QPair<int, int>> IntegerGrid::GetMajorXCoords(const int width) const {
    QVector<QPair<int, int>> out;

    qreal world_half_width = static_cast<qreal>(width)/global_scale_/2.0;

    int left_x  = global_pos_.x()-world_half_width;
    int right_x = global_pos_.x()+world_half_width;
    int sweep_x = RoundToNearestMajor(left_x);
    int end_x   = RoundToNearestMajor(right_x);
    int coord_inc = GetMajorCoordIncrement();

    while (sweep_x <= end_x) {
        int x_pixel = CoordToPixel(sweep_x, global_pos_.x(), width);
        out.push_back(QPair<int, int>(x_pixel, sweep_x));
        sweep_x += coord_inc;
    }

    return out;
}

/*!
 * @brief IntegerGrid::GetMajorYCoords
 * @param height - viewport height
 * @return - vector of (pixel, value) line labels for visible y-coords.
 */
QVector<QPair<int, int>> IntegerGrid::GetMajorYCoords(const int height) const {
    QVector<QPair<int, int>> out;

    qreal world_half_height = static_cast<qreal>(height)/global_scale_/2.0;

    int top_y    = global_pos_.y()-world_half_height;
    int bottom_y = global_pos_.y()+world_half_height;
    int sweep_y  = RoundToNearestMajor(top_y);
    int end_y    = RoundToNearestMajor(bottom_y);
    int major_inc = GetMajorCoordIncrement();

    while (sweep_y <= end_y) {
        int y_pixel = CoordToPixel(sweep_y, global_pos_.y(), height);
        out.push_back(QPair<int, int>(y_pixel, sweep_y));
        sweep_y += major_inc;
    }

    return out;
}

//=============================================================================
// Magnification and Translation
//=============================================================================

/*!
 * @brief IntegerGrid::IncreaseMagnification
 */
void IntegerGrid::IncreaseMagnification() {
    if (local_mag_ == local_mag_max_-1 && global_mag_ == global_mag_max_-1) {
        return;
    }

    if (++local_mag_ == local_mag_max_) {
        local_mag_ = 0;
        local_pos_ *= major_line_spacing_;

        if (++global_mag_ == global_mag_max_) {
            global_mag_ = global_mag_max_-1;
        }
    }

    // ensure magnifications are within [0, MAX)
    assert(local_mag_ >= 0 && local_mag_ < local_mag_max_);
    assert(global_mag_ >= 0 && global_mag_ < global_mag_max_);

    WrapGrid();
}

/*!
 * @brief IntegerGrid::DecreaseMagnification
 */
void IntegerGrid::DecreaseMagnification() {
    if (local_mag_ == 0 && global_mag_ == 0) {
        return;
    }

    if (--local_mag_ < 0) {
        local_mag_ = local_mag_max_-1;
        local_pos_ /= major_line_spacing_;

        if (--global_mag_ < 0) {
            global_mag_ = 0;
        }
    }

    // ensure magnifications are within [0, MAX)
    assert(local_mag_ >= 0 && local_mag_ < local_mag_max_);
    assert(global_mag_ >= 0 && global_mag_ < global_mag_max_);

    WrapGrid();
}

/*!
 * @brief IntegerGrid::Translate
 * @param pixel_delta
 */
void IntegerGrid::Translate(const QVector2D &pixel_delta) {
    global_pos_ += pixel_delta/global_scale_;
    local_pos_  += pixel_delta/local_scale_;

    WrapGrid();
}

/*!
 * @brief IntegerGrid::GetCoordsForPixel
 * @param pixel - position in screen space
 * @return position in world space
 */
QVector2D IntegerGrid::GetCoordsForPixel(const QVector2D &pixel) const {
    return QVector2D(pixel.x()/global_scale_+global_pos_.x(),
                     pixel.y()/global_scale_+global_pos_.y());
}

/*!
 * @brief IntegerGrid::WrapGrid
 */
void IntegerGrid::WrapGrid() {
    local_scale_ = (local_mag_max_*min_pixel_spacing_)/
            (local_mag_max_-local_mag_);

    global_scale_ = local_scale_/
            powf(major_line_spacing_, global_mag_max_-global_mag_-1);

    local_pos_ = global_pos_*global_scale_/local_scale_;
    qreal min_translation = min_pixel_spacing_*major_line_spacing_;

    while (fabs(local_pos_.x()) > min_translation) {
        if (local_pos_.x() > 0.0) {
            local_pos_.setX(local_pos_.x()-min_translation);
        } else {
            local_pos_.setX(local_pos_.x()+min_translation);
        }
    }

    while (fabs(local_pos_.y()) > min_translation) {
        if (local_pos_.y() > 0.0) {
            local_pos_.setY(local_pos_.y()-min_translation);
        } else {
            local_pos_.setY(local_pos_.y()+min_translation);
        }
    }

    /*
    qDebug() << "lscale = " << local_scale_ << "\n";
    qDebug() << "gscale = " << global_scale_ << "\n";
    qDebug() << "lmag = " << local_mag_ << "\n";
    qDebug() << "gmag = " << global_mag_ << "\n";
    qDebug() << "lpos = " << local_pos_ << "\n";
    qDebug() << "gpos = " << global_pos_ << "\n";
    */
}

/*!
 * @brief IntegerGrid::RoundToNearestMinor
 * @param coord
 * @return
 */
int IntegerGrid::RoundToNearestMinor(const int coord) const {
    return coord;
}

/*!
 * @brief IntegerGrid::RoundToNearestMajor
 * @param coord - value along some axis in world space
 * @return input value rounded to the nearest major grid line in world space
 */
int IntegerGrid::RoundToNearestMajor(const int coord) const {
    int lowest_snap = GetMajorCoordIncrement();
    return lowest_snap*static_cast<int>(coord/lowest_snap);
}

/*!
 * @brief IntegerGrid::GetMajorCoordIncrement
 * @return distance in world coordinates between successive major grid lines
 */
int IntegerGrid::GetMajorCoordIncrement() const {
    return major_line_spacing_*
            powf(major_line_spacing_, global_mag_max_-global_mag_-1);
}

/*!
 * @brief IntegerGrid::CoordToPixel
 * @param a - value along an axis
 * @param global_a - global position of camera along the same axis
 * @param length - width for x axis or height for y axis
 * @return pixel value for a position in world space along some axis
 */
int IntegerGrid::CoordToPixel(const int a, const qreal global_a,
                              const int length) const {
    return ((a-global_a)*global_scale_)+(length/2);
}

const QVector2D& IntegerGrid::local_pos() const {
    return local_pos_;
}
const qreal& IntegerGrid::local_scale() const {
    return local_scale_;
}
const QVector2D& IntegerGrid::global_pos() const {
    return global_pos_;
}
const qreal& IntegerGrid::global_scale() const {
    return global_scale_;
}

} // namespace DDAD
