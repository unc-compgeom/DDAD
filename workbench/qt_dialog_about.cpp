/*!
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 2012-05-23
 */

// Workbench
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
