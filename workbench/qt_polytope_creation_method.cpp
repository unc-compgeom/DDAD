#include "qt_polytope_creation_method.h"
#include "ui_polytope_creation_method.h"

PolytopeCreationMethod::PolytopeCreationMethod(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::PolytopeCreationMethod)
{
    ui->setupUi(this);
}

PolytopeCreationMethod::~PolytopeCreationMethod()
{
    delete ui;
}
