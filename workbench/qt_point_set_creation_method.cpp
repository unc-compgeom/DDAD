/*
 * This file is part of DDAD.
 *
 * DDAD is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * DDAD is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details. You should have received a copy of the GNU General Public
 * License along with DDAD. If not, see <http://www.gnu.org/licenses/>.
 */

#include "qt_point_set_creation_method.h"
#include "ui_point_set_creation_method.h"

#include "common.h"

PointSetCreationMethod::PointSetCreationMethod(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::PointSetCreationMethod) {
    ui->setupUi(this);

    ui->file_button_choose->hide();
    ui->file_button_generate->hide();
    ui->file_name->hide();
}

PointSetCreationMethod::~PointSetCreationMethod() {
    delete ui;
}

void PointSetCreationMethod::on_method_dropdown_currentTextChanged(const QString &arg1) {
    if (arg1 == "Click") {
        ui->file_button_choose->hide();
        ui->file_button_generate->hide();
        ui->file_name->hide();

        ui->click_text->show();
    } else if (arg1 == "File") {
        ui->click_text->hide();

        ui->file_button_choose->show();
        ui->file_button_generate->show();
        ui->file_name->show();
    } else if (arg1 == "Random") {
        ui->file_button_choose->hide();
        ui->file_button_generate->hide();
        ui->file_name->hide();
        ui->click_text->hide();
    }
}

void PointSetCreationMethod::on_file_button_choose_clicked() {
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open point set data"),
        "./",
        tr("Text files (*.txt)")
    );

    ui->file_name->setText(fileName);
}

void PointSetCreationMethod::on_file_button_generate_clicked() {
    LOG(INFO) << "on_generate_clicked with file "
              << ui->file_name->text().toStdString();

    QFile file(ui->file_name->text());

    // error check
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        LOG(ERROR) << "Unable to open point set file.";
        return;
    }

    LOG(INFO) << "Successfully opened point set file. Continuing.";

    QVector<QVector3D> points;
    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine();
        auto tokens = line.split(QRegularExpression("\\s+"), QString::SkipEmptyParts);
        points.push_back(QVector3D(tokens.at(0).toFloat(),
                                   tokens.at(1).toFloat(),
                                   tokens.at(2).toFloat()));
    }

    emit CreatePointSet(points);
}
