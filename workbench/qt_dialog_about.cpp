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

// DDAD
#include "qt_dialog_about.h"
#include "ui_dialog_about.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog) {
    ui->setupUi(this);
}


AboutDialog::~AboutDialog() {
    delete ui;
}

void AboutDialog::addBuildDate() {
    QString s = ui->aboutMessage->text();
    QString date(__DATE__);
    s.replace("_BD_", date);
    ui->aboutMessage->setText(s);
    setFixedSize(width(), minimumSizeHint().height());
}
