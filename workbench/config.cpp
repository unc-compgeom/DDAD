/*!
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 2013-04-22
 */

// Qt
#include <QSysInfo>

// Workbench
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

    //rInfo("Setting default GL format.");

    input_state_ = CREATE_POLYTOPE;
    input_color_ = RED;
    snap_to_grid_ = true;
    grid_minor_color_ = QColor(175, 175, 175);
    grid_major_color_ = QColor(75, 75, 75);
}

const InputState& ConfigManager::input_state() const {
    return input_state_;
}
const InputColor& ConfigManager::input_color() const {
    return input_color_;
}
void ConfigManager::set_input_state(const InputState& input_state) {
    input_state_ = input_state;
}
void ConfigManager::switch_input_color() {
    if(input_color_ == RED) {input_color_ = BLUE;}
    if(input_color_ == BLUE){input_color_ = RED;}
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

