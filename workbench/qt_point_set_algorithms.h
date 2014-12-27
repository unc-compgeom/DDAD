#ifndef QT_POINT_SET_ALGORITHMS_H
#define QT_POINT_SET_ALGORITHMS_H

#include <QGroupBox>

namespace Ui {
class PointSetAlgorithms;
}

class PointSetAlgorithms : public QGroupBox
{
    Q_OBJECT

public:
    explicit PointSetAlgorithms(QWidget *parent = 0);
    ~PointSetAlgorithms();

signals:
    void ComputeTerrainMeshForSelectedPointSet();

private slots:
    void on_compute_clicked();

private:
    Ui::PointSetAlgorithms *ui;
};

#endif // QT_POINT_SET_ALGORITHMS_H
