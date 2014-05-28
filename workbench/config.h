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
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 2013-08-16
 * @brief Manager type responsible for global configuration settings.
 */

#ifndef RC_CONFIG_H
#define RC_CONFIG_H

// RationalCAD
#include "common.h"

namespace DDAD {

enum InputState {
    SELECT,
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

#endif // RC_CONFIG_H
