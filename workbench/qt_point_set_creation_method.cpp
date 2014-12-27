#include "qt_point_set_creation_method.h"
#include "ui_point_set_creation_method.h"

PointSetCreationMethod::PointSetCreationMethod(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::PointSetCreationMethod) {
    ui->setupUi(this);

    ui->file_button_choose->hide();
    ui->file_button_generate->hide();
    ui->file_path->hide();
}

PointSetCreationMethod::~PointSetCreationMethod() {
    delete ui;
}

void PointSetCreationMethod::on_method_dropdown_currentTextChanged(const QString &arg1) {
    if (arg1 == "Click") {
        ui->file_button_choose->hide();
        ui->file_button_generate->hide();
        ui->file_path->hide();

        ui->click_text->show();
    } else if (arg1 == "File") {
        ui->click_text->hide();

        ui->file_button_choose->show();
        ui->file_button_generate->show();
        ui->file_path->show();
    } else if (arg1 == "Random") {
        ui->file_button_choose->hide();
        ui->file_button_generate->hide();
        ui->file_path->hide();
        ui->click_text->hide();
    }
}
