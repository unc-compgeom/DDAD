/*!
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 2013-05-23
 * @brief Main window responsible for forwarding widget input.
 */

#ifndef DDAD_RC_QT_WINDOW_MAIN_H
#define DDAD_RC_QT_WINDOW_MAIN_H

// Qt
#include <QMainWindow>

// Workbench
#include "common.h"
#include "scene.h"
#include "logger.h"

BEGIN_NAMESPACE(Ui)

class MainWindow;

END_NAMESPACE(Ui)

class MainWindow : public QMainWindow {

    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    // polytope
    void BeginCreatePolytope(const QString& name, const QColor& face_color);
    void EndCreatePolytope();

    void CreateTerrainMesh(const QVector<QVector3D>& data);

    // all objects
    void UpdateSelectedObjectName(const QString& name);
    void UpdateSelectedObjectColor(const QColor& color);
    void DeleteSelectedObject();
    void Deselect();
    void QuitManager();

private slots:
    void on_action_about_triggered();
    void on_action_preferences_triggered();
    void UpdateStatusBarMsg(const QString& status);

    void onSelectObjectsTriggered();
    void onCreatePolyLineTriggered();
    void onCreatePolytopeTriggered();
    void onCreateArrangementTriggered();
    void onCreateTerrainTriggered();
    void onSnapToGridToggled(bool state);

    void on_actionUser_Manual_triggered();
    void onLogMessage(const QOpenGLDebugMessage& message);

private:
    void initializeLogging();
    void initializeConnections();

    Ui::MainWindow *ui;

    DDAD::Renderer* renderer_;
    DDAD::SceneManager* scene_manager_;

    //QSharedPointer<DDAD::Renderer> renderer_;
    //QSharedPointer<DDAD::SceneManager> scene_manager_;
    //DDAD::RCLogger logger_;

    QColor create_object_color_;
};

#endif // DDAD_RC_QT_WINDOW_MAIN_H
