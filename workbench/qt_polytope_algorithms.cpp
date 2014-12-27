#include "qt_polytope_algorithms.h"
#include "ui_polytope_algorithms.h"

PolytopeAlgorithms::PolytopeAlgorithms(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::PolytopeAlgorithms)
{
    ui->setupUi(this);
}

PolytopeAlgorithms::~PolytopeAlgorithms()
{
    delete ui;
}

void PolytopeAlgorithms::on_compute_clicked() {
    // emit signal
}
