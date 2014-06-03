/*!
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 2013-06-11
 */

// Workbench
#include "common.h"
#include "scene.h"
#include "config.h"

// geometry geometry
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

SceneObserver::~SceneObserver() {}

void SceneObserver::GenerateVboPoints() {
    QVector<GL::Vertex> points;
    for (auto i = viz_points_.begin(); i != viz_points_.end(); ++i) {
        GL::Vertex v(approx_points_.value(i.key())->approx());
        v.set_mat_ambient(i->back().material().ambient());
        /*
        qDebug() << i->back().material().ambient().r() << " "
                 << i->back().material().ambient().g() << " "
                 << i->back().material().ambient().b() << " "
                 << i->back().material().ambient().a(); */
        points.push_back(v);
    }
    emit UpdateVertexBuffer(Visual::Coverage::eOPAQUE,
                            Visual::Lighting::eUNLIT,
                            GL::Primitive::ePOINTS, points);
    //qDebug() << "updatevertexbuffer emitted";
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
    emit UpdateVertexBuffer(Visual::Coverage::eOPAQUE,
                            Visual::Lighting::eUNLIT,
                            GL::Primitive::eLINES, lines);
    //emit UpdateVboLines(lines);
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
    emit UpdateVertexBuffer(Visual::Coverage::eOPAQUE,
                            Visual::Lighting::eFLAT,
                            GL::Primitive::eTRIANGLES, triangles);
    //emit UpdateVboTriangles(triangles);
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
    GenerateVboPoints();
    thread()->msleep(msec_delay);
    QApplication::processEvents();
}

void SceneObserver::SlotPopVisualPoint_2r(const Point_2r& p,
                                          const uint32_t msec_delay) {
    //rAssert(!viz_points_.value(p.unique_id()).isEmpty());
    viz_points_[p.unique_id()].pop_back();

    if (viz_points_.value(p.unique_id()).isEmpty()) {
        viz_points_.remove(p.unique_id());
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
    //rAssert(!viz_segments_.value(key).isEmpty());
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
    //rAssert(!viz_points_.value(p.unique_id()).isEmpty());
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
    //rAssert(!viz_segments_.value(key).isEmpty());
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
// Polytope_3 management
//=============================================================================

void SceneObserver::onBeginCreatePolytope(const QVector2D& start,
                                          const QVector2D& cur) {
    //qDebug() << "SceneObserver BeginCreatePolytope " << start << ", " << cur;

    onDeselect();

    static int numPolytopes = 0;
    selected_name_ = QString("polytope3_%1").arg(numPolytopes++);
    scene_objects_.insert(selected_name_,
                          QSharedPointer<ISceneObject>(new ScenePolytope_3()));
    SelectedPolytope_3()->AddObserver(this);
    SelectedPolytope_3()->Initialize(start, cur);
}

void SceneObserver::onUpdateNewPolytope(const QVector2D& cur) {
    //qDebug() << "SceneObserver UpdateNewPolytope " << cur;
    SelectedPolytope_3()->Update(cur);
}

void SceneObserver::onEndCreatePolytope() {
    //qDebug() << "SceneObserver EndCreatePolytope";
    //ConfigManager::get().set_input_state(CREATE_POLYTOPE);
    //SelectedPolytope_3()->Update();
}

//=============================================================================
// TerrainMesh_3 management
//=============================================================================
//#include <ctime>
void SceneObserver::onCreateTerrainMesh(const QVector<QVector3D>& data) {

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
    if (!ObjectIsSelected()) {
        return;
    }

    QSharedPointer<ISceneObject> obj_ptr(SelectedObject());
    //scene_objects_.remove(selected_name_);
    selected_name_ = name;
    //scene_objects_.insert(selected_name_, obj_ptr);
}

void SceneObserver::onUpdateSelectedObjectColor(const QColor &color) {
    if (!ObjectIsSelected()) {
        return;
    }

    SelectedObject()->UpdateColor(color);
}

void SceneObserver::onDeleteSelectedObject() {
    if (!ObjectIsSelected()) {
        return;
    }

    //scene_objects_.remove(selected_name_);
    //selected_name_ = "";
}

void SceneObserver::onSelectObject(const QVector2D& coords) {
    onDeselect();
}

void SceneObserver::onDeselect() {
    if (!ObjectIsSelected()) {
        return;
    }

    SelectedObject()->Deselect();
    selected_name_ = "";
}

int SceneObserver::NumObjects() const {
    return 0;
}

ISceneObject* SceneObserver::SelectedObject() {
    return scene_objects_.value(selected_name_).data();
}

ScenePolytope_3* SceneObserver::SelectedPolytope_3() {
    return dynamic_cast<ScenePolytope_3*>(SelectedObject());
}

SceneTerrainMesh_3* SceneObserver::SelectedTerrainMesh_3() {
    return dynamic_cast<SceneTerrainMesh_3*>(SelectedObject());
}

bool SceneObserver::ObjectIsSelected() const {
    return selected_name_ != "";
}

const QString& SceneObserver::selected_name() const {
    return selected_name_;
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

Point_2f ToPoint_2f(const QVector2D &v, bool snap) {
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
