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

// Qt
#include <QColorDialog>

// DDAD
#include "common.h"
#include "qt_dialog_preferences.h"
#include "ui_dialog_preferences.h"

using namespace DDAD;

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog) {
    ui->setupUi(this);

    QPushButton* apply_btn = ui->buttonBox->button(QDialogButtonBox::Apply);
    connect(apply_btn, SIGNAL(clicked()), this, SLOT(on_buttonBox_applied()));
    QPushButton* restore_btn = ui->buttonBox->button(QDialogButtonBox::RestoreDefaults);
    connect(restore_btn, SIGNAL(clicked()), this, SLOT(on_buttonBox_defaultsrestored()));

/*
    QString major_style = QString("border:0;background-color: %1;").arg(
                g_config.grid_major_color_.name());
    ui->selector_color_major->setStyleSheet(major_style);
    QString minor_style = QString("border:0;background-color: %1;").arg(
                g_config.grid_minor_color_.name());
    ui->selector_color_minor->setStyleSheet(minor_style);
    */
}

PreferencesDialog::~PreferencesDialog() {
    delete ui;
}

void PreferencesDialog::on_selector_color_minor_clicked() {
/*
    QColor c = QColorDialog::getColor(g_config.grid_minor_color_, this);
    temp_config_.grid_minor_color_ = c;
    QString minor_style = QString("border:0;background-color: %1;").arg(c.name());
    ui->selector_color_minor->setStyleSheet(minor_style);
    */
}

void PreferencesDialog::on_buttonBox_accepted() {
    //g_config = temp_config_;
}

void PreferencesDialog::on_buttonBox_rejected() {

}

void PreferencesDialog::on_buttonBox_applied() {
    //g_config = temp_config_;
}

void PreferencesDialog::on_buttonBox_defaultsrestored() {
/*
    initializeGlobalConfig();
    temp_config_ = g_config;
    QString minor_style = QString("border:0;background-color: %1;").arg(temp_config_.grid_minor_color_.name());
    ui->selector_color_minor->setStyleSheet(minor_style);
    QString major_style = QString("border:0;background-color: %1;").arg(temp_config_.grid_major_color_.name());
    ui->selector_color_major->setStyleSheet(major_style);
    */
}

void PreferencesDialog::on_selector_color_major_clicked() {
    /*
    QColor c = QColorDialog::getColor(g_config.grid_major_color_, this);
    temp_config_.grid_major_color_ = c;
    QString major_style = QString("border:0;background-color: %1;").arg(c.name());
    ui->selector_color_major->setStyleSheet(major_style);
    */
}
