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
 * @brief Customizable grid type to support precise placement of objects.
 */

#ifndef WB_GRID_H
#define WB_GRID_H

// DDAD
#include "common.h"

namespace DDAD {

namespace GL {

class Vertex;

}

class IntegerGrid {
public:
    IntegerGrid();

    void InitializeGrid(const int min_pixel_spacing,
                        const int major_line_spacing,
                        const int n_major_lines,
                        const int bit_complexity,
                        QVector<GL::Vertex>& grid_verts);

    void IncreaseMagnification();
    void DecreaseMagnification();
    void Translate(const QVector2D& pixel_delta);
    QVector<QPair<int, int>> GetMajorXCoords(const int width) const;
    QVector<QPair<int, int>> GetMajorYCoords(const int height) const;
    QVector2D GetCoordsForPixel(const QVector2D& pixel) const;
    int RoundToNearestMinor(const int coord) const;
    int RoundToNearestMajor(const int coord) const;

    const QVector2D& local_pos() const;
    const qreal& local_scale() const;
    const QVector2D& global_pos() const;
    const qreal& global_scale() const;

private:
    void WrapGrid();
    int CoordToPixel(const int a, const qreal global_a,
                     const int length) const;
    int GetMajorCoordIncrement() const;

    QVector2D global_pos_;
    qreal global_scale_;

    int min_pixel_spacing_;         // the minimum number of pixels we will
                                    // accept between minor grid lines before
                                    // we move to a lower global magnification

    int major_line_spacing_;        // the number of columns of whitespace
                                    // between each successive major grid line

    int n_major_lines_;             // the number of major gridlines to draw
                                    // on each side of the origin, not
                                    // including the origin
                                    // TODO: automatically determine this

    int local_mag_;
    int local_mag_max_;
    int global_mag_;
    int global_mag_max_;

    QVector2D local_pos_;
    qreal local_scale_;

    int n_grid_lines_;
    int n_grid_verts_;

    //static const int kMaxGlobalMag;
    //static const int kMaxLocalMag;

    //Q_DISABLE_COPY(IntegerGrid)
};

} // namespace DDAD

#endif // WB_GRID_H
