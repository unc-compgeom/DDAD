#include "qt_polyline_algorithms.h"
#include "ui_polyline_algorithms.h"

PolylineAlgorithms::PolylineAlgorithms(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::PolylineAlgorithms) {
    ui->setupUi(this);
}

PolylineAlgorithms::~PolylineAlgorithms() {
    delete ui;
}

void PolylineAlgorithms::on_compute_clicked() {
    if (ui->algorithms_dropdown->currentText() == "Melkman") {
        emit ComputeMelkmanForSelectedPolyline();
    }
}
