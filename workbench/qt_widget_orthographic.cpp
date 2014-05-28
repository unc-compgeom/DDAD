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
 * @date 2013-01-29
 */

// RationalCAD
#include "common.h"
#include "qt_window_main.h"
#include "qt_widget_orthographic.h"
#include "grid.h"
#include "config.h"

// geometry
#include "../geometry/point.h"
#include "../geometry/vector.h"

using namespace DDAD;
using namespace DDAD::Visual;

const int OrthographicWidget::kRedrawMsec = 16;
const int OrthographicWidget::kMinHintWidth = 64;
const int OrthographicWidget::kMinHintHeight = 64;
const int OrthographicWidget::kPrefHintWidth = 512;
const int OrthographicWidget::kPrefHintHeight = 384;

//=============================================================================
// Constructors / Destructors
//=============================================================================

OrthographicWidget::OrthographicWidget(OrthoOrientation orientation,
                                       QWidget *parent,
                                       const QGLWidget *shareWidget) :
    QGLWidget(parent, shareWidget),
    orientation_(orientation),
    num_frames_(0) {}

//=============================================================================
// Initialization
//=============================================================================

void OrthographicWidget::initialize(Renderer* renderer,
    SceneManager* scene_manager) {
    renderer_ = renderer;
    scene_manager_ = scene_manager;

    setAutoFillBackground(false);
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this,
            SIGNAL(customContextMenuRequested(const QPoint&)),
            this,
            SLOT(ShowContextMenu(const QPoint&)));
    connect(this,
            SIGNAL(SelectObject(QVector2D)),
            &scene_manager_->scene_observer_,
            SLOT(onSelectObject(QVector2D)));
    connect(this,
            SIGNAL(BeginCreatePolytope(QVector2D, QVector2D)),
            &scene_manager_->scene_observer_,
            SLOT(onBeginCreatePolytope(QVector2D, QVector2D)));
    connect(this,
            SIGNAL(UpdateNewPolytope(QVector2D)),
            &scene_manager_->scene_observer_,
            SLOT(onUpdateNewPolytope(QVector2D)));
    connect(this,
            SIGNAL(EndCreatePolytope()),
            &scene_manager_->scene_observer_,
            SLOT(onEndCreatePolytope()));
}

void OrthographicWidget::initializeGL() {
    qDebug() << "Initializing orthographic OpenGL.";

    initializeOpenGLFunctions();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_CULL_FACE);
    glEnable(GL_PROGRAM_POINT_SIZE);

    modelview_.setToIdentity();

    QVector<GL::AttributeMeta> attributes;
    attributes.push_back(GL::Vertex::kPositionMeta);
    attributes.push_back(GL::Vertex::kMatAmbientMeta);
    i_grid_rg_.InitCommon(
        ":shaders/mat_unlit_opaque.vsh",
        ":shaders/mat_unlit_opaque.fsh",
        attributes
    );
    i_grid_rg_.InitContext(GL::Context::eORTHOGRAPHIC);
    QVector<GL::Vertex> grid_verts;
    i_grid_.InitializeGrid(8, 8, 16, 16, grid_verts);
    i_grid_rg_.UploadVertices(GL::Primitive::eLINES, grid_verts);

    renderer_->InitContext(GL::Context::eORTHOGRAPHIC);

    timer_.setTimerType(Qt::PreciseTimer);
    connect(&timer_, SIGNAL(timeout()), this, SLOT(update()));
    timer_.start(kRedrawMsec);
}

//=============================================================================
// Rendering
//=============================================================================

void OrthographicWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    makeCurrent();

    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawGrid();
    drawScene();

    draw2DOverlay();
}

void OrthographicWidget::drawGrid() {
    // setup modelview
    QMatrix4x4 mv;
    mv.scale(i_grid_.local_scale());
    mv.translate(-i_grid_.local_pos().x(), -i_grid_.local_pos().y());

    i_grid_rg_.BindContextPrimitive(GL::Context::eORTHOGRAPHIC,
                                    GL::Primitive::eLINES);
    i_grid_rg_.program_[GL::Context::eORTHOGRAPHIC].setUniformValue("m_modelview", mv);
    glDrawArrays(GL_LINES, 0, i_grid_rg_.NumVertices(GL::Primitive::eLINES));
    i_grid_rg_.ReleaseContextPrimitive(GL::Context::eORTHOGRAPHIC,
                                       GL::Primitive::eLINES);
}

void OrthographicWidget::drawScene() {
    // restore gl state
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // setup modelview
    modelview_.setToIdentity();

    switch (orientation_) {
    case RIGHT:
        modelview_.rotate(-90.0f, 0.0f, 0.0f, 1.0f);
        modelview_.rotate(-90.0f, 0.0f, 1.0f, 0.0f);
        break;
    case FRONT:
        modelview_.rotate(-90.0f, 1.0f, 0.0f, 0.0f);
    default:
        break;
    }

    QVector3D position(-i_grid_.global_pos().x(),
                       -i_grid_.global_pos().y(), 0.0f);

    modelview_.scale(i_grid_.global_scale());
    modelview_.translate(position);

    auto& rg = renderer_->render_groups_[Coverage::eOPAQUE][Lighting::eUNLIT];
    rg.BindContextPrimitive(GL::Context::eORTHOGRAPHIC, GL::Primitive::ePOINTS);
    rg.program_[GL::Context::eORTHOGRAPHIC].setUniformValue("m_modelview", modelview_);
    glDrawArrays(GL_POINTS, 0, rg.NumVertices(GL::Primitive::ePOINTS));
    rg.ReleaseContextPrimitive(GL::Context::eORTHOGRAPHIC, GL::Primitive::ePOINTS);
    rg.BindContextPrimitive(GL::Context::eORTHOGRAPHIC, GL::Primitive::eLINES);
    glDrawArrays(GL_LINES, 0, rg.NumVertices(GL::Primitive::eLINES));
    rg.ReleaseContextPrimitive(GL::Context::eORTHOGRAPHIC, GL::Primitive::eLINES);
}

void OrthographicWidget::draw2DOverlay() {
    // setup the QPainter for drawing the overlay (e.g. 2D text)
    QPainter painter;
    painter.begin(this);
    painter.setFont(QFont("Segoe UI", 8, QFont::Bold));

    // label x coordinates
    QVector<QPair<int, int>> x_coords = i_grid_.GetMajorXCoords(width());
    for (int i = 0; i < x_coords.size(); ++i) {
        painter.drawText(x_coords.at(i).first+2, 12,
                         QString::number(x_coords.at(i).second));
    }

    // label y coordinates
    QVector<QPair<int, int>> y_coords = i_grid_.GetMajorYCoords(height());
    for (int i = 0; i < y_coords.size(); ++i) {
        int convertedY = height()-y_coords.at(i).first;
        painter.drawText(6, convertedY-2,
                         QString::number(y_coords.at(i).second));
    }

    // clean up
    painter.end();
}

void OrthographicWidget::resizeGL(int width, int height) {
    glViewport(0, 0, width, height);

    float halfWidth  = static_cast<float>(width)/2.0f;
    float halfHeight = static_cast<float>(height)/2.0f;

    projection_.setToIdentity();
    projection_.ortho(-halfWidth, halfWidth, -halfHeight, halfHeight,
                      -8192.0f*8, 8192.0f*8);

    i_grid_rg_.program_[GL::Context::eORTHOGRAPHIC].bind();
    i_grid_rg_.program_[GL::Context::eORTHOGRAPHIC].setUniformValue("m_projection", projection_);
    i_grid_rg_.program_[GL::Context::eORTHOGRAPHIC].release();
    auto rg = &renderer_->render_groups_[Coverage::eOPAQUE][Lighting::eUNLIT];
    rg->program_[GL::Context::eORTHOGRAPHIC].bind();
    rg->program_[GL::Context::eORTHOGRAPHIC].setUniformValue("m_projection", projection_);
    rg->program_[GL::Context::eORTHOGRAPHIC].release();
}

//=============================================================================
// Input handlers: mouse, keyboard, and timer events
//=============================================================================

void OrthographicWidget::timerEvent(QTimerEvent *event) {
    Q_UNUSED(event);
    update();
}

static QPoint last_click_pos;
static QPoint create_polytope_pos;

QVector2D OrthographicWidget::mousePressToWorld(const QPoint& pos) const {
    int converted_y = height()-pos.y();
    qreal halfwidth = static_cast<qreal>(width())/2;
    qreal halfheight = static_cast<qreal>(height())/2;
    QVector2D screen_coords(pos.x()-halfwidth, converted_y-halfheight);
    return i_grid_.GetCoordsForPixel(screen_coords);
}

void OrthographicWidget::mousePressEvent(QMouseEvent *event) {
    int convertedY = height()-event->y();
    QVector2D world_coords = mousePressToWorld(event->pos());

    if (event->buttons() & Qt::LeftButton) {
        switch (ConfigManager::get().input_state()) {
        case CREATE_POLYTOPE:
            /* need to save initial click position to determine where to begin
             * creating the polytope. there are two cases:
             *
             * snapping is off. we can either save the screen coords or save
             * the world coords. then we check inside of mouse move event to see
             * if the user has dragged some "far enough" distance while holding
             * down the lmb. far enough is some arbitrary value when snapping is
             * off, and it would depend upon whether we are doing it in screen
             * coords or in world coords. it may be better to do it in screen
             * coords so that we don't create objects "too easily" when a user
             * is zoomed out.
             *
             * snapping is on.
             *
             */
            //qDebug() << "mousePressEvent";
            create_polytope_pos = event->pos();

            break;
        default:
            break;
        }
    }

    if (event->buttons() & Qt::MiddleButton) {
        setCursor(Qt::ClosedHandCursor);
        last_click_pos.setX(event->x());
        last_click_pos.setY(convertedY);
    }

    if (event->buttons() & Qt::RightButton) {
        switch (ConfigManager::get().input_state()) {
        case SELECT:
        default:
            break;
        }
    }
}

void OrthographicWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (!(event->buttons() & Qt::MiddleButton)) {
        setCursor(Qt::ArrowCursor);
    }

    if (!(event->buttons() & Qt::LeftButton)) {
        switch (ConfigManager::get().input_state()) {
        case UPDATE_POLYTOPE:
            emit EndCreatePolytope();
            ConfigManager::get().set_input_state(CREATE_POLYTOPE);
            break;
        default:
            break;
        }
    }
}

void OrthographicWidget::mouseMoveEvent(QMouseEvent *event) {
    int convertedY = height()-event->y();

    QVector2D delta(last_click_pos.x()-event->x(),
                    last_click_pos.y()-convertedY);

    QVector2D cpoly_world = mousePressToWorld(create_polytope_pos);
    QVector2D cpoly_world_snapped(i_grid_.RoundToNearestMinor(cpoly_world.x()),
                                  i_grid_.RoundToNearestMinor(cpoly_world.y()));

    QVector2D cur_world = mousePressToWorld(event->pos());
    QVector2D cur_world_snapped(i_grid_.RoundToNearestMinor(cur_world.x()),
                                i_grid_.RoundToNearestMinor(cur_world.y()));

    if (event->buttons() & Qt::LeftButton) {
        switch (ConfigManager::get().input_state()) {
        case CREATE_POLYTOPE:
            if (ConfigManager::get().snap_to_grid()) {
                if (cur_world_snapped.x() != cpoly_world_snapped.x() &&
                    cur_world_snapped.y() != cpoly_world_snapped.y()) {
                    emit BeginCreatePolytope(cpoly_world_snapped,
                                             cur_world_snapped);
                    ConfigManager::get().set_input_state(UPDATE_POLYTOPE);
                }
            } else {

            }
            break;
        case UPDATE_POLYTOPE:
            if (ConfigManager::get().snap_to_grid()) {
                emit UpdateNewPolytope(cur_world_snapped);
            } else {

            }
            break;
        default:
            break;
        }
    }

    if (event->buttons() & Qt::MiddleButton) {
        i_grid_.Translate(delta);
    }

    last_click_pos.setX(event->x());
    last_click_pos.setY(convertedY);
    QCoreApplication::processEvents();
}

void OrthographicWidget::wheelEvent(QWheelEvent *event) {
    if (event->delta() > 0) {
        i_grid_.IncreaseMagnification();   // forward, away from user, zoom in
    } else if (event->delta() < 0) {
        i_grid_.DecreaseMagnification();   // backward, toward user, zoom out
    }
}

void OrthographicWidget::ShowContextMenu(const QPoint &p) {
    /*
    if(g_config.input_state_ != SELECT)  {
        return;
    }

    QPoint gp = mapToGlobal(p);

    QMenu ctxt_menu;
    ctxt_menu.addAction("Import mesh here...");

    QAction* selected_item = ctxt_menu.exec(gp);
    if(selected_item) {
        QString selected_file = QFileDialog::getOpenFileName(
                    this, "Import Mesh", "./",
                    "3ds Max 3DS (*.3ds);;"
                    "ASCII Scene Export (*.ase);;"
                    "Collada (*.dae);;"
                    "Wavefront Object (*.obj)");
        //Assimp::Importer importer;

        const aiScene* scene = importer.ReadFile(
                    selected_file.toStdString(),
                    aiProcess_Triangulate            |
                    aiProcess_JoinIdenticalVertices);

        if(!scene) {
            rDebug("error");
        }
    } else {

    }
    */
}

void OrthographicWidget::keyPressEvent(QKeyEvent *event) {
    key_states_[event->key()] = true;
}

void OrthographicWidget::keyReleaseEvent(QKeyEvent *event) {
    key_states_[event->key()] = false;
}

//=============================================================================
// Misc overrides / OpenGL support routines
//=============================================================================

QSize OrthographicWidget::minimumSizeHint() const {
    return QSize(kMinHintWidth, kMinHintHeight);
}

QSize OrthographicWidget::sizeHint() const {
    return QSize(kPrefHintWidth, kPrefHintHeight);
}
