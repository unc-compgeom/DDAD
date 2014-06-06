/*!
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 2013-01-13
 * @brief Manager type responsible for storing and manipulating scene objects.
 */

#ifndef RC_SCENE_H
#define RC_SCENE_H

// Workbench
#include "common.h"
#include "config.h"
#include "opengl.h"

// geometry geometry
#include "../geometry/polygon.h"
#include "../geometry/polytope.h"
#include "../geometry/triangulation.h"
#include "../geometry/arithmetic.h"
#include "../geometry/line.h"
#include "../geometry/visual.h"
#include "../geometry/triangle.h"
#include "../geometry/arrangement.h"

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

    const Point_3f& approx() const;

private:
    Point_3f approx_;
};

//=============================================================================
// Interface: ISceneObject
//=============================================================================

struct ISceneObject {
    virtual void Select() = 0;
    virtual void Deselect() = 0;
    virtual void UpdateColor(const QColor& color) = 0;
};

//=============================================================================
// Interface: ScenePolyLine_2
//=============================================================================

class ScenePolyLine_2 : public ISceneObject, public Visual::Geometry {
public:
    ScenePolyLine_2() {
        model_polyline_.AddObserver(this);
    }
    const PolyChain_2r& polyline() {
        return model_polyline_;
    }

    void Initialize(const QVector2D& start) {
        model_polyline_.AppendVertex(Point_2r(start.x(), start.y()));
    }

    void Update(const QVector2D& cur) {
        //model_polyline_.Update(Point_3f(cur.x(), cur.y(), 64));
        model_polyline_.AppendVertex(Point_2r(cur.x(), cur.y()));
    }

    void Select() override {}
    void Deselect() override {}
    void UpdateColor(const QColor &color) override {}

private:
    PolyChain_2r model_polyline_;
};

//=============================================================================
// Interface: SceneArrangement_2
//=============================================================================
class SceneArrangement_2 : public ISceneObject, public Visual::Geometry {
public:
    SceneArrangement_2() {
        model_arrangement_.AddObserver(this);
    }
    void InitSceneSegment(const QVector2D& cur, InputColor color) {
        qDebug() << "Init scene segment";
        model_arrangement_.PushPoint(Point_2r(cur.x(), cur.y()), color==RED);
    }
    void AddSceneSegment(const QVector2D& cur) {
        qDebug() << "Add scene segment";
        model_arrangement_.EndSegment(Point_2r(cur.x(), cur.y()));
    }
    const Arrangement_2r& arrangement() { return model_arrangement_; }

    void Select() override {}
    void Deselect() override {}
    void UpdateColor(const QColor &color) override {}

private:
    Arrangement_2r model_arrangement_;
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
                                   Point_3f(cur.x(), cur.y(), 64));
    }

    void Update(const QVector2D& cur) {
        model_polytope_.Update(Point_3f(cur.x(), cur.y(), 64));
    }

    void Select() override {}
    void Deselect() override {}
    void UpdateColor(const QColor &color) override {}

private:
    Polytope_3r model_polytope_;
};

//=============================================================================
// Interface: SceneTerrainMesh_3
//=============================================================================

class SceneTerrainMesh_3 : public ISceneObject, public Visual::Geometry {
public:
    SceneTerrainMesh_3() {
        model_terrain_mesh_.AddObserver(this);
    }

    void Initialize(const std::vector<Point_3f>& data) {
        model_terrain_mesh_.Initialize(data);
    }

    void Select() override {}
    void Deselect() override {}
    void UpdateColor(const QColor &color) override {}

private:
    TerrainMesh_3r model_terrain_mesh_;
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
    void onBeginCreatePolyLine(const QVector2D& start);
    void onUpdateNewPolyLine(const QVector2D& cur);
    void onEndCreatePolyLine();

    void onBeginCreateArrangement(const QVector2D& start);
    void onBeginCreateSegment(const QVector2D& start);
    void onEndCreateSegment(const QVector2D& start);
    void onEndCreateArrangement();
    void onSwitchInputColor();

    void onBeginCreatePolytope(const QVector2D& start, const QVector2D& cur);
    void onUpdateNewPolytope(const QVector2D& cur);
    void onEndCreatePolytope();

    void onCreateTerrainMesh(const QVector<QVector3D>& data);

    void onUpdateSelectedObjectName(const QString& name);
    void onUpdateSelectedObjectColor(const QColor& color);
    void onDeleteSelectedObject();
    void onSelectObject(const QVector2D& coords);
    void onDeselect();

signals:
    void UpdateVertexBuffer(const quint32 coverage_idx,
                            const quint32 lighting_idx,
                            const quint32 primtype_idx,
                            QVector<GL::Vertex> verts);

private:
    void GenerateVboPoints();
    void GenerateVboLines();
    void GenerateVboTriangles();

    bool ObjectIsSelected() const;
    ISceneObject* SelectedObject();
    ScenePolyLine_2* SelectedPolyLine_2();
    SceneArrangement_2* SelectedArrangement_2();
    ScenePolytope_3* SelectedPolytope_3();
    SceneTerrainMesh_3* SelectedTerrainMesh_3();

    QHash<QString, QSharedPointer<ISceneObject>> scene_objects_;
    QString selected_name_;
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

#endif // RC_SCENE_H
