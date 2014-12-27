#include "qt_polyline_creation_method.h"
#include "ui_polyline_creation_method.h"

PolylineCreationMethod::PolylineCreationMethod(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::PolylineCreationMethod)
{
    ui->setupUi(this);
}

PolylineCreationMethod::~PolylineCreationMethod()
{
    delete ui;
}
