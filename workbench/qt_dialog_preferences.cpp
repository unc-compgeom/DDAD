/*!
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 01/29/2013
 */

// Qt
#include <QColorDialog>

// Workbench
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
