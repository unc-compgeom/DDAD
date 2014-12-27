#include "qt_point_set_algorithms.h"
#include "ui_point_set_algorithms.h"

PointSetAlgorithms::PointSetAlgorithms(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::PointSetAlgorithms) {
    ui->setupUi(this);
}

PointSetAlgorithms::~PointSetAlgorithms() {
    delete ui;
}

void PointSetAlgorithms::on_compute_clicked() {
    if (ui->algorithms_dropdown->currentText() == "Terrain Mesh") {
        emit ComputeTerrainMeshForSelectedPointSet();
    }
}
