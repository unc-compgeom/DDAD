/*!
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 2013-01-29
 * @brief Modal dialog that allows user to alter global configuration settings.
 */

#ifndef RC_QT_DIALOG_PREFERENCES_H
#define RC_QT_DIALOG_PREFERENCES_H

// Qt
#include <QDialog>

// Workbench
#include "common.h"
#include "config.h"

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget *parent = 0);
    ~PreferencesDialog();

private slots:
    void on_selector_color_minor_clicked();

    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_buttonBox_applied();
    void on_buttonBox_defaultsrestored();

    void on_selector_color_major_clicked();

private:
    Ui::PreferencesDialog *ui;
};

#endif // RC_QT_DIALOG_PREFERENCES_H
