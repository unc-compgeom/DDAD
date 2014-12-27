#ifndef QT_POLYLINE_ALGORITHMS_H
#define QT_POLYLINE_ALGORITHMS_H

#include <QGroupBox>

namespace Ui {
class PolylineAlgorithms;
}

class PolylineAlgorithms : public QGroupBox
{
    Q_OBJECT

public:
    explicit PolylineAlgorithms(QWidget *parent = 0);
    ~PolylineAlgorithms();

signals:
    void ComputeMelkmanForSelectedPolyline();

private slots:
    void on_compute_clicked();

private:
    Ui::PolylineAlgorithms *ui;
};

#endif // QT_POLYLINE_ALGORITHMS_H
