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
 * @brief Main window responsible for forwarding widget input.
 */

#ifndef WB_QT_WINDOW_MAIN_H
#define WB_QT_WINDOW_MAIN_H

// Qt
#include <QMainWindow>

// DDAD
#include "common.h"
#include "scene.h"
#include "logger.h"

BEGIN_NAMESPACE(Ui)

class MainWindow;

END_NAMESPACE(Ui)

class MainWindow : public QMainWindow {

    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    // polytope
    void BeginCreatePolytope(const QString& name, const QColor& face_color);
    void EndCreatePolytope();

    void CreateTerrainMesh(const QVector<QVector3D>& data);

    // all objects
    void UpdateSelectedObjectName(const QString& name);
    void UpdateSelectedObjectColor(const QColor& color);
    void DeleteSelectedObject();
    void Deselect();
    void QuitManager();

private slots:
    void onUpdateStatusBarMsg(const QString& status);
    void onLogMessage(const QOpenGLDebugMessage& message);

    // toolbar actions
    void on_select_objects_toggled(bool checked);
    void on_translate_toggled(bool checked);
    void on_rotate_toggled(bool checked);
    void on_snap_to_grid_toggled(bool checked);

    // create buttons
    void on_create_polytope_toggled(bool checked);
    void on_create_polyline_toggled(bool checked);
    void on_create_point_set_toggled(bool checked);

    void on_action_about_triggered();
    void on_action_preferences_triggered();
    void on_action_user_manual_triggered();

    void onUpdateContextSensitiveMenus(const QString& selected_object_type,
                                       const QString& selected_object_name);

private:
    void initializeLogging();
    void initializeConnections();

    void uncheckCreateButtons();
    void uncheckInputModeButtons();

    //void resetToDefaultCreationMode();
    //void addDefaultPointSetCreationMode();


    QActionGroup *toolbar_buttons_;
    QAction* select_button_;
    QAction* translate_button_;
    QAction* rotate_button_;
    QAction* snap_to_grid_button_;

    Ui::MainWindow *ui;

    DDAD::Renderer* renderer_;
    DDAD::SceneManager* scene_manager_;

    QColor create_object_color_;
};

#endif // WB_QT_WINDOW_MAIN_H
