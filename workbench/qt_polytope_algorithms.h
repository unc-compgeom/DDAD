#ifndef QT_POLYTOPE_ALGORITHMS_H
#define QT_POLYTOPE_ALGORITHMS_H

#include <QGroupBox>

namespace Ui {
class PolytopeAlgorithms;
}

class PolytopeAlgorithms : public QGroupBox
{
    Q_OBJECT

public:
    explicit PolytopeAlgorithms(QWidget *parent = 0);
    ~PolytopeAlgorithms();

private slots:
    void on_compute_clicked();

private:
    Ui::PolytopeAlgorithms *ui;
};

#endif // QT_POLYTOPE_ALGORITHMS_H
