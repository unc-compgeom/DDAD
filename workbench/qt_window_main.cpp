/*!
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 2012-05-23
 */

// Workbench
#include "common.h"
#include "qt_dialog_about.h"
#include "qt_dialog_preferences.h"
#include "qt_window_main.h"
#include "ui_window_main.h"
#include "qt_widget_orthographic.h"
#include "qt_widget_perspective.h"
#include "scene.h"

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

    // create toolbar buttons
    QActionGroup *input_state_buttons = new QActionGroup(ui->toolBar->layout());

    // select objects button
    QAction* select_objects = new QAction("Select Objects",
                                          input_state_buttons);
    select_objects->setIcon(QIcon("://icons/select_object.png"));
    select_objects->setCheckable(true);

    connect(select_objects,
            SIGNAL(triggered()),
            SLOT(onSelectObjectsTriggered()));

    // create polyline button
    QAction* create_polyline = new QAction("Create PolyLine",
                                           input_state_buttons);
    //create_polytope->setIcon(QIcon("://icons/create_polytope.png"));
    create_polyline->setCheckable(true);
    create_polyline->setChecked(true);

    connect(create_polyline,
            SIGNAL(triggered()),
            SLOT(onCreatePolyLineTriggered()));

    // create polytope button
    QAction* create_polytope = new QAction("Create Polytope",
                                           input_state_buttons);
    create_polytope->setIcon(QIcon("://icons/create_polytope.png"));
    create_polytope->setCheckable(true);
    create_polytope->setChecked(true);

    connect(create_polytope,
            SIGNAL(triggered()),
            SLOT(onCreatePolytopeTriggered()));

    // create terrain button
    QAction* create_terrain = new QAction("Create Terrain", input_state_buttons);
    create_terrain->setIcon(QIcon("://icons/create_terrain.png"));
    create_terrain->setCheckable(true);

    connect(create_terrain,
            SIGNAL(triggered()),
            SLOT(onCreateTerrainTriggered()));

    // snap to grid button
    QAction* snap_to_grid = new QAction("Snap to Grid", ui->toolBar->layout());
    snap_to_grid->setIcon(QIcon("://icons/snap_to_grid.png"));
    snap_to_grid->setCheckable(true);
    snap_to_grid->setChecked(true);

    connect(snap_to_grid,
            SIGNAL(toggled(bool)),
            SLOT(onSnapToGridToggled(bool)));

    // add buttons to toolbar
    ui->toolBar->addAction(select_objects);
    ui->toolBar->addAction(create_polyline);
    ui->toolBar->addAction(create_polytope);
    ui->toolBar->addAction(create_terrain);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(snap_to_grid);

    // create perspective widget
    //rInfo("Creating perspective view.");
    qDebug() << "Creating perspective view";
    auto perspective = new PerspectiveWidget(ui->group_perspective);
    ui->group_perspective->layout()->addWidget(perspective);
    perspective->installEventFilter(this);
    qDebug() << "persp is sharing? " << perspective->context()->isSharing();
    qDebug() << "persp is valid? " << perspective->context()->isValid();
    //perspective->context()->makeCurrent();
    //qDebug() << perspective->format();


    // create orthographic widget
    //rInfo("Creating orthographic view.");
    qDebug() << "Creating orthographic view";
    auto ortho_top = new OrthographicWidget(TOP, ui->group_top, perspective);
    ui->group_top->layout()->addWidget(ortho_top);
    ortho_top->installEventFilter(this);
    qDebug() << "persp is sharing? " << perspective->context()->isSharing();
    qDebug() << "ortho is sharing?" << ortho_top->context()->isSharing();
    //qDebug() << ortho_top->format();

    perspective->context()->makeCurrent();

    // create renderer
    //rInfo("Creating renderer.");
    qDebug() << "Creating renderer.";
    renderer_ = new Renderer();

    // create scene manager
    //rInfo("Creating scene manager.");
    qDebug() << "Creating scene manager.";
    scene_manager_ = new SceneManager(renderer_);

    connect(this,
            SIGNAL(CreateTerrainMesh(const QVector<QVector3D>&)),
            &scene_manager_->scene_observer_,
            SLOT(onCreateTerrainMesh(const QVector<QVector3D>&)));

    // initialize widgets
    qDebug() << "Initializing ortho.";
    ortho_top->initialize(renderer_, scene_manager_);
    qDebug() << "Initializing perspective.";
    perspective->initialize(renderer_, scene_manager_);

    //perspective->context()->makeCurrent();

    /*
    QOpenGLDebugLogger *logger = new QOpenGLDebugLogger(this);
    logger->initialize();
    connect(logger,
            &QOpenGLDebugLogger::messageLogged,
            this,
            &MainWindow::onLogMessage);
    logger->startLogging();
    */

    connect(ortho_top,
            SIGNAL(ChangeMessage(const QString&)),
            SLOT(UpdateStatusBarMsg(const QString&)));
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
    delete renderer_;
    delete ui;
}

void MainWindow::onSelectObjectsTriggered() {
    ConfigManager::get().set_input_state(SELECT);
}

void MainWindow::onCreatePolyLineTriggered() {
    ConfigManager::get().set_input_state(CREATE_POLYLINE);
}

void MainWindow::onCreatePolytopeTriggered() {
    ConfigManager::get().set_input_state(CREATE_POLYTOPE);
}

void MainWindow::onCreateTerrainTriggered() {
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open terrain data"),
        "./",
        tr("Text files (*.txt)")
    );

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        //rInfo("Could not open file %s.", fileName.toUtf8().data());
        return;
    } else {
        //rInfo("Successfully opened file %s.", fileName.toUtf8().data());
    }

    QVector<QVector3D> points;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        auto tokens = line.split(QRegularExpression("\\s+"), QString::SkipEmptyParts);
        qDebug() << tokens << " size: " << tokens.size();
        if(tokens.size() > 3){
        points.push_back(QVector3D(tokens.at(1).toFloat(),
                                   tokens.at(2).toFloat(),
                                   tokens.at(3).toFloat()));
        }
        if(tokens.size() == 3){
        points.push_back(QVector3D(tokens.at(0).toFloat(),
                                   tokens.at(1).toFloat(),
                                   tokens.at(2).toFloat()));
        }
        //qDebug() << points.back();
    }

    emit CreateTerrainMesh(points);
}

void MainWindow::onSnapToGridToggled(bool state) {
    ConfigManager::get().set_snap_to_grid(state);
}

void MainWindow::initializeLogging() {
    /*
    logger_.set_console(ui->console);
    logger_.subscribeTo(rlog::GetGlobalChannel("info"));
    logger_.subscribeTo(rlog::GetGlobalChannel("debug"));
    logger_.subscribeTo(rlog::GetGlobalChannel("error"));
    */
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

void MainWindow::on_action_about_triggered() {
    AboutDialog *a = new AboutDialog(this);
    a->addBuildDate();
    a->show();
}

void MainWindow::on_action_preferences_triggered() {
    PreferencesDialog *p = new PreferencesDialog(this);
    p->show();
}

void MainWindow::UpdateStatusBarMsg(const QString &status) {
    ui->statusbar_main->showMessage(status);
}

void MainWindow::on_actionUser_Manual_triggered() {
    QDesktopServices::openUrl(QUrl("file:///C:/WorkbenchUserManual.pdf"));
}
