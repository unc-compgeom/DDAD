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
 * @brief OpenGL widget that displays an orthogonal view of the current scene.
 */

#ifndef WB_QT_WIDGET_ORTHOGRAPHIC_H
#define WB_QT_WIDGET_ORTHOGRAPHIC_H

// Qt
#include <QtOpenGL/QGLWidget>
#include <QOpenGLFunctions_3_3_Core>

// DDAD
#include "common.h"
#include "grid.h"
#include "scene.h"
#include "opengl.h"

class QGLShaderProgram;
class MainWindow;

enum OrthoOrientation {
    BOTTOM,
    TOP,
    LEFT,
    RIGHT,
    FRONT,
    BACK
};

class OrthographicWidget : public QGLWidget,
    protected QOpenGLFunctions_3_3_Core {

    Q_OBJECT

public:
    OrthographicWidget(OrthoOrientation orientation, QWidget* parent = nullptr,
                       const QGLWidget* shareWidget = nullptr);
    ~OrthographicWidget() {
        qDebug() << "ortho delete!";
    }

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void initialize(DDAD::Renderer* renderer,
                    DDAD::SceneManager* scene_manager);

public slots:
    void ShowContextMenu(const QPoint& p);

signals:
    void ChangeMessage(const QString& msg);
    void SelectObject(const QVector2D& coords);

    // pointset
    void BeginCreatePointSet(const QVector2D& start);
    void UpdateNewPointSet(const QVector2D& cur);
    void EndCreatePointSet();

    // polyline
    void BeginCreatePolyline(const QVector2D& start);
    void UpdateNewPolyline(const QVector2D& cur);
    void EndCreatePolyline();
    void ExecuteMelkman();

    // polytope
    void BeginCreatePolytope(const QVector2D& start, const QVector2D& cur);
    void UpdateNewPolytope(const QVector2D& cur);
    void EndCreatePolytope();

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    QVector2D mousePressToWorld(const QPoint& pos) const;
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void drawScene();
    void drawGrid();
    void draw2DOverlay();

    static const int kRedrawMsec;
    static const int kMinHintWidth;
    static const int kMinHintHeight;
    static const int kPrefHintWidth;
    static const int kPrefHintHeight;

    DDAD::Renderer* renderer_;
    DDAD::SceneManager* scene_manager_;

    DDAD::IntegerGrid i_grid_;
    DDAD::GL::RenderGroup i_grid_rg_;

    quint32 num_frames_;
    QTimer timer_;
    QMatrix4x4 modelview_;
    QMatrix4x4 projection_;
    QHash<int, bool> key_states_;
    OrthoOrientation orientation_;
};

#endif // WB_QT_WIDGET_ORTHOGRAPHIC_H
