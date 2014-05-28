/*!
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 2013-01-29
 * @brief OpenGL widget that allows for navigation with 3D perspective camera.
 */

#ifndef DDAD_RC_QT_WIDGET_PERSPECTIVE_H
#define DDAD_RC_QT_WIDGET_PERSPECTIVE_H

// Qt
#include <QtOpenGL/QGLWidget>
#include <QOpenGLFunctions_3_3_Core>

// Workbench
#include "common.h"
#include "opengl.h"
#include "scene.h"

class QBasicTimer;
class QOpenGLShaderProgram;

class PerspectiveWidget : public QGLWidget,
    protected QOpenGLFunctions_3_3_Core {

    Q_OBJECT

public:
    PerspectiveWidget(QWidget* parent, const QGLWidget* shareWidget = nullptr);
    ~PerspectiveWidget() {
        qDebug() << "perspective delete!";
    }

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void initialize(DDAD::Renderer* renderer, DDAD::SceneManager* scene_manager);

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void handleInput();
    void drawScene();
    void draw2DOverlay();

    static const int kRedrawMsec;
    static const int kMinHintWidth;
    static const int kMinHintHeight;
    static const int kPrefHintWidth;
    static const int kPrefHintHeight;

    quint32 num_frames_;
    QTimer timer_;
    QLinkedList<quint32> frame_times_;
    quint32 total_time_;
    QHash<int, bool> key_states_;
    QMatrix4x4 modelview_;
    QMatrix4x4 projection_;
    QVector3D camera_pos_;
    QVector3D camera_rot_;
    bool camera_active_;

    DDAD::Renderer* renderer_;
    DDAD::SceneManager* scene_manager_;
};

#endif // DDAD_RC_QT_WIDGET_PERSPECTIVE_H
