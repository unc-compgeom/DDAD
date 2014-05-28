/*!
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 2013-01-29
 * @brief OpenGL widget that displays an orthogonal view of the current scene.
 */

#ifndef DDAD_RC_QT_WIDGET_ORTHOGRAPHIC_H
#define DDAD_RC_QT_WIDGET_ORTHOGRAPHIC_H

// Qt
#include <QtOpenGL/QGLWidget>
#include <QOpenGLFunctions_3_3_Core>

// Workbench
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

#endif // DDAD_RC_QT_WIDGET_ORTHOGRAPHIC_H
