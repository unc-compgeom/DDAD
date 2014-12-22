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

// Qt
#include <QSysInfo>

// DDAD
#include "common.h"
#include "config.h"

namespace DDAD {

ConfigManager::ConfigManager() {}
ConfigManager::ConfigManager(const ConfigManager&) {}
ConfigManager& ConfigManager::operator=(const ConfigManager&) { return *this; }

void ConfigManager::Initialize() {
    QGLFormat glf = QGLFormat::defaultFormat();
    glf.setRedBufferSize(8);
    glf.setGreenBufferSize(8);
    glf.setBlueBufferSize(8);
    glf.setAlphaBufferSize(8);
    glf.setSampleBuffers(true);
    glf.setSamples(8);
    glf.setDepth(true);
    glf.setDepthBufferSize(24);
    glf.setVersion(3, 3);
    glf.setProfile(QGLFormat::CompatibilityProfile);
    glf.setSwapInterval(1);
    QGLFormat::setDefaultFormat(glf);

    input_state_ = InputState::SELECT;
    snap_to_grid_ = true;
    grid_minor_color_ = QColor(175, 175, 175);
    grid_major_color_ = QColor(75, 75, 75);
}

const InputState& ConfigManager::input_state() const {
    return input_state_;
}
void ConfigManager::set_input_state(const InputState& input_state) {
    input_state_ = input_state;
}
bool ConfigManager::snap_to_grid() const {
    return snap_to_grid_;
}
void ConfigManager::set_snap_to_grid(bool snap_to_grid) {
    snap_to_grid_ = snap_to_grid;
}
const QColor& ConfigManager::grid_minor_color() const {
    return grid_minor_color_;
}
void ConfigManager::set_grid_minor_color(const QColor& grid_minor_color) {
    grid_minor_color_ = grid_minor_color;
}
const QColor& ConfigManager::grid_major_color() const {
    return grid_major_color_;
}
void ConfigManager::set_grid_major_color(const QColor& grid_major_color) {
    grid_major_color_ = grid_major_color;
}

} // namespace DDAD

