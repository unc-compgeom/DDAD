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
#include "scene.h"
#include "config.h"

// geometry kernel
#include "../geometry/common.h"
#include "../geometry/arithmetic.h"
#include "../geometry/vector.h"
#include "../geometry/polygon.h"
#include "../geometry/matrix.h"
#include "../geometry/line.h"
#include "../geometry/intersection.h"

#include <ctime>

namespace DDAD {

Point_2f ToPoint_2f(const QVector2D& v, bool snap = false);

//=============================================================================
// Implementation: SceneObserver
//=============================================================================

SceneObserver::SceneObserver() :
    cur_point_uid_(1) {}

SceneObserver::~SceneObserver() {
    for (auto object : selected_objects_) {
        object->Deselect();
    }
    selected_objects_.clear();
    scene_objects_.clear();
}

void SceneObserver::GenerateVboPoints() {
    QVector<GL::Vertex> points;
    for (auto i = viz_points_.begin(); i != viz_points_.end(); ++i) {
        GL::Vertex v(approx_points_.value(i.key())->approx());
        v.set_mat_ambient(i->back().material().ambient());
        points.push_back(v);
    }
    emit UpdateVertexBuffer(Visual::Coverage::E_OPAQUE,
                            Visual::Lighting::E_UNLIT,
                            GL::Primitive::E_POINTS, points);
}

void SceneObserver::GenerateVboLines() {
    QVector<GL::Vertex> lines;
    for (auto i = viz_segments_.begin(); i != viz_segments_.end(); ++i) {
        GL::Vertex p(approx_points_.value(i.key().first)->approx());
        GL::Vertex q(approx_points_.value(i.key().second)->approx());
        p.set_mat_ambient(i->back().material().ambient());
        q.set_mat_ambient(i->back().material().ambient());
        lines.push_back(p);
        lines.push_back(q);
    }
    emit UpdateVertexBuffer(Visual::Coverage::E_OPAQUE,
                            Visual::Lighting::E_UNLIT,
                            GL::Primitive::E_LINES, lines);
}

void SceneObserver::GenerateVboTriangles() {
    QVector<GL::Vertex> triangles;
    for (auto i = viz_triangles_.begin(); i != viz_triangles_.end(); ++i) {
        auto current_vt = i.value().top();
        Point_3f ap = approx_points_.value(i.key().at(0))->approx();
        Point_3f bp = approx_points_.value(i.key().at(1))->approx();
        Point_3f cp = approx_points_.value(i.key().at(2))->approx();
        Vector_3f v0 = bp-ap;
        Vector_3f v1 = cp-ap;
        Vector_3f n = Normalized(Cross(v0, v1));
        GL::Vertex a(ap);
        GL::Vertex b(bp);
        GL::Vertex c(cp);
        a.set_mat_ambient(current_vt.material().ambient());
        a.set_normal(n);
        b.set_mat_ambient(current_vt.material().ambient());
        b.set_normal(n);
        c.set_mat_ambient(current_vt.material().ambient());
        c.set_normal(n);
        triangles.push_back(a);
        triangles.push_back(b);
        triangles.push_back(c);
    }
    emit UpdateVertexBuffer(Visual::Coverage::E_OPAQUE,
                            Visual::Lighting::E_FLAT,
                            GL::Primitive::E_TRIANGLES, triangles);
}

void SceneObserver::SlotRegisterPoint_2r(Point_2r &p) {
    if (!p.unique_id()) {
        p.set_unique_id(cur_point_uid_++);
    }
    if (!approx_points_.contains(p.unique_id())) {
        auto approx = QSharedPointer<ApproxPoint_3f>(new ApproxPoint_3f(p));
        approx_points_.insert(p.unique_id(), approx);
        p.AddObserver(approx.data());
    }
}

void SceneObserver::SlotPushVisualPoint_2r(const Point_2r& p,
                                           const Visual::Point& vp,
                                           const uint32_t msec_delay) {
    viz_points_[p.unique_id()].push_back(vp);
    approx_points_[p.unique_id()]->set_z_order(vp.z_order());
    GenerateVboPoints();
    thread()->msleep(msec_delay);
    QApplication::processEvents();
}

void SceneObserver::SlotPopVisualPoint_2r(const Point_2r& p,
                                          const uint32_t msec_delay) {
    viz_points_[p.unique_id()].pop_back();

    if (viz_points_.value(p.unique_id()).isEmpty()) {
        viz_points_.remove(p.unique_id());
    } else {
        approx_points_[p.unique_id()]->set_z_order(viz_points_[p.unique_id()].back().z_order());
    }

    GenerateVboPoints();
    thread()->msleep(msec_delay);
    QApplication::processEvents();
}

void SceneObserver::SlotPushVisualSegment_2r(const Segment_2r& s,
                                             const Visual::Segment& vs,
                                             const uint32_t msec_delay) {
    QPair<uint32_t, uint32_t> key(s.p().unique_id(), s.q().unique_id());
    viz_segments_[key].push_back(vs);
    GenerateVboLines();
    thread()->msleep(msec_delay);
    QApplication::processEvents();
}

void SceneObserver::SlotPopVisualSegment_2r(const Segment_2r& s,
                                            const uint32_t msec_delay) {
    QPair<uint32_t, uint32_t> key(s.p().unique_id(), s.q().unique_id());
    viz_segments_[key].pop_back();

    if (viz_segments_.value(key).isEmpty()) {
        viz_segments_.remove(key);
    }

    GenerateVboLines();
    thread()->msleep(msec_delay);
    QApplication::processEvents();
}

void SceneObserver::SlotPushVisualTriangle_2r(const Triangle_2r &t,
                                              const Visual::Triangle& vt,
                                              const uint32_t msec_delay) {
    QVector<uint32_t> key;
    key.push_back(t.a().unique_id());
    key.push_back(t.b().unique_id());
    key.push_back(t.c().unique_id());

    viz_triangles_[key].push_back(vt);

    GenerateVboTriangles();
    thread()->msleep(msec_delay);
    QApplication::processEvents();
}

void SceneObserver::SlotPopVisualTriangle_2r(const Triangle_2r &t,
                                             const uint32_t msec_delay) {
    QVector<uint32_t> key;
    key.push_back(t.a().unique_id());
    key.push_back(t.b().unique_id());
    key.push_back(t.c().unique_id());

    viz_triangles_[key].pop_back();

    if (viz_triangles_.value(key).isEmpty()) {
        viz_triangles_.remove(key);
    }

    GenerateVboTriangles();
    thread()->msleep(msec_delay);
    QApplication::processEvents();
}

void SceneObserver::SlotRegisterPoint_3r(Point_3r &p) {
    if (!p.unique_id()) {
        p.set_unique_id(cur_point_uid_++);
    }
    if (!approx_points_.contains(p.unique_id())) {
        auto approx = QSharedPointer<ApproxPoint_3f>(new ApproxPoint_3f(p));
        approx_points_.insert(p.unique_id(), approx);
        p.AddObserver(approx.data());
    }
}

void SceneObserver::SlotPushVisualPoint_3r(const Point_3r& p,
                                          const Visual::Point& vp,
                                          const uint32_t msec_delay) {
    viz_points_[p.unique_id()].push_back(vp);
    GenerateVboPoints();
    thread()->msleep(msec_delay);
    QApplication::processEvents();
}

void SceneObserver::SlotPopVisualPoint_3r(const Point_3r& p,
                                      const uint32_t msec_delay) {
    viz_points_[p.unique_id()].pop_back();

    if (viz_points_.value(p.unique_id()).isEmpty()) {
        viz_points_.remove(p.unique_id());
    }

    GenerateVboPoints();
    thread()->msleep(msec_delay);
    QApplication::processEvents();
}

void SceneObserver::SlotPushVisualSegment_3r(const Segment_3r& s,
                                             const Visual::Segment& vs,
                                             const uint32_t msec_delay) {
    QPair<uint32_t, uint32_t> key(s.p().unique_id(), s.q().unique_id());
    viz_segments_[key].push_back(vs);
    GenerateVboLines();
    thread()->msleep(msec_delay);
    QApplication::processEvents();
}

void SceneObserver::SlotPopVisualSegment_3r(const Segment_3r& s,
                                            const uint32_t msec_delay) {
    QPair<uint32_t, uint32_t> key(s.p().unique_id(), s.q().unique_id());
    viz_segments_[key].pop_back();

    if (viz_segments_.value(key).isEmpty()) {
        viz_segments_.remove(key);
    }

    GenerateVboLines();
    thread()->msleep(msec_delay);
    QApplication::processEvents();
}

void SceneObserver::SlotPushVisualTriangle_3r(const Triangle_3r &t,
                                              const Visual::Triangle& vt,
                                              const uint32_t msec_delay) {
    QVector<uint32_t> key;
    key.push_back(t.a().unique_id());
    key.push_back(t.b().unique_id());
    key.push_back(t.c().unique_id());

    viz_triangles_[key].push_back(vt);

    GenerateVboTriangles();
    thread()->msleep(msec_delay);
    QApplication::processEvents();
}

void SceneObserver::SlotPopVisualTriangle_3r(const Triangle_3r &t,
                                             const uint32_t msec_delay) {
    QVector<uint32_t> key;
    key.push_back(t.a().unique_id());
    key.push_back(t.b().unique_id());
    key.push_back(t.c().unique_id());

    viz_triangles_[key].pop_back();

    if (viz_triangles_.value(key).isEmpty()) {
        viz_triangles_.remove(key);
    }

    GenerateVboTriangles();
    thread()->msleep(msec_delay);
    QApplication::processEvents();
}

void SceneObserver::SlotUpdate() {
    GenerateVboPoints();
    GenerateVboLines();
    GenerateVboTriangles();
}

//=============================================================================
// Polyline_2 management
//=============================================================================

void SceneObserver::onBeginCreatePolyline(const QVector2D& v) {
    LOG(DEBUG) << "onBeginCreatePolyline";

    onDeselect();

    static int numPolylines = 0;

    auto name = QString("polyline2_%1").arg(numPolylines++);
    auto polyline = QSharedPointer<ISceneObject>(new ScenePolyline_2());
    polyline->set_name(name);
    scene_objects_.insert(name, polyline);
    selected_objects_.push_back(polyline);
    SelectedPolyline_2()->AddObserver(this);
    SelectedPolyline_2()->Initialize(v);
    ConfigManager::get().set_input_state(InputState::UPDATE_POLYLINE);
}

void SceneObserver::onUpdateNewPolyline(const QVector2D& v) {
    SelectedPolyline_2()->Update(v);
}

void SceneObserver::onEndCreatePolyline() {
    SelectedPolyline_2()->Select();
    ConfigManager::get().set_input_state(InputState::CREATE_POLYLINE);
    emit UpdateContextSensitiveMenus(SelectedObject()->scene_object_type(),
                                     SelectedObject()->name());
}

void SceneObserver::onExecuteMelkman() {
    // TODO: capture resulting polygon to create scene object
    DDAD::Melkman(SelectedPolyline_2()->model_polyline(), this);
}

void SceneObserver::onComputeMelkmanForSelectedPolyline() {
    // TODO: capture resulting polygon to create scene object
    DDAD::Melkman(SelectedPolyline_2()->model_polyline(), this);
}

//=============================================================================
// Polytope_3 management
//=============================================================================

void SceneObserver::onBeginCreatePolytope(const QVector2D& start,
                                          const QVector2D& cur) {
    LOG(DEBUG) << "onBeginCreatePolytope";

    onDeselect();

    static int numPolytopes = 0;
    auto name = QString("polytope3_%1").arg(numPolytopes++);
    auto polytope = QSharedPointer<ISceneObject>(new ScenePolytope_3());
    polytope->set_name(name);
    scene_objects_.insert(name, polytope);
    selected_objects_.push_back(polytope);
    SelectedPolytope_3()->AddObserver(this);
    SelectedPolytope_3()->Initialize(start, cur);
}

void SceneObserver::onUpdateNewPolytope(const QVector2D& cur) {
    SelectedPolytope_3()->Update(cur);
}

void SceneObserver::onEndCreatePolytope() {
    SelectedPolytope_3()->Select();
    emit UpdateContextSensitiveMenus(SelectedObject()->scene_object_type(),
                                     SelectedObject()->name());
}

//=============================================================================
// PointSet_3 management
//=============================================================================

void SceneObserver::onBeginCreatePointSet(const QVector2D &cur) {
    LOG(DEBUG) << "scene observer creating new point set";

    onDeselect();

    static int numPointSets = 0;

    auto name = QString("pointset3_%1").arg(numPointSets++);
    auto pointset = QSharedPointer<ISceneObject>(new ScenePointSet_3());
    pointset->set_name(name);
    scene_objects_.insert(name, pointset);
    selected_objects_.push_back(pointset);
    SelectedPointSet_3()->AddObserver(this);
    SelectedPointSet_3()->Initialize(cur);
    ConfigManager::get().set_input_state(InputState::UPDATE_POINTSET);
}

void SceneObserver::onUpdateNewPointSet(const QVector2D &cur) {
    SelectedPointSet_3()->Update(cur);
}

void SceneObserver::onEndCreatePointSet() {
    SelectedPointSet_3()->Select();
    ConfigManager::get().set_input_state(InputState::CREATE_POINTSET);
    emit UpdateContextSensitiveMenus(SelectedObject()->scene_object_type(),
                                     SelectedObject()->name());
}

void SceneObserver::onCreatePointSet(const QVector<QVector3D>& data) {
    LOG(DEBUG) << "onCreatePointSet";

    onDeselect();

    static int numPointSets = 0;
    auto name = QString("pointset3_%1").arg(numPointSets++);
    auto pointset = QSharedPointer<ISceneObject>(new ScenePointSet_3());
    pointset->set_name(name);
    scene_objects_.insert(name, pointset);
    selected_objects_.push_back(pointset);
    SelectedPointSet_3()->AddObserver(this);
    SelectedPointSet_3()->Initialize(data);

    // TODO: should probably have "onEndCreatePointSet" to match others
    // for now just doing select call here
    SelectedPointSet_3()->Select();
    emit UpdateContextSensitiveMenus(SelectedObject()->scene_object_type(),
                                     SelectedObject()->name());
}

void SceneObserver::onComputeTerrainMeshForSelectedPointSet() {
    LOG(DEBUG) << "computing terrain mesh for selected point set...";

    DDAD::DelaunayTerrain(SelectedPointSet_3()->model_point_set(), this);
}

//=============================================================================
// TerrainMesh_3 management
//=============================================================================

void SceneObserver::onCreateTerrainMesh(const QVector<QVector3D>& data) {

    /*
    onDeselect();

    std::vector<Point_3f> points;
    foreach(QVector3D p, data) {
        points.push_back(Point_3f(p.x(), p.y(), p.z()));
    }

    std::srand ( unsigned ( std::time(0) ) );
    std::random_shuffle ( points.begin(), points.end() );

    static int numTerrain = 0;
    selected_name_ = QString("terrain3_%1").arg(numTerrain++);
    scene_objects_.insert(selected_name_,
                          QSharedPointer<ISceneObject>(new SceneTerrainMesh_3()));
    SelectedTerrainMesh_3()->AddObserver(this);
    SelectedTerrainMesh_3()->Initialize(points);
    */
}

/*
void SceneObserver::onUpdateNewPolytope(const QVector2D& cur) {
    //qDebug() << "SceneObserver UpdateNewPolytope " << cur;
    SelectedPolytope_3()->Update(cur);
}

void SceneObserver::onEndCreatePolytope() {
    //qDebug() << "SceneObserver EndCreatePolytope";
    //ConfigManager::get().set_input_state(CREATE_POLYTOPE);
    //SelectedPolytope_3()->Update();
}
*/

//=============================================================================
// Object management
//=============================================================================

void SceneObserver::onUpdateSelectedObjectName(const QString &name) {
    /*
    if (!ObjectIsSelected()) {
        return;
    }

    QSharedPointer<ISceneObject> obj_ptr(SelectedObject());
    //scene_objects_.remove(selected_name_);
    //selected_name_ = name;
    //scene_objects_.insert(selected_name_, obj_ptr);
    */
}

void SceneObserver::onUpdateSelectedObjectColor(const QColor &color) {
    /*
    if (!ObjectIsSelected()) {
        return;
    }

    SelectedObject()->UpdateColor(color);
    */
}

// TODO: rename to deleteSelectedObjects
void SceneObserver::onDeleteSelectedObject() {
    LOG(DEBUG) << "onDeleteSelectedObject";

    for (auto object : selected_objects_) {
        object->Deselect();
        LOG(DEBUG) << "removed " << scene_objects_.remove(object->name()) << " objects...";
    }
    selected_objects_.clear();

    emit UpdateContextSensitiveMenus("", "");
}

/*
 * Selection has a variety of issues to address:
 * - when we are clicking in the ortho view, conceptually we are shooting a
 *   ray from infinity down to the XY plane (or other axis-aligned planes if
 *   we generalize it in the future.) This is tricky to represent using only
 *   the Ray_3r class.
 * - when we are clicking in the perspective view, conceptually we are shooting
 *   a ray from the camera toward the view direction. This is straightforward
 *   to represent as a simple Ray_3r.
 * - while it would be nice to use the same function with a single input type,
 *   it makes more sense to just separate the two and handle the orthographic
 *   selection ray in a "reasonable" fashion. for example, we could take as
 *   input the xy world coords of the ortho click. we can compute a ray as follows.
 *   we know the direction is (0, 0, -1). we can set the ray origin as the (x, y)
 *   coords with z equal to the "top" of the AABB of the scene. then we can
 *   forward this newly constructed ray to the same function we use for the
 *   perspective view.
 *
 * TODO: rationals are overkill for selection
 */
void SceneObserver::onSelectObject(const Ray_3r& selection_ray) {
    LOG(DEBUG) << "onSelectObject";

    onDeselect();

    QSharedPointer<ISceneObject> selected_object;
    // TODO: currently using large constant, in future intersect with AABB
    rational closest_isect_time = 999999999;
    for (auto object : scene_objects_) {
        auto isect = object->intersect(selection_ray);
        if (!isect.empty()) {
            if (isect.time() < closest_isect_time) {
                closest_isect_time = isect.time();
                selected_object = object;
            }
        }
    }

    if (selected_object) {
        LOG(DEBUG) << "selected object: " << selected_object->name().toStdString();

        selected_object->Select();
        selected_objects_.push_back(selected_object);

        emit UpdateContextSensitiveMenus(SelectedObject()->scene_object_type(),
                                         SelectedObject()->name());
    } else {
        LOG(DEBUG) << "no object selected.";
        emit UpdateContextSensitiveMenus("", "");
    }
}

void SceneObserver::onSelectObjectFromOrtho(const QVector2D& coords) {
    LOG(DEBUG) << "onSelectObjectFromOrtho";

    // TODO: currently using large constant, in future compute AABB of scene
    auto origin = std::make_shared<Point_3r>(coords.x(), coords.y(), 4096);
    onSelectObject(Ray_3r(origin, Vector_3r(0, 0, -1)));
}

void SceneObserver::onSelectObjectFromPerspective(const QVector3D& origin,
                                                  const QVector3D& dir) {
    LOG(DEBUG) << "onSelectObjectFromPerspective";

    auto shared_origin = std::make_shared<Point_3r>(origin.x(), origin.y(), origin.z());
    onSelectObject(Ray_3r(shared_origin, Vector_3r(dir.x(), dir.y(), dir.z())));
}

void SceneObserver::onDeselect() {
    for (auto object : selected_objects_) {
        object->Deselect();
    }
    selected_objects_.clear();

    emit UpdateContextSensitiveMenus("", "");
}

int SceneObserver::NumObjects() const {
    return 0;
}

ISceneObject* SceneObserver::SelectedObject() {
    assert(selected_objects_.size() == 1);
    return selected_objects_.at(0).data();
}

ScenePointSet_3 *SceneObserver::SelectedPointSet_3() {
    return dynamic_cast<ScenePointSet_3*>(SelectedObject());
}

ScenePolyline_2* SceneObserver::SelectedPolyline_2() {
    return dynamic_cast<ScenePolyline_2*>(SelectedObject());
}

ScenePolytope_3* SceneObserver::SelectedPolytope_3() {
    return dynamic_cast<ScenePolytope_3*>(SelectedObject());
}

SceneTerrainMesh_3* SceneObserver::SelectedTerrainMesh_3() {
    return dynamic_cast<SceneTerrainMesh_3*>(SelectedObject());
}

bool SceneObserver::ObjectIsSelected() const {
    return true;
}

//=============================================================================
// Implementation: SceneManager
//=============================================================================

SceneManager::SceneManager(Renderer* renderer) :
    renderer_(renderer) {
    animation_thread_ = QSharedPointer<QThread>(new QThread());
    scene_observer_.moveToThread(animation_thread_.data());
    animation_thread_->start();

    connect(&scene_observer_,
            SIGNAL(UpdateVertexBuffer(const quint32,
                                      const quint32,
                                      const quint32,
                                      QVector<GL::Vertex>)),
            this,
            SLOT(onUpdateVertexBuffer(const quint32,
                                      const quint32,
                                      const quint32,
                                      QVector<GL::Vertex>)));
}

SceneManager::~SceneManager() {
    animation_thread_->quit();
    animation_thread_->wait();
}

void SceneManager::onUpdateVertexBuffer(const quint32 coverage_idx,
                                        const quint32 lighting_idx,
                                        const quint32 primtype_idx,
                                        QVector<GL::Vertex> verts) {
    renderer_->UpdateRenderGroup(
        static_cast<Visual::Coverage::Type>(coverage_idx),
        static_cast<Visual::Lighting::Type>(lighting_idx),
        static_cast<GL::Primitive::Type>(primtype_idx),
        verts
    );
}

static Point_2f ToPoint_2f(const QVector2D &v, bool snap) {
    return snap ? Point_2f(qRound(v.x()), qRound(v.y())) :
                  Point_2f(v.x(), v.y());
}

//=============================================================================
// ApproximatePoint_3f
//=============================================================================

ApproxPoint_3f::ApproxPoint_3f() {}

ApproxPoint_3f::ApproxPoint_3f(const Point_2r &p) {
    approx_.set_x(p.x().get_d());
    approx_.set_y(p.y().get_d());
    approx_.set_z(0);
}

ApproxPoint_3f::ApproxPoint_3f(const Point_3r &p) {
    approx_.set_x(p.x().get_d());
    approx_.set_y(p.y().get_d());
    approx_.set_z(p.z().get_d());
}

void ApproxPoint_3f::SlotPositionChanged_2r(const Point_2r &p) {
    approx_.set_x(p.x().get_d());
    approx_.set_y(p.y().get_d());
}

void ApproxPoint_3f::SlotPositionChanged_3r(const Point_3r& p) {
    approx_.set_x(p.x().get_d());
    approx_.set_y(p.y().get_d());
    approx_.set_z(p.z().get_d());
}

const Point_3f& ApproxPoint_3f::approx() const {
    return approx_;
}

} // namespace DDAD


/*!
 * Specifies a lexicographical ordering on two vectors of nonnegative integers.
 */
bool operator<(const QVector<uint32_t>& a, const QVector<uint32_t>& b) {
    for (int i = 0; i < qMin(a.size(), b.size()); ++i) {
        if (a[i] < b[i]) {
            return true;
        } else if (a.at(i) > b.at(i)) {
            return false;
        }
    }

    // if we reach here they have to be equal
    if (a.size() == b.size()) {
        return false;
    } else if (a.size() < b.size()) {
        return true;
    } else {
        return false;
    }
}
