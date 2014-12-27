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
#include "qt_dialog_about.h"
#include "qt_dialog_preferences.h"
#include "qt_window_main.h"
#include "ui_window_main.h"
#include "qt_widget_orthographic.h"
#include "qt_widget_perspective.h"
#include "scene.h"
#include "qt_point_set_creation_method.h"
#include "qt_polyline_creation_method.h"
#include "qt_polytope_creation_method.h"
#include "qt_point_set_algorithms.h"
#include "qt_polyline_algorithms.h"
#include "qt_polytope_algorithms.h"

#include "../geometry/point.h"

using namespace DDAD;

//=============================================================================
// Constructors/Destructors
//=============================================================================

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {

    ui->setupUi(this);

    initializeLogging();

    ConfigManager::get().Initialize();

    // toolbar buttons
    toolbar_buttons_ = new QActionGroup(ui->toolBar->layout());

    // select objects button
    select_button_ = new QAction("Select", toolbar_buttons_);
    select_button_->setIcon(QIcon("://icons/select.png"));
    select_button_->setCheckable(true);
    select_button_->setChecked(true);

    connect(select_button_,
            SIGNAL(toggled(bool)),
            SLOT(on_select_objects_toggled(bool)));

    // translate button
    translate_button_ = new QAction("Translate", toolbar_buttons_);
    translate_button_->setIcon(QIcon("://icons/translate.png"));
    translate_button_->setCheckable(true);

    connect(translate_button_,
            SIGNAL(toggled(bool)),
            SLOT(on_translate_toggled(bool)));

    // rotate button
    rotate_button_ = new QAction("Rotate", toolbar_buttons_);
    rotate_button_->setIcon(QIcon("://icons/rotate.png"));
    rotate_button_->setCheckable(true);

    connect(rotate_button_,
            SIGNAL(toggled(bool)),
            SLOT(on_rotate_toggled(bool)));

    // snap to grid button
    snap_to_grid_button_ = new QAction("Snap to Grid", ui->toolBar->layout());
    snap_to_grid_button_->setIcon(QIcon("://icons/snap_to_grid.png"));
    snap_to_grid_button_->setCheckable(true);
    snap_to_grid_button_->setChecked(true);

    connect(snap_to_grid_button_,
            SIGNAL(toggled(bool)),
            SLOT(on_snap_to_grid_toggled(bool)));

    // add buttons to toolbar
    ui->toolBar->addAction(select_button_);
    ui->toolBar->addAction(translate_button_);
    ui->toolBar->addAction(rotate_button_);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(snap_to_grid_button_);

    // create perspective widget
    qDebug() << "Creating perspective view";
    auto perspective = new PerspectiveWidget(ui->group_perspective);
    ui->group_perspective->layout()->addWidget(perspective);
    perspective->installEventFilter(this);

    // create orthographic widget
    qDebug() << "Creating orthographic view";
    auto ortho_top = new OrthographicWidget(TOP, ui->group_top, perspective);
    ui->group_top->layout()->addWidget(ortho_top);
    ortho_top->installEventFilter(this);

    perspective->context()->makeCurrent();

    // create renderer
    qDebug() << "Creating renderer.";
    renderer_ = new Renderer();

    // create scene manager
    qDebug() << "Creating scene manager.";
    scene_manager_ = new SceneManager(renderer_);

    connect(&scene_manager_->scene_observer_,
            SIGNAL(UpdateContextSensitiveMenus(QString, QString)),
            this,
            SLOT(onUpdateContextSensitiveMenus(QString, QString)));

    // initialize widgets
    qDebug() << "Initializing ortho.";
    ortho_top->initialize(renderer_, scene_manager_);
    qDebug() << "Initializing perspective.";
    perspective->initialize(renderer_, scene_manager_);

    connect(ortho_top,
            SIGNAL(ChangeMessage(const QString&)),
            SLOT(onUpdateStatusBarMsg(const QString&)));
    connect(this,
            SIGNAL(Deselect()),
            &scene_manager_->scene_observer_,
            SLOT(onDeselect()));
    connect(this,
            SIGNAL(DeleteSelectedObject()),
            &scene_manager_->scene_observer_,
            SLOT(onDeleteSelectedObject()));

    QList<int> sizes;
    sizes.push_back(this->height());
    sizes.push_back(0);
    ui->vsplit_console->setSizes(sizes);
}

void MainWindow::onLogMessage(const QOpenGLDebugMessage &message) {
    qDebug() << message.message();
}

MainWindow::~MainWindow() {
    delete scene_manager_;
    delete renderer_;
    delete ui;
}

void MainWindow::uncheckCreateButtons() {
    ui->buttonGroup->setExclusive(false);
    ui->create_point_set->setChecked(false);
    ui->create_polyline->setChecked(false);
    ui->create_polytope->setChecked(false);
    ui->buttonGroup->setExclusive(true);
}

void MainWindow::uncheckInputModeButtons() {
    toolbar_buttons_->setExclusive(false);
    select_button_->setChecked(false);
    translate_button_->setChecked(false);
    rotate_button_->setChecked(false);
    toolbar_buttons_->setExclusive(true);
}

void MainWindow::on_select_objects_toggled(bool checked) {
    qDebug() << "on_select_objects_toggled: " << checked;
    if (checked) {
        ConfigManager::get().set_input_state(InputState::SELECT);
        uncheckCreateButtons();
    }
}

void MainWindow::on_translate_toggled(bool checked) {
    qDebug() << "on_translate_toggled: " << checked;
    if (checked) {
        ConfigManager::get().set_input_state(InputState::TRANSLATE);
        uncheckCreateButtons();
    }
}

void MainWindow::on_rotate_toggled(bool checked) {
    qDebug() << "on_rotate_toggled: " << checked;
    if (checked) {
        ConfigManager::get().set_input_state(InputState::ROTATE);
        uncheckCreateButtons();
    }
}

void MainWindow::on_create_point_set_toggled(bool checked) {
    qDebug() << "on_create_point_set_toggled: " << checked;

    static PointSetCreationMethod *creation_method = nullptr;

    if (checked) {
        ConfigManager::get().set_input_state(InputState::CREATE_POINTSET);
        uncheckInputModeButtons();

        creation_method = new PointSetCreationMethod();

        QLayoutItem *spacer = ui->create_tab_spacer;
        ui->create->layout()->removeItem(spacer);
        ui->create->layout()->addWidget(creation_method);
        ui->create->layout()->addItem(spacer);

        connect(creation_method,
                SIGNAL(CreatePointSet(const QVector<QVector3D>&)),
                &scene_manager_->scene_observer_,
                SLOT(onCreatePointSet(const QVector<QVector3D>&)));

    } else if (creation_method) {
        ui->create->layout()->removeWidget(creation_method);
        delete creation_method;
        creation_method = nullptr;
    }
}

void MainWindow::on_create_polyline_toggled(bool checked) {
    qDebug() << "on_create_polyline_toggled: " << checked;

    static PolylineCreationMethod *creation_method = nullptr;

    if (checked) {
        ConfigManager::get().set_input_state(InputState::CREATE_POLYLINE);
        uncheckInputModeButtons();

        creation_method = new PolylineCreationMethod();

        QLayoutItem *spacer = ui->create_tab_spacer;
        ui->create->layout()->removeItem(spacer);
        ui->create->layout()->addWidget(creation_method);
        ui->create->layout()->addItem(spacer);

    } else if (creation_method) {
        ui->create->layout()->removeWidget(creation_method);
        delete creation_method;
        creation_method = nullptr;
    }
}

void MainWindow::on_create_polytope_toggled(bool checked) {
    qDebug() << "on_create_polytope_toggled: " << checked;

    static PolytopeCreationMethod *creation_method = nullptr;

    if (checked) {
        ConfigManager::get().set_input_state(InputState::CREATE_POLYTOPE);
        uncheckInputModeButtons();

        creation_method = new PolytopeCreationMethod();

        QLayoutItem *spacer = ui->create_tab_spacer;
        ui->create->layout()->removeItem(spacer);
        ui->create->layout()->addWidget(creation_method);
        ui->create->layout()->addItem(spacer);

    } else if (creation_method) {
        ui->create->layout()->removeWidget(creation_method);
        delete creation_method;
        creation_method = nullptr;
    }
}

void MainWindow::on_snap_to_grid_toggled(bool state) {
    ConfigManager::get().set_snap_to_grid(state);
}

void MainWindow::initializeLogging() {

}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if(event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        switch(keyEvent->key()) {
        case Qt::Key_Escape:
        case Qt::Key_Delete:
        case Qt::Key_Backspace:
            keyPressEvent(keyEvent);
            return true;
        default:
            break;
        }
    }

    return QObject::eventFilter(obj, event);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
    case Qt::Key_Escape:
        emit Deselect();
        break;
    case Qt::Key_Delete:
    case Qt::Key_Backspace:
        emit DeleteSelectedObject();
        break;
    default:
        break;
    }
}

//=============================================================================
// Top Menubar Items
//=============================================================================

void MainWindow::onUpdateStatusBarMsg(const QString &status) {
    ui->statusbar_main->showMessage(status);
}

void MainWindow::on_action_about_triggered() {
    AboutDialog *a = new AboutDialog(this);
    a->addBuildDate();
    a->show();
}

void MainWindow::on_action_preferences_triggered() {
    PreferencesDialog *p = new PreferencesDialog(this);
    p->show();
}

void MainWindow::on_action_user_manual_triggered() {
    QDesktopServices::openUrl(QUrl("file:///C:/DDADUserManual.pdf"));
}

void MainWindow::onUpdateContextSensitiveMenus(const QString &selected_object_type,
                                               const QString &selected_object_name) {
    LOG(DEBUG) << "updating context sensitive menus for object type " << selected_object_type.toStdString();

    static QWidget* algorithms = nullptr;
    QLayoutItem *spacer = ui->compute_tab_spacer;

    if (algorithms) {
        LOG(DEBUG) << "deleting algorithms menu";
        ui->compute->layout()->removeWidget(algorithms);
        delete algorithms;
        algorithms = nullptr;
    }

    if (selected_object_type == "") {
        ui->object_name->setText("No object selected");
        return;
    }

    ui->object_name->setText(selected_object_name);

    if (selected_object_type == "PointSet") {
        algorithms = new PointSetAlgorithms();
        connect(algorithms,
                SIGNAL(ComputeTerrainMeshForSelectedPointSet()),
                &scene_manager_->scene_observer_,
                SLOT(onComputeTerrainMeshForSelectedPointSet()));
    } else if (selected_object_type == "Polyline") {
        algorithms = new PolylineAlgorithms();
        connect(algorithms,
                SIGNAL(ComputeMelkmanForSelectedPolyline()),
                &scene_manager_->scene_observer_,
                SLOT(onComputeMelkmanForSelectedPolyline()));
    } else if (selected_object_type == "Polytope") {
        algorithms = new PolytopeAlgorithms();
    }

    ui->compute->layout()->removeItem(spacer);
    ui->compute->layout()->addWidget(algorithms);
    ui->compute->layout()->addItem(spacer);
}
