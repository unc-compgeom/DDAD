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

#ifndef WB_CONFIG_H
#define WB_CONFIG_H

// DDAD
#include "common.h"

namespace DDAD {

enum class InputState {
    SELECT,
    TRANSLATE,
    ROTATE,
    CREATE_POINTSET,
    UPDATE_POINTSET,
    CREATE_POLYLINE,
    UPDATE_POLYLINE,
    CREATE_POLYTOPE,
    UPDATE_POLYTOPE
};

class ConfigManager {
private:
    ConfigManager();
    ConfigManager(const ConfigManager&);
    ConfigManager& operator=(const ConfigManager&);

public:
    static ConfigManager& get() {
        static ConfigManager instance;
        return instance;
    }

    void Initialize();

    const InputState& input_state() const;
    void set_input_state(const InputState& input_state);
    bool snap_to_grid() const;
    void set_snap_to_grid(bool snap_to_grid);
    const QColor& grid_minor_color() const;
    void set_grid_minor_color(const QColor& grid_minor_color);
    const QColor& grid_major_color() const;
    void set_grid_major_color(const QColor& grid_major_color);

private:
    InputState input_state_;
    bool snap_to_grid_;
    QColor grid_minor_color_;
    QColor grid_major_color_;
};

} // namespace DDAD

#endif // WB_CONFIG_H
