/*!
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 2013-08-16
 * @brief Manager type responsible for global configuration settings.
 */

#ifndef RC_CONFIG_H
#define RC_CONFIG_H

// Workbench
#include "common.h"

namespace DDAD {

enum InputState {
    SELECT,
    CREATE_POLYLINE,
    UPDATE_POLYLINE,
    CREATE_ARRANGEMENT,
    CREATE_SEGMENT,
    UPDATE_ARRANGEMENT,
    CREATE_POLYTOPE,
    UPDATE_POLYTOPE
};

enum InputColor {
    RED,
    BLUE
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
    const InputColor& input_color() const;
    void switch_input_color();
    void set_input_state(const InputState& input_state);
    bool snap_to_grid() const;
    void set_snap_to_grid(bool snap_to_grid);
    const QColor& grid_minor_color() const;
    void set_grid_minor_color(const QColor& grid_minor_color);
    const QColor& grid_major_color() const;
    void set_grid_major_color(const QColor& grid_major_color);

private:
    InputState input_state_;
    InputColor input_color_;
    bool snap_to_grid_;
    QColor grid_minor_color_;
    QColor grid_major_color_;
};

} // namespace DDAD

#endif // RC_CONFIG_H
