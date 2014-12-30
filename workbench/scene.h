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
 * @brief Manager type responsible for storing and manipulating scene objects.
 */

#ifndef WB_SCENE_H
#define WB_SCENE_H

// DDAD
#include "common.h"
#include "config.h"
#include "opengl.h"

// geometry kernel
#include "../geometry/pointset.h"
#include "../geometry/polygon.h"
#include "../geometry/polytope.h"
#include "../geometry/triangulation.h"
#include "../geometry/arithmetic.h"
#include "../geometry/line.h"
#include "../geometry/visual.h"
#include "../geometry/triangle.h"
#include "../geometry/intersection.h"
#include "../geometry/terrain.h"

bool operator<(const QVector<uint32_t>& a, const QVector<uint32_t>& b);

namespace DDAD {

//=============================================================================
// Interface: ApproximatePoint_3f
//=============================================================================

class ApproxPoint_3f : public IPointObserver {
public:
    ApproxPoint_3f();
    ApproxPoint_3f(const Point_2r& p);
    ApproxPoint_3f(const Point_3r& p);

    void SlotPositionChanged_2r(const Point_2r& p) override;
    void SlotPositionChanged_3r(const Point_3r& p) override;
    void set_z_order(const int32_t z_order) { approx_.set_z(z_order); }

    const Point_3f& approx() const;

private:
    Point_3f approx_;
};

//=============================================================================
// Interface: ISceneObject
//=============================================================================

namespace Intersection {

class Ray_3rSceneObject {
public:
    Ray_3rSceneObject() :
        empty_(true),
        time_(0) {}

    Ray_3rSceneObject(bool empty, const rational& time) :
        empty_(empty),
        time_(time) {}


    bool empty() const {
        return empty_;
    }
    const rational& time() const {
        return time_;
    }

private:
    bool empty_;
    rational time_;
};

}

struct ISceneObject {
    virtual ~ISceneObject() {}
    virtual void Select() = 0;
    virtual void Deselect() = 0;
    virtual void UpdateColor(const QColor& color) = 0;
    virtual const QString& name() const = 0;
    virtual void set_name(const QString& name) = 0;
    virtual QString scene_object_type() const = 0;
    virtual Intersection::Ray_3rSceneObject intersect(const Ray_3r& ray) = 0;
};

//=============================================================================
// Interface: ScenePolyline_2
//=============================================================================

class ScenePolyline_2 : public ISceneObject, public Visual::Geometry {
public:
    ScenePolyline_2() {
        model_polyline_.AddObserver(this);
    }

    ~ScenePolyline_2() {
        LOG(DEBUG) << "destroying scene polyline...";
    }

    void Initialize(const QVector2D& start) {
        LOG(DEBUG) << "initializing polyline.";

        model_polyline_.push_back(Point_2r(start.x(), start.y()));
    }

    void Update(const QVector2D& cur) {
        LOG(DEBUG) << "updating polyline.";

        model_polyline_.push_back(Point_2r(cur.x(), cur.y()));
    }

    void Select() override {
        LOG(DEBUG) << "selecting polyline.";

        Visual::Material selected_mat;
        selected_mat.set_ambient(Visual::Color::SKYBLUE);

        SharedPoint_2r last_vertex;
        for (auto vertex : model_polyline_.vertices()) {
            SigPushVisualPoint_2r(*vertex, Visual::Point(selected_mat,
                model_polyline_.z_order()));

            if (last_vertex) {
                SigPushVisualSegment_2r(Segment_2r(last_vertex, vertex),
                                        Visual::Segment(selected_mat));
            }

            last_vertex = vertex;
        }
    }

    void Deselect() override {
        LOG(DEBUG) << "deselecting polyline.";

        SharedPoint_2r last_vertex;
        for (auto vertex : model_polyline_.vertices()) {
            SigPopVisualPoint_2r(*vertex);

            if (last_vertex) {
                SigPopVisualSegment_2r(Segment_2r(last_vertex, vertex));
            }

            last_vertex = vertex;
        }
    }

    Intersection::Ray_3rSceneObject intersect(const Ray_3r &ray) {
        Intersection::Toleranced::Ray_3rPolyline_2r isect(&ray, &model_polyline_);
        return Intersection::Ray_3rSceneObject(isect.type() == Intersection::Toleranced::Ray_3rPolyline_2r::INTERSECTION_EMPTY, isect.time());
    }

    void UpdateColor(const QColor &color) override {}

    QString scene_object_type() const override {
        return "Polyline";
    }

    const QString& name() const override {
        return name_;
    }

    void set_name(const QString &name) override {
        name_ = name;
    }

    const Polyline_2r& model_polyline() const {
        return model_polyline_;
    }

private:
    Polyline_2r model_polyline_;
    QString name_;
};

//=============================================================================
// Interface: ScenePolytope_3
//=============================================================================

class ScenePolytope_3 : public ISceneObject, public Visual::Geometry {
public:
    ScenePolytope_3() {
        model_polytope_.AddObserver(this);
    }

    void Initialize(const QVector2D& start, const QVector2D& cur) {
        model_polytope_.Initialize(Point_3f(start.x(), start.y(), 0),
                                   Point_3f(cur.x(), cur.y(), 8));
    }

    void Update(const QVector2D& cur) {
        model_polytope_.Update(Point_3f(cur.x(), cur.y(), 8));
    }

    void Select() override {
        Visual::Material selected_mat;
        selected_mat.set_ambient(Visual::Color::SKYBLUE);

        QuadEdge::CellVertexIterator cellVerts(model_polytope_.cell());
        QuadEdge::Vertex *v;
        while ((v = cellVerts.next()) != 0) {
            SigPushVisualPoint_3r(*v->pos, Visual::Point(selected_mat));
        }

        QuadEdge::CellFaceIterator cellFaces(model_polytope_.cell());
        QuadEdge::Face *f;
        while ((f = cellFaces.next()) != 0) {
            QuadEdge::FaceEdgeIterator faceEdges(f);
            QuadEdge::Edge *e;

            while ((e = faceEdges.next()) != 0) {
                if (e->Org() < e->Dest()) {
                    SigPushVisualSegment_3r(Segment_3r(e->Org()->pos, e->Dest()->pos),
                                            Visual::Segment(selected_mat));
                }
            }
        }
    }

    void Deselect() override {
        QuadEdge::CellVertexIterator cellVerts(model_polytope_.cell());
        QuadEdge::Vertex *v;
        while ((v = cellVerts.next()) != 0) {
            SigPopVisualPoint_3r(*v->pos);
        }

        QuadEdge::CellFaceIterator cellFaces(model_polytope_.cell());
        QuadEdge::Face *f;
        while ((f = cellFaces.next()) != 0) {
            QuadEdge::FaceEdgeIterator faceEdges(f);
            QuadEdge::Edge *e;

            while ((e = faceEdges.next()) != 0) {
                if (e->Org() < e->Dest()) {
                    SigPopVisualSegment_3r(Segment_3r(e->Org()->pos, e->Dest()->pos));
                }
            }
        }
    }

    Intersection::Ray_3rSceneObject intersect(const Ray_3r &ray) {
        Intersection::Toleranced::Ray_3rPolytope_3r isect(&ray, &model_polytope_);
        return Intersection::Ray_3rSceneObject(isect.type() == Intersection::Toleranced::Ray_3rPolytope_3r::INTERSECTION_EMPTY, isect.time());
    }

    void UpdateColor(const QColor &color) override {}

    QString scene_object_type() const override {
        return "Polytope";
    }

    const QString& name() const override {
        return name_;
    }
    void set_name(const QString &name) override {
        name_ = name;
    }

private:
    Polytope_3r model_polytope_;
    QString name_;
};

//=============================================================================
// Interface: ScenePointSet_3
//=============================================================================

class ScenePointSet_3 : public ISceneObject, public Visual::Geometry {
public:
    ScenePointSet_3() {
        model_point_set_.AddObserver(this);
    }

    void Initialize(const QVector2D& point) {
        model_point_set_.add(Point_3r(point.x(), point.y(), 0));
    }

    void Update(const QVector2D& point) {
        model_point_set_.add(Point_3r(point.x(), point.y(), 0));
    }

    void Initialize(const QVector<QVector3D>& data) {
        for (auto point : data) {
            model_point_set_.add(Point_3r(point.x(), point.y(), point.z()));
        }
    }

    void Select() override {
        Visual::Material selected_mat;
        selected_mat.set_ambient(Visual::Color::SKYBLUE);

        for (auto point : model_point_set_.points()) {
            SigPushVisualPoint_3r(*point, Visual::Point(selected_mat));
        }
    }

    void Deselect() override {
        for (auto point : model_point_set_.points()) {
            SigPopVisualPoint_3r(*point);
        }
    }

    Intersection::Ray_3rSceneObject intersect(const Ray_3r &ray) {
        Intersection::Toleranced::Ray_3rPointSet_3r isect(&ray, &model_point_set_);
        return Intersection::Ray_3rSceneObject(isect.type() == Intersection::Toleranced::Ray_3rPointSet_3r::INTERSECTION_EMPTY, isect.time());
    }

    void UpdateColor(const QColor &color) override {}

    QString scene_object_type() const override {
        return "PointSet";
    }

    const QString& name() const override {
        return name_;
    }
    void set_name(const QString &name) override {
        name_ = name;
    }

    const PointSet_3r& model_point_set() const {
        return model_point_set_;
    }

private:
    PointSet_3r model_point_set_;
    QString name_;
};

//=============================================================================
// Interface: SceneTerrainMesh_3
//=============================================================================

class SceneTerrainMesh_3 : public ISceneObject, public Visual::Geometry {
public:
    SceneTerrainMesh_3() {
        //model_terrain_mesh_.AddObserver(this);
    }

    void Initialize(const std::vector<Point_3f>& data) {
        //model_terrain_mesh_.Initialize(data);
    }

    void Select() override {}
    void Deselect() override {}

    Intersection::Ray_3rSceneObject intersect(const Ray_3r &ray) {
        return Intersection::Ray_3rSceneObject();
    }

    void UpdateColor(const QColor &color) override {}

    QString scene_object_type() const override {
        return "TerrainMesh";
    }

    const QString& name() const override {
        return name_;
    }
    void set_name(const QString &name) override {
        name_ = name;
    }

private:
    //TerrainMesh_3r model_terrain_mesh_;
    QString name_;
};

//=============================================================================
// Interface: SceneObserver
//=============================================================================

class SceneObserver : public QObject, public Visual::IGeometryObserver {

    Q_OBJECT

public:
    SceneObserver();
    ~SceneObserver();

    void SlotRegisterPoint_2r(Point_2r& p) override;

    void SlotPushVisualPoint_2r(const Point_2r& p, const Visual::Point& vp,
        const uint32_t msec_delay = 0) override;

    void SlotPushVisualSegment_2r(const Segment_2r& s,
        const Visual::Segment& vs, const uint32_t msec_delay = 0) override;

    void SlotPushVisualTriangle_2r(const Triangle_2r& t,
        const Visual::Triangle& vt, const uint32_t msec_delay = 0) override;

    void SlotPopVisualPoint_2r(const Point_2r& p,
        const uint32_t msec_delay = 0) override;

    void SlotPopVisualSegment_2r(const Segment_2r& s,
        const uint32_t msec_delay = 0) override;

    void SlotPopVisualTriangle_2r(const Triangle_2r& t,
        const uint32_t msec_delay = 0) override;

    void SlotRegisterPoint_3r(Point_3r& p) override;

    void SlotPushVisualPoint_3r(const Point_3r& p, const Visual::Point& vp,
        const uint32_t msec_delay = 0) override;

    void SlotPushVisualSegment_3r(const Segment_3r& s,
        const Visual::Segment& vs, const uint32_t msec_delay = 0) override;

    void SlotPushVisualTriangle_3r(const Triangle_3r& t,
        const Visual::Triangle& vt, const uint32_t msec_delay = 0) override;

    void SlotPopVisualPoint_3r(const Point_3r& p,
        const uint32_t msec_delay = 0) override;

    void SlotPopVisualSegment_3r(const Segment_3r& s,
        const uint32_t msec_delay = 0) override;

    void SlotPopVisualTriangle_3r(const Triangle_3r& t,
        const uint32_t msec_delay = 0) override;

    void SlotUpdate() override;

    int NumObjects() const;
    const QString& selected_name() const;

public slots:
    // point set
    void onBeginCreatePointSet(const QVector2D& cur);
    void onCreatePointSet(const QVector<QVector3D>& data);
    void onUpdateNewPointSet(const QVector2D& cur);
    void onEndCreatePointSet();
    void onComputeTerrainMeshForSelectedPointSet();

    // polyline
    void onBeginCreatePolyline(const QVector2D& cur);
    void onUpdateNewPolyline(const QVector2D& cur);
    void onEndCreatePolyline();
    void onExecuteMelkman();
    void onComputeMelkmanForSelectedPolyline();

    // polytope
    void onBeginCreatePolytope(const QVector2D& start, const QVector2D& cur);
    void onUpdateNewPolytope(const QVector2D& cur);
    void onEndCreatePolytope();

    void onCreateTerrainMesh(const QVector<QVector3D>& data);

    // picking and selection
    void onUpdateSelectedObjectName(const QString& name);
    void onUpdateSelectedObjectColor(const QColor& color);
    void onDeleteSelectedObject();
    void onDeselect();
    void onSelectObjectFromOrtho(const QVector2D& coords);
    void onSelectObjectFromPerspective(const QVector3D& origin,
                                       const QVector3D& dir);

signals:
    void UpdateVertexBuffer(const quint32 coverage_idx,
                            const quint32 lighting_idx,
                            const quint32 primtype_idx,
                            QVector<GL::Vertex> verts);

    void UpdateContextSensitiveMenus(const QString& selected_obj_type,
                                     const QString& selected_obj_name);

private:
    void GenerateVboPoints();
    void GenerateVboLines();
    void GenerateVboTriangles();

    void onSelectObject(const Ray_3r& selection_ray);

    bool ObjectIsSelected() const;
    ISceneObject* SelectedObject();
    ScenePointSet_3* SelectedPointSet_3();
    ScenePolyline_2* SelectedPolyline_2();
    ScenePolytope_3* SelectedPolytope_3();
    SceneTerrainMesh_3* SelectedTerrainMesh_3();

    QHash<QString, QSharedPointer<ISceneObject>> scene_objects_;
    QVector<QSharedPointer<ISceneObject>> selected_objects_;

    quint32 cur_point_uid_;
    QHash<uint32_t, QSharedPointer<ApproxPoint_3f>> approx_points_;
    QHash<uint32_t, QStack<Visual::Point>> viz_points_;
    QMap<QPair<uint32_t, uint32_t>, QStack<Visual::Segment>> viz_segments_;
    QMap<QVector<uint32_t>, QStack<Visual::Triangle>> viz_triangles_;
};

//=============================================================================
// Interface: SceneManager
//=============================================================================

class SceneManager : public QObject {

    Q_OBJECT

public:
    SceneManager(Renderer* renderer);
    ~SceneManager();

    SceneObserver scene_observer_;

public slots:
    void onUpdateVertexBuffer(const quint32 coverage_idx,
                              const quint32 lighting_idx,
                              const quint32 primtype_idx,
                              QVector<GL::Vertex> verts);

private:
    Renderer* renderer_;
    QSharedPointer<QThread> animation_thread_;
};

} // namespace DDAD

#endif // WB_SCENE_H
